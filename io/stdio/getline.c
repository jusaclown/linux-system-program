#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp;
    char *linebuf = NULL;  // 这两个初始化非常重要！！！！！！！！
    size_t linesize = 0;

    if(argc != 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }

    while(1)
    {    
        if(getline(&linebuf, &linesize, fp) < 0)
            break;
        printf("%d\n", strlen(linebuf));
        printf("%d\n", linesize);
    }
    
    free(linebuf);
    fclose(fp);
    exit(0);
}
