#ifndef h_proto_def
#define h_proto_def
#pragma once

#define MAX_PACKET_SIZE	512
#define PACKET_HEADER_MAGIC	0x2181
#define MAX_PATH_LEN	260
#define MAX_FILE_NAME	260
#define	IsCodeValid(what, code)	(code >= what ## _end) ? true : false

#include "../common/common.h"
#include "../common/utils.h"
#include "../blog/blog.h"
#include "session.h"

enum ProtocolCode : unsigned char {
	//code from slave
	request_checksums,
	//code from master
	request_checksums_answer,
	ProtocolCode_end
};

struct BPHeader {
	BPHeader(ProtocolCode code, unsigned short size) {
		m_code = code;
		m_magic = PACKET_HEADER_MAGIC;
		m_size = size;
	};

	unsigned short m_magic;
	unsigned short m_size;
	ProtocolCode m_code;

	void Get(unsigned char *raw) {
		m_magic = Cast2Value(raw, unsigned short);
		raw += sizeof(unsigned short);
		m_size = Cast2Value(raw, unsigned short);
		raw += sizeof(unsigned short);
		m_code = Cast2Value(raw, ProtocolCode);
	}

};

struct BPacket : BLimitBuffer {
	BPacket(ProtocolCode code) : BLimitBuffer(MAX_PACKET_SIZE) {
		BPHeader header(code, 0);
		Append(&header, sizeof(BPHeader));
	}

	void Pack() {
		GetHeader()->m_size = m_size;
	}

	BPHeader *GetHeader() {
		return Cast2Pointer(m_raw, BPHeader*);
	}
};

/*========================packet operations========================*/

struct BPOp {
	BPOp(BSession *session) {
		m_session = session;
	}

	void ClearPackets() {
		std::vector<BPacket>().swap(m_packets);
		m_cPacketSent = 0;
	}

	BSession *m_session;
	std::vector<BPacket> m_packets;
	int m_cPacketSent;	//number of packets in m_packets sent
	ProtocolCode m_code;

	virtual void Process() = 0;
};

struct BPOp_request_checksums : BPOp {
	//data for both request/answer
	struct ChecksumFile {
		char fileName[MAX_FILE_NAME];
		int checksum;
	};
	struct ChecksumGroup {
		char path[MAX_PATH_LEN];
		std::vector<ChecksumFile> m_files;
	};

	//element codes
	enum ElementCode : unsigned char {
		//request
		checksum_group,
		checksum_file,
	};

	BPOp_request_checksums(BSession *session) : BPOp(session) {
		m_code = request_checksums;
		m_cPacketSent = 0;
	};
	~BPOp_request_checksums() {
	}
	
	std::vector<int> m_checksums;
	std::vector<ChecksumGroup> m_checksumGroups;

	void Process() {
		if (m_session->m_type == BSession::kMaster) {
			BPHeader *header = Cast2Pointer(m_session->m_dataBuffer.m_raw, BPHeader*);
			log_protocol("(master) received request");

			m_packets.push_back(BPacket(request_checksums_answer));
			BPacket *packet = &m_packets.back();
			packet->Pack();
			Send();
		}
		else if (m_session->m_type == BSession::kSlave) {
			BPHeader *header = Cast2Pointer(m_session->m_dataBuffer.m_raw, BPHeader*);
			log_protocol("(slave) received answer for request");
		}
	}
		
	void MapFrom(unsigned char *raw) {	//read the request answer packet
		for (int i = 0; i < m_checksumGroups.size(); i++) {
			for (int ii = 0; ii < m_checksumGroups[i].m_files.size(); ii++) {
				m_checksumGroups[i].m_files[ii].checksum = Cast2Value(raw, int);
				raw += sizeof(int);
			}
		}
	}

	void Build() {	//build the request packet
		//set header for packet
		m_packets.push_back(BPacket(request_checksums));
		BPacket *packet = &m_packets.back();

		//append checksum groups and files
		for (int i = 0; i < m_checksumGroups.size(); i++) {
			if (packet->m_overflowed) {
				packet->Pack();
				m_packets.push_back(BPacket(request_checksums));
				packet = &m_packets.back();
			}

			packet->AppendK<ElementCode>(checksum_group);
			packet->Append(m_checksumGroups[i].path, strlen(m_checksumGroups[i].path));

			for (int ii = 0; ii < m_checksumGroups[i].m_files.size(); ii++) {
				packet->AppendK<ElementCode>(checksum_file);
				packet->Append(m_checksumGroups[i].m_files[ii].fileName, strlen(m_checksumGroups[i].m_files[ii].fileName));
			}
		}
	};

	void Send() {
		asio::async_write(m_session->m_socket, asio::buffer(m_packets[m_cPacketSent].m_raw, m_packets[m_cPacketSent].m_size),
			[this](std::error_code ec, std::size_t cTransferred) {
			if (ec) {
				log_session("write error (code %d)", ec);
				m_session->Terminate();
			}

			if (m_cPacketSent < m_packets.size() - 1) {
				Send();
				m_cPacketSent++;
				return;
			}

			ClearPackets();	//clear after every packet has been sent
		});
	}
};

#endif