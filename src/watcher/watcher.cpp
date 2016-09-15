#include "../watcher/watcher.h"
#include "../blog/blog.h"
#include "../network/network.h"

BWatcher::BWatcher() {
	m_blist = nullptr;
}

BWatcher::~BWatcher() {
}

void BWatcher::SetBList(BList *blist) {
  m_blist = blist;
}
void BWatcher::Run() {
	BNetwork::Get()->Init(m_blist);
	BNetwork::Get()->Run("127.0.0.1", 12345);
}
