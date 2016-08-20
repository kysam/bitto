#include "../common/common.h"
#include "../blist/blist.h"

class BConn {
public:
  BConn() {
    m_socket = -1;
  };
  ~BConn() {
  };

  int m_socket;
  bool Connect(BListTarget *target);
};
