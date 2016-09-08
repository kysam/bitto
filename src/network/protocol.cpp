#include "protocol.h"
#include "../common/common.h"
#include "../blog/blog.h"
#include "session.h"

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
	BDataBuffer& buffer = session->m_dataBuffer;
	BPHeader& targetHeader = session->m_targetHeader;

	if (targetHeader.m_size == -1) {;
		auto raw = buffer.GetIn();
		targetHeader.Get(raw);

		if (targetHeader.m_magic != PACKET_HEADER_MAGIC) {
			log_protocol("wrong header magic");
			return;
		}

		if (targetHeader.m_size > MAX_PACKET_SIZE) {
			log_protocol("invalid packet size");
			return;
		}
	}

	if (targetHeader.m_size == buffer.m_wPos) {	//expected packet has come in full
		switch (targetHeader.m_code) {
			
		}
	}

	
}
