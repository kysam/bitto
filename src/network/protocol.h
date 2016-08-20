#ifndef h_protocol
#define h_protocol

#include "client.h"
#define MAX_PACKET_SIZE 100

class BDataBuffer
{
public:
	BDataBuffer(int max) {
		m_raw = new unsigned char[max];
		m_maxSize = max;
	};
	~BDataBuffer() {
	};

private:
	unsigned char *m_raw;
	int m_pos;
	int m_maxSize;

public:
	unsigned char *GetIn();
	void AdvancePos(int len);
	int GetAvailable();
};

class BProtocol
{
public:
	BProtocol();
	~BProtocol();

	static BProtocol* m_singleton;

	static BProtocol* Get();
	void Process(BClient* client);
};
#endif
