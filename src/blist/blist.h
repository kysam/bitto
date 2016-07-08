#include <vector>
using std::vector;
#define BLIST_FILE "blist"

struct BListTarget {
  char addr[16];
};

struct BListItem {
  BListTarget a;
  BListTarget b;
};

class BList {
  BList();
  ~BList();

public:
  vector<BListItem> m_items;
  void Load();
};
