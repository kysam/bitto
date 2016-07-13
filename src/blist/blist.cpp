#include "blist.h"
#include "blog.hs"
#include "common/common.h"
#include <stdio.h>
#include <fstream>

BList::BList() {}

BList::~BList() {}

void BList::Load(const char* fileName) {
  std::fstream s;
  s.open(fileName, std::ios::in);
  if (!s.is_open()) {
    BLog::Log("BList file failed to open");
    return;
  }

  std::string line;
  char *lineStr;
  while(getline(s, line)) {
    lineStr = line.c_str();
    BListTarget a, b;
    sscanf(lineStr, "%s %s:%d:%s", a.dir, b.addr, b.port, b.dir);

    hostent *host = gethostbyname(b.addr);
    strcpy(b.addr, inet_ntoa(host->h_addr_list[0]));
  }
}
