#include "network.h"

BNetwork* BNetwork::m_singleton = nullptr;
asio::io_service BNetwork::m_io_service;

BNetwork::BNetwork() : m_acceptor(m_io_service),
					m_accSocket(m_io_service)
					{

}

BNetwork::~BNetwork() {
	Stop();
}

BNetwork* BNetwork::Get() {
	if (m_singleton == nullptr) {
		m_singleton = new BNetwork;
	}
	return m_singleton;
}

void BNetwork::Init(BList *blist) {
	m_blist = blist;
	auto groups = blist->m_itemGroups;

	asio::ip::tcp::resolver resolver(m_io_service);
	for (int i = 0; i < groups.size(); i++) {
			asio::ip::tcp::resolver::iterator it = resolver.resolve({ groups[i].addr, std::to_string(groups[i].port) });
			log_network("target %s:%d", groups[i].addr, groups[i].port);
			m_masterTargets.push_back(BMasterTarget(std::move(it), &groups[i]));
	}

	m_targetConnectState = new ConnectState[m_masterTargets.size()];
	m_targetElapsed = new float[m_masterTargets.size()];
	for (int i = 0; i < m_masterTargets.size(); i++) {
		m_targetConnectState[i] = kNotConnected;
		m_targetElapsed[i] = clock() / 1000.0f;
	}
}

void BNetwork::Run(const char* name, short port) {
	m_ioThread = new std::thread([this] {
		while (true) {
			SleepFor(10);
			m_io_service.run();
		}
	});

	Connect();
	Listen(name, port);
	
}

void BNetwork::Stop() {
	m_connThread->join();
	m_ioThread->join();
	delete m_connThread;
	delete m_ioThread;
}

void BNetwork::Listen(const char* name, short port) {
	asio::ip::tcp::resolver resolver(m_io_service);
	asio::ip::tcp::endpoint endpoint = *resolver.resolve({std::string(name), std::to_string(port)});
	m_acceptor.open(endpoint.protocol());

	m_acceptor.set_option(asio_acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();

	Accept();
}

void BNetwork::Connect() {
	m_connThread = new std::thread([this] {
		while (true) {
			SleepFor(200);
			for (int i = 0; i < m_masterTargets.size(); i++) {
				if (m_targetConnectState[i] == kConnecting || m_targetConnectState[i] == kConnected)
					continue;
				if (((clock() / 1000.0f) - m_targetElapsed[i]) < 0.5f)
					continue;

				m_targetElapsed[i] = clock() / 1000.0f;
				log_network("connecting to %s", m_masterTargets[i].m_epIterator->host_name().c_str());
				m_targetConnectState[i] = kConnecting;

				std::shared_ptr<BSession> session = std::make_shared<BSession>(m_io_service);
				m_sessions.push_back(session);
				asio::async_connect(session->m_socket, m_masterTargets[i].m_epIterator,
					[this, session, i](std::error_code ec, asio::ip::tcp::resolver::iterator) {
					if (!ec) {
						session->m_type = BSession::kSlave;
						session->m_blist = m_blist;

						session->m_blistGroup = m_masterTargets[i].m_blistGroup;
						session->Start();
						m_targetConnectState[i] = kConnected;
						return;
					}

					m_targetConnectState[i] = kNotConnected;
					log_network("connect to %s failed, trying again...", m_masterTargets[i].m_epIterator->host_name().c_str());
				});
			}
		}
	});
}

void BNetwork::Accept() {
	std::shared_ptr<BSession> session = std::make_shared<BSession>(m_io_service);
	m_sessions.push_back(session);
	m_acceptor.async_accept(session->m_socket,
		[this, session](std::error_code ec) {
		if (!ec) {
			session->m_type = BSession::kMaster;
			session->Start();

			Accept();
		}
		else {
			log_network("accept error (code %d)", ec);
		}
	});
}



