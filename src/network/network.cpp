#include "network.h"

BNetwork* BNetwork::m_singleton = nullptr;

BNetwork::BNetwork() : m_acceptor(m_io_service),
											 m_accSocket(m_io_service) {

}

BNetwork::~BNetwork() {

}

BNetwork* BNetwork::Get() {
	if (m_singleton == nullptr) {
		m_singleton = new BNetwork;
	}
	return m_singleton;
}

void BNetwork::Init() {
	
}

void BNetwork::Run(const char* name, short port) {
	Listen(name, port);
	m_io_service.run();
}

void BNetwork::Listen(const char* name, short port) {
	asio::ip::tcp::resolver resolver(m_io_service);
	asio::ip::tcp::endpoint endpoint = *resolver.resolve({std::string(name), std::to_string(port)});
	m_acceptor.open(endpoint.protocol());

	m_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();

	Accept();
}

void BNetwork::Accept() {
	m_acceptor.async_accept(m_accSocket,
		[this](std::error_code ec) {
		if (!ec) {
			ptrBClient client = std::make_shared<BClient>(std::move(m_accSocket));
			client->Start();
			m_clients.push_back(client);
			Accept();
		}
	});
}



