#ifndef h_session
#define h_session
#include "../common/common.h"
#include "databuffer.h"
#include "../blist/blist.h"
#include "../blog/blog.h"

struct BPHeader;
struct BPOp;
class BSession
{
public:
	BSession(asio::ip::tcp::socket socket);
	~BSession();

	enum SessionType {
		kSlave,
		kMaster
	};
	enum State {
		kIdle,
		kConnected,
	};

	BDataBuffer m_dataBuffer;
	SessionType m_type;
	State m_state;
	asio::ip::tcp::socket m_socket;
	BPHeader *m_targetHeader;	//current header to account incoming data for
	BPOp *m_currentOp;
	BList *m_blist;
	BListItemGroup *m_blistGroup;

	void Start();
	void StartSlave();
	void StartMaster();
	void Terminate();
	void Receive();
};

typedef std::shared_ptr<BSession> _ptrSession;
#endif
