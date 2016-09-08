#ifndef h_network
#define h_network
#include "../common/common.h"
#include <vector>
#include "session.h"
#include "protocol.h"
#include "../blist/blist.h"

class BConnector {
public:
	BConnector(asio::io_service& io_service) {
		m_current = -1;
	}

	struct ConnectorTarget {
		ConnectorTarget() : conSocket(m_io_service) {};
		asio::ip::tcp::socket conSocket;
		asio::ip::tcp::endpoint endPoint;
	};

	std::vector<ConnectorTarget> m_targets;
	int m_current;
	asio::io_service m_io_service;

	ConnectorTarget *GetNext() {
		m_current++;
		if (m_current >= m_targets.size())
			m_current = 0;
		return &m_targets[m_current];
	}
};

class BNetwork {
public:
	BNetwork();
	~BNetwork();

	static BNetwork* m_singleton;
	asio::io_service m_io_service;
	asio::ip::tcp::acceptor m_acceptor;
	std::vector<_ptrSession> m_clients;
	
	asio::ip::tcp::socket m_accSocket;
	BList *m_blist;
	static BNetwork* Get();

	void Init(BList *blist);
	void Run(const char* name, short port);
	void Listen(const char* ip, short port);
	void Connect(BConnector *connector);
	void Accept();
};

#endif
