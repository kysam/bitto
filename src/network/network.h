#ifndef h_network
#define h_network
#include "../common/common.h"
#include <vector>
#include "session.h"
#include "protocol.h"
#include "../blist/blist.h"
#include <thread>

struct BMasterTarget {
	BMasterTarget(asio::io_service& io_service, asio::ip::tcp::resolver::iterator& it, BListItemGroup* group)
		: m_socket(io_service),
		m_epIterator(it),
		m_blistGroup(group) {
	};

	asio::ip::tcp::socket m_socket;
	asio::ip::tcp::resolver::iterator m_epIterator;
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
	bool* m_targetConnected;
	float* m_targetElapsed;
	std::thread* m_connThread;
	std::thread* m_ioThread;

	BList *m_blist;

	static BNetwork* Get();
	void Init(BList *blist);
	void Run(const char* name, short port);
	void Listen(const char* ip, short port);
	void Connect();
	void Accept();
};

#endif
