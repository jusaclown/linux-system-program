/*
 *  实现一个简单的shell（只处理外部命令）
 * 
 *  框架图
 *  while(首先一个死循环)
 *      打印提示符
 *      获取输入内容
 *      解析命令
 *          if（内部命令）
 *              dosomething
 *          else（外部命令）{
 *              fork()
 *              让子进程开始干活（用exec）
 *              让父进程等着收尸（用wait）
 *          }
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <wait.h>

#define DELIMS " \t\n"

struct cmd_st {
    glob_t globres;
};

static void prompt(void)
{
    printf("[mysh-0.1]$ ");
}

static void parse(char *line, struct cmd_st *res)
{
    char *token;
    int i = 0;
    
    while (1) {
        token = strsep(&line, DELIMS);    
        if (token == NULL)
            break;
        if (token[0] == '\0')
            continue;
        glob(token, GLOB_NOCHECK|GLOB_APPEND*i, NULL, &res->globres);
        i = 1;
    }
}

int main()
{
    char *linebuf = NULL;
    size_t linebuf_size = 0;
    struct cmd_st cmd;
    pid_t pid;

    while(1) {
        prompt();
    
        if (getline(&linebuf, &linebuf_size, stdin) < 0)
            break;

        parse(linebuf, &cmd);

        if(0) {
        /* do something */
        } else {
            pid = fork();
            if (pid < 0) {
                perror("fork()");
                exit(1);
            } else if (pid == 0) {
                execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
                perror("execvp()");
                exit(1);
            } else {
                wait(NULL);
            }
        }


    }



    exit(0);
}
