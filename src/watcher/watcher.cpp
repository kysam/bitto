#include "watcher/watcher.h"
#include "blog/blog.h"

BWatcher::BWatcher() {
  m_listenSocket = -1;
#ifdef _WIN32
  WSADATA data;
  WSAStartup(0x0202, &data);
#endif
}

BWatcher::~BWatcher() {
#ifdef _WIN32
  WSACleanup();
#endif

  if (m_listenSocket < 0)
    return;

#ifdef __linux__
  close(m_listenSocket);
#elif _WIN32
  closesocket(m_listenSocket);
#endif
}

void BWatcher::SetList(BList *list) {
  m_list = list;
}
void BWatcher::Run() {
  //listening
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_listenSocket < 0) {
		BLog::Log("Listen socket failed to be created");
    return;
	}

  sa.sin_port = htons(31245);
	sa.sin_addr.s_addr = INADDR_ANY;
	if (bind(thisSocket, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
    BLog::Log("Binding socket failed");
    return;
	}

#ifdef __linux__

#elif _WIN32

#endif
  while (true) {

  }
}
