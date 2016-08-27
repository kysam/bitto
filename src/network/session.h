#ifndef h_session
#define h_session
#include "../common/common.h"
#include "protocol.h"
#include "databuffer.h"

class BDataBuffer;
class BSession
{
public:
	BSession(asio::ip::tcp::socket socket);
	~BSession();

	enum State {
		kCreated,
		kConnected,
		kReceiving,
	};

	BDataBuffer m_dataBuffer;
	State m_state;
	asio::ip::tcp::socket m_socket;

	void Start();
	void Receive();
};

typedef std::shared_ptr<BSession> _ptrSession;
#endif
