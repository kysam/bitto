#include "blist/blist.h"
#include "bconn.h"
#include <vector>

class BWatcher {
  BWatcher();
  ~BWatcher();

public:
  int m_listenSocket;
  BList *m_list;
  std::vector<BConn> m_conns;
  void SetList(BList *list);
  void Run();
}
