#include <stdio.h>
#include "common/default.h"
#include "blist/blist.h"
#include "watcher/watcher.h"
#include "blog/blog.h"
#include "network/network.h"
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char const *argv[]) {

	BList list;
	list.Load("blist");
	BWatcher watcher;
	watcher.SetList(&list);
	watcher.Run();

	return 0;
}
