#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fp;

    fp = open("mycp.c", O_RDONLY);
    printf("%d\n", fp);

    exit(0);
}
