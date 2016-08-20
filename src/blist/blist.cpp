#include "blist.h"
#include "../blog/blog.h"
#include "../common/common.h"
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

  char lineBuffer[100];
  memset(lineBuffer, 0, sizeof(lineBuffer));

  while(s.getline(lineBuffer, 0)) {
    BListTarget a, b;
    sscanf(lineBuffer, "%s %s:%d:%s", a.dir, b.addr, b.port, b.dir);

    hostent *host = gethostbyname(b.addr);
		in_addr** addresses = reinterpret_cast<in_addr**>(host->h_addr_list);
		strcpy(b.addr, inet_ntoa(*addresses[0]));
  }
}
