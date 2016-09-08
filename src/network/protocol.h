#ifndef h_protocol
#define h_protocol
#include "../common/common.h"

#define log_protocol(msg)	BLog::Log("protocol", msg)

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
