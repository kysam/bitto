#include "network.h"

BNetwork* BNetwork::m_singleton = nullptr;
asio::io_service BNetwork::m_io_service;

BNetwork::BNetwork() : m_acceptor(m_io_service),
					m_accSocket(m_io_service)
					{

}

BNetwork::~BNetwork() {
	m_connThread->join();
	m_ioThread->join();
	delete m_connThread;
	delete m_ioThread;
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
			m_masterTargets.push_back(BMasterTarget(m_io_service, std::move(it), &groups[i]));
	}

	m_targetConnected = new bool[m_masterTargets.size()];
	m_targetElapsed = new float[m_masterTargets.size()];
	for (int i = 0; i < m_masterTargets.size(); i++) {
		m_targetConnected[i] = false;
		m_targetElapsed[i] = clock() / 1000.0f;
	}
}

void BNetwork::Run(const char* name, short port) {
	m_ioThread = new std::thread([this] {
		m_io_service.run();
	});

	Listen(name, port);
	Connect();
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

void BNetwork::Connect() {
	m_connThread = new std::thread([this] {
		while (true) {
			SleepFor(200);
			for (int i = 0; i < m_masterTargets.size(); i++) {
				if (m_targetConnected[i])
					continue;
				if (((clock() / 1000.0f) - m_targetElapsed[i]) < 0.5f)
					continue;

				m_targetElapsed[i] = clock() / 1000.0f;
				log_network("connecting to %s", m_masterTargets[i].m_epIterator->host_name().c_str());

				asio::async_connect(m_masterTargets[i].m_socket, m_masterTargets[i].m_epIterator,
					[this, i](std::error_code ec, asio::ip::tcp::resolver::iterator) {
					if (!ec) {
						_ptrSession session = std::make_shared<BSession>(std::move(m_masterTargets[i].m_socket));
						session->m_type = BSession::kSlave;
						session->m_blist = m_blist;

						session->m_blistGroup = m_masterTargets[i].m_blistGroup;
						session->Start();
						m_targetConnected[i] = true;
						return;
					}
					log_network("connect to failed, trying again...", m_masterTargets[i].m_epIterator->host_name().c_str());
				});
			}
		}
	});
}

void BNetwork::Accept() {
	m_acceptor.async_accept(m_accSocket,
		[this](std::error_code ec) {
		if (!ec) {
			_ptrSession session = std::make_shared<BSession>(std::move(m_accSocket));
			m_sessions.push_back(session);

			session->m_type = BSession::kMaster;
			session->Start();
			Accept();
		}
	});
}



