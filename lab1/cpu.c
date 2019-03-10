#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
    fprintf(stderr, "usage: cpu <string>\n");
    exit(1);
    }
    char *str = argv[1];
    int i = 10;
    while (i-- >= 0) {
    sleep(2);
    printf("%s\n", str);
    }
	return 0;
}
