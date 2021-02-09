/*
 * 判断模式匹配
 * 功能：打印符合PAT格式的路径
 * 不包括隐藏文件， 如果要打印隐藏文件
 * 需要再弄一个匹配模式，比如/etc/.*
 */

#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

//#define PAT "/etc/a*.conf"
#define PAT "/etc/"

#if 0
int errfunc_(const char *errpath, int eerrno)
{
    puts(errpath);
    fprintf(stderr, "ERROR MSG:%s\n", strerror(eerror));
    return 0;
}
#endif

int main()
{
    int err, i;
    glob_t globres;

    err = glob(PAT, 0, NULL, &globres);
    if(err)
    {
        printf("ERROR CODE = %d\n", err);
        exit(1);
    }

    for(i = 0; i < globres.gl_pathc; i++)
        puts(globres.gl_pathv[i]);

    globfree(&globres);
    exit(0);
}
