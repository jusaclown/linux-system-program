/*
 *  创建守护进程
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

#define FILENAME "/tmp/out"

static int daemonize(void)
{
    int fd;
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid > 0) // 父进程
        exit(0);
    else {
        fd = open("/dev/null", O_RDWR);
        if (fd < 0) {
            return -1;
        }
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        if (fd > 2)
            close(fd);

        setsid();
        chdir("/");
//        umask(0);
        return 0;
    }
}

int main()
{
    FILE *fp;
    int i;

    openlog("Mydaemon", LOG_PID, LOG_DAEMON);

    if (daemonize()) {
        syslog(LOG_ERR, "daemonize() failed!");
        exit(1);
    } else {
        syslog(LOG_INFO, "daemonize() successded!");
    }
    
    fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        syslog(LOG_ERR, "fopen():%s", strerror(errno));
        exit(1);
    }

    syslog(LOG_INFO, "%s was opended.", FILENAME);

    for (i = 0; ; i++) {
        fprintf(fp, "%d\n", i);
        fflush(fp);
        syslog(LOG_DEBUG, "%d is printed", i);
        sleep(1);
    }
    
    fclose(fp);
    closelog();

    exit(0);
}
