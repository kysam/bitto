#ifndef h_session
#define h_session
#include "../common/common.h"
#include "protoDef.h"
#include "databuffer.h"

class BSession
{
public:
	BSession(asio::ip::tcp::socket socket);
	~BSession();

	enum State {
		kCreated,
		kConnected,
	};

	BDataBuffer m_dataBuffer;
	State m_state;
	asio::ip::tcp::socket m_socket;
	BPHeader m_targetHeader;

	void Start();
	void Receive();
};

typedef std::shared_ptr<BSession> _ptrSession;
#endif
