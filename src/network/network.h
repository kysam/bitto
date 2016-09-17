#ifndef h_network
#define h_network
#include "../common/common.h"
#include <vector>
#include "session.h"
#include "protocol.h"
#include "../blist/blist.h"


struct BMasterTarget {
	BMasterTarget(asio::io_service& io_service, asio::ip::address& address, unsigned short port) 
		: socket(io_service),
		endPoint(address, port),
		m_blistGroup(nullptr) {};

	asio::ip::tcp::socket socket;
	asio::ip::tcp::endpoint endPoint;
	BListItemGroup *m_blistGroup;
};

class BNetwork {
public:
	BNetwork();
	~BNetwork();


	static BNetwork* m_singleton;
	static asio::io_service m_io_service;
	asio::ip::tcp::acceptor m_acceptor;
	std::vector<_ptrSession> m_sessions;
	asio::ip::tcp::socket m_accSocket;
	std::vector<BMasterTarget> m_masterTargets;
	BList *m_blist;

	static BNetwork* Get();
	void Init(BList *blist);
	void Run(const char* name, short port);
	void Listen(const char* ip, short port);
	void Connect();
	void ContinueConnect(int idx);
	void Accept();
};

#endif
