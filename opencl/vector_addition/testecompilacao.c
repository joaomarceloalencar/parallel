#include <stdio.h>

#ifdef __linux__
#define LINUX 1
#elif  _WIN32
#define LINUX 0
#endif

int main(int argc, char *argv[]) {

    if (LINUX) printf("Linux\n");
    else printf("Windows\n");

    return 0;
}