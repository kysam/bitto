#include "session.h"
#include "protocol.h"
#include "protoDef.h"

BSession::BSession(asio::ip::tcp::socket socket) : m_socket(std::move(socket)),
													m_dataBuffer(MAX_PACKET_SIZE),
													m_state(kIdle),
													m_targetHeader(ProtocolCode_end, -1),
													m_blist(nullptr),
													m_blistGroup(nullptr) {
}

BSession::~BSession() {

}

void BSession::Start() {
	m_state = kConnected;
	m_type == kMaster ? StartMaster() : StartSlave();
	
}

void BSession::StartSlave() {
	BPOp_request_checksums *op = new BPOp_request_checksums(this);
	m_currentOp = op;

	for (int i = 0; i < m_blistGroup->items.size(); i++) {
		BPOp_request_checksums::ChecksumGroup group;
		memcpy(group.path, m_blistGroup->items[i].b.dir, strlen(m_blistGroup->items[i].b.dir));
		op->m_checksumGroups.push_back(group);
	}

	op->Build();
	asio::async_write(m_socket, asio::buffer(op->m_packet, op->m_packetSize),
		[this](std::error_code ec, std::size_t cTransferred) {
		if (ec) {
			log_session("slave session - write error (code %d)", ec);
			Terminate();
			}
		});
	Receive();
}

void BSession::StartMaster() {
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
			if (cTransferred <= 0)
				return;
			
			m_dataBuffer.AdvancePos(cTransferred);
			BProtocol::Get()->Process(this);
			Receive();
		}
	});
}

