#include "../blist/blist.h"
#include "bconn.h"
#include <vector>
#include "../common/common.h"

class BWatcher {
public:
  BWatcher();
  ~BWatcher();

  BList *m_list;
  std::vector<BConn> m_conns;
  void SetList(BList *list);
  void Run();
};
