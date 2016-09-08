#include "session.h"
#include "protocol.h"

BSession::BSession(asio::ip::tcp::socket socket) : m_socket(std::move(socket)),
												 m_dataBuffer(MAX_PACKET_SIZE),
												 m_state(kCreated) {
	m_targetHeader.m_size = -1;
}

BSession::~BSession() {

}

void BSession::Start() {
	m_state = kConnected;
	Receive();
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

