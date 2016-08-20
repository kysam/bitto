#ifndef h_client
#define h_client
#include "../common/common.h"
#include "protocol.h"

class BClient
{
public:
	BClient(asio::ip::tcp::socket socket);
	~BClient();

	enum State {
		kDead,
		kConnected,
		kReceiving,
	};

	BDataBuffer m_dataBuffer;
	State m_state;
	asio::ip::tcp::socket m_socket;

	void Start();
	void Receive();
};

typedef std::shared_ptr<BClient> ptrBClient;
#endif
