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

void BProtocol::Process(BSession *session) {
	
}
