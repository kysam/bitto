// Includes based on OS
#ifdef __linux__
  #include <netdb.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#elif _WIN32
  #include <Winsock2.h>
#endif

#define ASIO_STANDALONE
#define ASIO_HAS_MOVE
#include "../asio.hpp"

#define Cast2Var(raw, type) *reinterpret_cast<type*>(raw)

