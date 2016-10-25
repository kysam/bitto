// Includes based on OS
#ifdef __linux__
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#elif _WIN32
#include <Winsock2.h>
#endif

#include "asio.h"
#include "types.h"

#define Cast2Value(raw, type) *reinterpret_cast<type*>(raw)	//get value of a type from pointer
#define Cast2Pointer(raw, type) reinterpret_cast<type>(raw)
#define SleepFor(mili)	std::this_thread::sleep_for(std::chrono::milliseconds(mili))
