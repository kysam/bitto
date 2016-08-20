#include "client.h"

BClient::BClient(asio::ip::tcp::socket socket) : m_socket(std::move(socket)),
																								 m_dataBuffer(MAX_PACKET_SIZE),
																								 m_state(kDead) {

}

BClient::~BClient() {

}

void BClient::Start() {
	m_state = kConnected;
	Receive();
}

void BClient::Receive() {
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

