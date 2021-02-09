/*  
 *  打印所有的环境变量
 *  envriron 存放环境变量的字符数组  
 */

#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main()
{
    int i;

    for (i = 0; environ[i] != NULL; i++)
        puts(environ[i]);



    exit(0);
}
