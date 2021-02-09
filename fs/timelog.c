/*
 *  打印时间戳 
 *  1 2020-11-26 10：05：06
 *  2 2020-11-26 10：05：07
 *  3 2020-11-26 10：05：08
 *
 */

#include <stdio.h>
#include <stdlib.h>
    #include <time.h>
#include <unistd.h>

#define FILENAME "/tmp/out"
#define BUFFSIZE 1024


int main()
{
    FILE * fp;
    char buf[BUFFSIZE];
    int count;
    time_t stamp;
    struct tm *tm;


    fp = fopen(FILENAME, "a+");
    if (fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }


    while(fgets(buf, BUFFSIZE - 1, fp) != NULL)
        count++;
    
    while(1)
    {   
        time(&stamp);
        tm = localtime(&stamp);
        fprintf(fp, "%-4d%d-%d-%d %d:%d-%d\n", ++count, \
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, \
                tm->tm_hour, tm->tm_min, tm->tm_sec);
        fflush(fp);
        sleep(1);
    }

    fclose(fp);

    exit(0);
}
