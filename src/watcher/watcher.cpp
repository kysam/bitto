#include "../watcher/watcher.h"
#include "../blog/blog.h"
#define ASIO_STANDALONE
#include "../asio.hpp"
#include "../network/network.h"

BWatcher::BWatcher() {
	m_list = nullptr;
}

BWatcher::~BWatcher() {
}

void BWatcher::SetList(BList *list) {
  m_list = list;
}
void BWatcher::Run() {
	BNetwork::Get()->Init();
	BNetwork::Get()->Run("127.0.0.1", 12345);
}
