#include "../blist/blist.h"
#include "bconn.h"
#include <vector>
#include "../common/common.h"

class BWatcher {
public:
  BWatcher();
  ~BWatcher();

  BList *m_blist;
  std::vector<BConn> m_conns;
  void SetBList(BList *blist);
  void Run();
};
