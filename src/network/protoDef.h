#ifndef h_proto_def
#define h_proto_def
#pragma once
#include "../common/common.h"

#define MAX_PACKET_SIZE 512
#define PACKET_HEADER_MAGIC 0x2181

enum ProtocolCode : unsigned char {

};

struct BPHeader {
	BPHeader() {};
	void Get(unsigned char *raw) {
		m_magic = CAST2VAR(raw, unsigned short);
		raw += sizeof(unsigned short);
		m_size = CAST2VAR(raw, int);
		raw += sizeof(int);
		m_code = CAST2VAR(raw, ProtocolCode);
	}
	unsigned short m_magic;
	int m_size;
	ProtocolCode m_code;
};
#endif