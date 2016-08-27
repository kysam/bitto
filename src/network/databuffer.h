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

private:
	unsigned char *m_raw;
	int m_pos;
	int m_maxSize;

public:
	unsigned char *GetIn();
	void AdvancePos(int len);
	int GetAvailable();
	unsigned char GetNextSingle();
};

#endif
