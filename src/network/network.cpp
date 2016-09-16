#include "network.h"

BNetwork* BNetwork::m_singleton = nullptr;

BNetwork::BNetwork() : m_acceptor(m_io_service),
					m_accSocket(m_io_service)
					{

}

BNetwork::~BNetwork() {

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

	for (int i = 0; i < groups.size(); i++) {
		for (int ii = 0; ii < groups[i].items.size(); ii++) {
			asio::ip::address addr;
			addr.from_string(groups[i].addr);
			m_masterTargets.push_back(BMasterTarget(m_io_service, addr, groups[i].port));
		}
	}
}

void BNetwork::Run(const char* name, short port) {
	Listen(name, port);
	Connect();
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

void BNetwork::Connect() {
	for (int i = 0; i < m_masterTargets.size(); i++) {
		m_masterTargets[i].socket.async_connect(m_masterTargets[i].endPoint,
			[this, i](std::error_code ec) {
			if (!ec) {
				_ptrSession session = std::make_shared<BSession>(std::move(m_masterTargets[i].socket));

				session->m_type = BSession::kSlave;
				session->m_blist = m_blist;
				session->m_blistGroup = m_masterTargets[i].m_blistGroup;
				session->Start();
				return;
			}
			ContinueConnect(i);
		});
	}
}

void BNetwork::ContinueConnect(int idx) {
	m_masterTargets[idx].socket.async_connect(m_masterTargets[idx].endPoint,
		[this, idx](std::error_code ec) {
		if (!ec) {
			_ptrSession session = std::make_shared<BSession>(std::move(m_masterTargets[idx].socket));

			session->m_type = BSession::kSlave;
			session->m_blist = m_blist;
			session->Start();
			return;
		}
		ContinueConnect(idx);
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



