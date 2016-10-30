#include "session.h"
#include "protocol.h"
#include "protoDef.h"

BSession::BSession(asio::io_service& io_service) : m_socket(io_service),
													m_dataBuffer(MAX_PACKET_SIZE),
													m_state(kIdle),
													m_blist(nullptr),
													m_blistGroup(nullptr),
													m_currentOp(nullptr) {
	m_targetHeader = new BPHeader(ProtocolCode_end, 0);
}

BSession::~BSession() {
	delete m_targetHeader;
}

void BSession::Start() {
	m_state = kConnected;
	log_session("starting %s session", m_type == kMaster ? "master" : "slave");
	m_type == kMaster ? StartMaster() : StartSlave();
	
}

void BSession::StartSlave() {
	NewOperation(new BPOp_request_checksums(this));
	BPOp_request_checksums *op = reinterpret_cast<BPOp_request_checksums*>(m_currentOp);
	auto recvHandler = [op] {
		log_protocol("(slave) received answer for request");
	};

	op->SetRecvHandler(recvHandler);
	Receive();
	for (int i = 0; i < m_blistGroup->items.size(); i++) {
		BPOp_request_checksums::ChecksumGroup group;
		memcpy(group.path, m_blistGroup->items[i].b.dir, strlen(m_blistGroup->items[i].b.dir));
		op->m_checksumGroups.push_back(group);
	}

	op->Build();
	op->Send();
}

void BSession::StartMaster() {
	NewOperation(new BPOp_request_checksums(this));
	BPOp_request_checksums *op = reinterpret_cast<BPOp_request_checksums*>(m_currentOp);
	auto recvHandler = [op] {
		log_protocol("(master) received request");
		op->m_packets.push_back(new BPacket(request_checksums));

		BPacket *packet = op->m_packets.back();
		packet->Pack();
		op->Send();
	};

	op->SetRecvHandler(recvHandler);
	Receive();
}

void BSession::Terminate() {
	m_socket.shutdown(asio::socket_base::shutdown_both);
	m_socket.close();
	m_state = kIdle;
}

void BSession::Receive() {
	m_socket.async_read_some(asio::buffer(m_dataBuffer.GetIn(), m_dataBuffer.GetAvailable()), 
		[this](std::error_code ec, std::size_t cTransferred) {
		if (!ec) {
			log_session("%s recvd %d bytes of data", m_type == kMaster ? "Master" : "Slave", cTransferred);
			if (cTransferred > 0) {
				m_dataBuffer.AdvancePos(cTransferred);
				BProtocol::Get()->Process(this);
			}

			Receive();
		}
	});
}

void BSession::NewOperation(BPOp *op) {
	if (m_currentOp)
		delete m_currentOp;

	m_currentOp = op;
}

