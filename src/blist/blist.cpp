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
    log_blist("file failed to open");
    return;
  }

  char lineBuffer[100];
  memset(lineBuffer, 0, sizeof(lineBuffer));

  BListItemGroup *currGroup = nullptr;
  while(s.getline(lineBuffer, 0)) {
	  if (lineBuffer[0] == '@') {
		  m_itemGroups.push_back(BListItemGroup());
		  currGroup = &m_itemGroups.back();
		  sscanf(lineBuffer, "@%s:%d", currGroup->addr, &currGroup->port);

		  hostent *host = gethostbyname(currGroup->addr);
		  in_addr** addresses = reinterpret_cast<in_addr**>(host->h_addr_list);
		  strcpy(currGroup->addr, inet_ntoa(*addresses[0]));
		  continue;
	  }

	  if (currGroup == nullptr)
		  continue;

	  BListItem item;
	  sscanf(lineBuffer, "%s:%s", item.a.dir, item.b.dir);
	  currGroup->items.push_back(item);
  }
}
