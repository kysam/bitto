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
	std::vector<int> m_checksums;
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
		m_packet = nullptr;
		m_code = request_checksums;
	};
	~BPOp_request_checksums() {
		if (m_packet != nullptr)
			delete[] m_packet;
	}
	
	unsigned char *m_packet;
	int m_packetSize;
	std::vector<ChecksumGroup> m_checksumGroups;

	void Process() {	//process this protocol code for m_session

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
		int packetSize = 0;
		packetSize += sizeof(BPHeader);

		for (int i = 0; i < m_checksumGroups.size(); i++) {
			packetSize += strlen(m_checksumGroups[i].path);
			
			for (int ii = 0; ii < m_checksumGroups[i].m_files.size(); ii++) {
				packetSize += strlen(m_checksumGroups[i].m_files[ii].fileName);
			}
			packetSize += sizeof(ElementCode) * m_checksumGroups[i].m_files.size();
		}

		packetSize += sizeof(ElementCode) * m_checksumGroups.size();
		m_packet = new unsigned char[packetSize];
		m_packetSize = packetSize;
		BBufferMan packet(m_packet, packetSize);

		//set header for packet
		BPHeader header(request_checksums, packetSize);
		packet.Append(&header, sizeof(header));

		//append checksum groups and files
		for (int i = 0; i < m_checksumGroups.size(); i++) {
			packet.AppendK<ElementCode>(checksum_group);
			packet.Append(m_checksumGroups[i].path, strlen(m_checksumGroups[i].path));

			for (int ii = 0; ii < m_checksumGroups[i].m_files.size(); ii++) {
				packet.AppendK<ElementCode>(checksum_file);
				packet.Append(m_checksumGroups[i].m_files[ii].fileName, strlen(m_checksumGroups[i].m_files[ii].fileName));
			}
		}
	};

};

#endif