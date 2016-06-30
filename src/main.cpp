#include <stdio.h>

int main(int argc, char const *argv[]) {
#ifdef __linux__ 
	printf("sam linux\n")
#elif _WIN32
	printf("sam win\n");
#endif
	return 0;
}
