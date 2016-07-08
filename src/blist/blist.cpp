#include "blist.h"
#include <fstream>
using std::fstream;

BList::BList() {}

BList::~BList() {}

void BList::Load() {
  fstream s;
  s.open(BLIST_FILE, std::ios::in);
  if (!s.is_open()) {
      
  }
}
