#include "protocol.h"
#include "../common/common.h"
#include "../blog/blog.h"
#include "session.h"
#include "protoDef.h"

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
	BPHeader *targetHeader = session->m_targetHeader;


	if (targetHeader->m_size == -1) {
		auto raw = buffer.m_raw;
		targetHeader->Get(raw);

		if (targetHeader->m_magic != PACKET_HEADER_MAGIC) {
			log_protocol("wrong header magic");
			return;
		}

		if (targetHeader->m_size > MAX_PACKET_SIZE) {
			log_protocol("invalid packet size");
			return;
		}
	}

	BPHeader *rawHeader = (BPHeader*)buffer.m_raw;
	printf("protocol process %d %d\n", rawHeader->m_size, buffer.m_wPos);


	if (targetHeader->m_size == buffer.m_wPos) {	//expected packet has come in full
		if (!IsCodeValid(ProtocolCode, session->m_targetHeader->m_code)) {
			log_protocol("unknown protocol code, terminating session");
			session->Terminate();
			return;
		}

		if (session->m_currentOp->m_code != session->m_targetHeader->m_code) {
			log_protocol("operation mismatch, terminating session");
			session->Terminate();
			return;
		}

		printf("protocol process 2\n");


		if (session->m_currentOp)
			session->m_currentOp->Process();	//process current operation

		targetHeader->m_size = -1;
		buffer.Reset();
	}
}
