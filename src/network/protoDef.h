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
		m_magic = Cast2Var(raw, unsigned short);
		raw += sizeof(unsigned short);
		m_size = Cast2Var(raw, unsigned short);
		raw += sizeof(unsigned short);
		m_code = Cast2Var(raw, ProtocolCode);
	}

};

/*========================packet operations========================*/

struct BPOp {
	BPOp(BSession *session) {
		m_session = session;
	}

	BSession *m_session;
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
	
	BSession *m_session;
	std::vector<int> m_checksums;
	std::vector<BLimitBuffer> m_packets;
	std::vector<ChecksumGroup> m_checksumGroups;
	int m_cPacketSent;	//number of packets in m_packets sent

	void Process() {

	}

	void MapFrom(unsigned char *raw) {	//read the request answer packet
		for (int i = 0; i < m_checksumGroups.size(); i++) {
			for (int ii = 0; ii < m_checksumGroups[i].m_files.size(); ii++) {
				m_checksumGroups[i].m_files[ii].checksum = Cast2Var(raw, int);
				raw += sizeof(int);
			}
		}
	}

	void Build() {	//build the request packet
		//set header for packet
		BPHeader header(request_checksums, 0);
		m_packets.push_back(BLimitBuffer(MAX_PACKET_SIZE));
		BLimitBuffer *packet = &m_packets.back();
		packet->Append(&header, sizeof(header));

		//append checksum groups and files
		for (int i = 0; i < m_checksumGroups.size(); i++) {
			if (packet->m_overflowed) {
				//modify current packet's header
				BPHeader *header = Cast2Var(packet->m_raw, BPHeader*);
				header->m_size = packet->m_size;
				m_packets.push_back(BLimitBuffer(MAX_PACKET_SIZE));
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
				log_session("slave session - write error (code %d)", ec);
				m_session->Terminate();
			}

			if (m_cPacketSent < m_packets.size()) {
				Send();
				m_cPacketSent++;
			}
		});
	}
};

#endif