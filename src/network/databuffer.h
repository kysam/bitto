#ifndef h_databuffer
#define h_databuffer

class BDataBuffer
{
public:
	BDataBuffer(int max) {
		m_raw = new unsigned char[max];
		m_maxSize = max;
	};
	~BDataBuffer() {
		delete[] m_raw;
	};

	unsigned char *m_raw;
	int m_wPos;
	int m_maxSize;

public:
	unsigned char *GetIn();
	void AdvancePos(int len);
	int GetWPos() { return m_wPos; };
	int GetAvailable();
};

#endif
