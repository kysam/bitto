#include "common/common.h"
#include "blist/blist.h"

class BConn {
  BConn() {
    m_socket = -1;
  };
  ~BConn();

public:
  int m_socket;
  bool Connect(BListTarget *target);
}
