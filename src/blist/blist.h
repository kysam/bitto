#ifndef h_blist
#define h_blist
#include <vector>


struct BListTarget {
	char dir[260];
};

struct BListItem {
	BListTarget a;
	BListTarget b;
};

struct BListItemGroup {
	char addr[16];
	unsigned short port;
	std::vector<BListItem> items;
};

class BList {
public:
	BList();
	~BList();

	std::vector<BListItemGroup> m_itemGroups;
	void Load(const char* fileName);
};
#endif
