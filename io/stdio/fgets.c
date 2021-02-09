#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

int main()
{
    FILE *fp;
    char buff[SIZE];
    int count = 0;

    fp = fopen("tmp", "r");
    if (fp == NULL) {
        perror("fopen()");
        exit(1);
    }

    while(fgets(buff, SIZE - 1, fp) != NULL) {
        //puts(buff);
        //printf("%s", buff);
        fputs(buff, stdout);
        count++;
    }

    printf("%d\n", count);

    fclose(fp);
    exit(0);
}
