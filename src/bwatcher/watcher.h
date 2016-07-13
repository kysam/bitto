#include "blist/blist.h"

class BWatcher {
  BWatcher();
  ~BWatcher();

public:
  void SetList(BList *list);
  void Run();
}
