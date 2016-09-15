#ifndef h_utils
#define h_utils
#pragma once
#include "../network/protoDef.h"

struct BLimitBuffer {
	BLimitBuffer(int maxSize) {
		m_size = 0;
		m_raw = new unsigned char[maxSize];
		m_maxSize = maxSize;
		m_overflowed = false;
	}

	~BLimitBuffer() {
		delete[] m_raw;
	}

	unsigned char *m_raw;
	unsigned int m_size;
	unsigned int m_maxSize;
	bool m_overflowed;	//has there been at least a failed append call?

	bool Append(void *data, int size) {
		if (m_size + size > m_maxSize) {
			m_overflowed = true;
			return false;
		}

		memcpy(&m_raw[m_size], data, size);
		m_size += size;
		return true;
	}

	template<class T>
	bool AppendK(T value) {
		if (m_size + sizeof(T) > m_maxSize) {
			m_overflowed = true;
			return false;
		}

		memcpy(&m_raw[m_size], &value, sizeof(T));
		m_size += sizeof(T);
		return true;
	}
};

#endif
