#include "blist.h"
#include "../blog/blog.h"
#include "../common/common.h"
#include <stdio.h>
#include <fstream>
#include "../network/network.h"

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
  
  while(s.getline(lineBuffer, 100)) {
	  if (lineBuffer[0] == '@') {
		  m_itemGroups.push_back(BListItemGroup());
		  currGroup = &m_itemGroups.back();
		  sscanf(lineBuffer, "@%[^:]:%d", currGroup->addr, &currGroup->port);
		  log_blist(currGroup->addr);
		  asio::ip::tcp::resolver resolver(BNetwork::m_io_service);
		  asio::ip::tcp::resolver::query query(currGroup->addr, "");
		  asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
		  
		  strcpy(currGroup->addr, (*it).endpoint().address().to_string().c_str());
		  continue;
	  }

	  if (currGroup == nullptr)
		  continue;

	  BListItem item;
	  sscanf(lineBuffer, "%s:%s", item.a.dir, item.b.dir);
	  currGroup->items.push_back(item);
  }
}
