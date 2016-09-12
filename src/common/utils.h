#ifndef h_utils
#define h_utils
#pragma once
#include "../network/protoDef.h"

struct BBufferMan {
	BBufferMan(unsigned char *raw, int maxSize) {
		m_pos = 0;
		m_raw = raw;
		m_maxSize = maxSize;
	}

	unsigned char *m_raw;
	int m_pos;
	int m_maxSize;

	bool Append(void *data, int size) {
		if (m_pos + size > m_maxSize)
			return false;

		memcpy(&m_raw[m_pos], data, size);
		m_pos += size;
		return true;
	}

	template<class T>
	bool AppendK(T value) {
		if (m_pos + sizeof(T) > m_maxSize)
			return false;

		memcpy(&m_raw[m_pos], &value, sizeof(T));
		m_pos += sizeof(T);
		return true;
	}
};

struct BPacketBuffers {
	BPacketBuffers() {
		m_available = -1;
	};

	std::vector<asio::streambuf::const_buffers_type> m_packetBuffers;
	int m_available;

	void Add(void *data, int size) {
		while (m_available < size) {
			unsigned char *data = new unsigned char[MAX_PACKET_SIZE];
			asio::buffer buffer(data, MAX_PACKET_SIZE);
			m_packetBuffers.push_back(asio::buffer());
		}
	}
};

#endif
