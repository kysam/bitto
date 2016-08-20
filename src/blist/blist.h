#ifndef h_blist
#define h_blist
#include <vector>

struct BListTarget {
  char addr[16];
  unsigned short port;
  char dir[256];
};

struct BListItem {
  BListTarget a;
  BListTarget b;
};

class BList {
public:
	BList();
	~BList();

	std::vector<BListItem> m_items;
	void Load(const char* fileName);
};
#endif
