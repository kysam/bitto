#include "databuffer.h"

unsigned char *BDataBuffer::GetIn() {
	return &m_raw[m_wPos];
}

void BDataBuffer::AdvancePos(int len) {
	if (m_wPos + len > m_maxSize)
		return;
	m_wPos += len;
}

int BDataBuffer::GetAvailable() {
	return (m_maxSize - m_wPos);
}

void BDataBuffer::Reset() {
	m_wPos = 0;
}
