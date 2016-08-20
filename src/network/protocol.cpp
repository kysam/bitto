#include "protocol.h"

BProtocol* BProtocol::m_singleton = nullptr;

BProtocol::BProtocol() {

}

BProtocol::~BProtocol() {

}

BProtocol* BProtocol::Get() {
	if (m_singleton == nullptr) {
		m_singleton = new BProtocol;
	}
	return m_singleton;
}

void BProtocol::Process(BClient* client) {

}

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
