#include <stdio.h>
#include <stdlib.h>

int main()
{

    exit(0);
}

ssize_t mygetline(char** line,size_t *n,FILE *fp)
{
    char *buf = *line;
    ssize_t c,i=0;//i来记录字符串长度，c来存储字符
    if(buf==NULL||*n==0)
    {
        *line = malloc(10);
        buf = *line;
        *n = 10;
    }
    //buf为或n为0时动态为期分配空间
    while((c=fgetc(fp))!='\n')
    {
        if(c==EOF)
            return -1;
        if(i<*n-2)//留2个空间给\n和\0
        {
            *(buf+i++)=c;
        }
        else
        {
            *n = *n+10;
            buf = realloc(buf,*n);//空间不足时，重新进行分配
            *(buf+i++)=c;
        }
    }
    *(buf+i++)='\n';
    *(buf+i)='\0';
    return i;

}
