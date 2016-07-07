#include "blist.h"
#include <fstream>
using std::fstream;

void Blist::Load() {
  fstream s;
  s.open(BLIST_FILE, std::ios::in);
  if (!s.is_open()) {
      
  }
}
