#include "databuffer.h"

unsigned char *BDataBuffer::GetIn() {
	return &m_raw[m_pos];
}

void BDataBuffer::AdvancePos(int len) {
	if (m_pos + len > m_maxSize)
		return;
	m_pos += len;
}

int BDataBuffer::GetAvailable() {
	return m_maxSize;
}

unsigned char BDataBuffer::GetNextSingle() {
	
}
