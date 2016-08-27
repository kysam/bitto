#ifndef h_protocol
#define h_protocol

#include "session.h"
#define MAX_PACKET_SIZE 100

class BSession;
class BProtocol
{
public:
	BProtocol();
	~BProtocol();

	static BProtocol* m_singleton;

	static BProtocol* Get();
	void Process(BSession *session);
};
#endif
