#ifndef h_network
#define h_network
#include "../common/common.h"
#include <vector>
#include "session.h"

class BNetwork {
public:
	BNetwork();
	~BNetwork();

	static BNetwork* m_singleton;
	asio::io_service m_io_service;
	asio::ip::tcp::acceptor m_acceptor;
	std::vector<_ptrSession> m_clients;
	asio::ip::tcp::socket m_accSocket;
	static BNetwork* Get();

	void Init();
	void Run(const char* name, short port);
	void Listen(const char* ip, short port);
	void Accept();
};

#endif
