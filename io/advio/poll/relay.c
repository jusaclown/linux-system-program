#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>

#define TTY1        "/dev/tty11"
#define TTY2        "/dev/tty12"
#define BUFSIZE     1024

enum {
    STATE_R = 1,
    STATE_W,
    STATE_AUTO,
    STATE_Ex,
    STATE_T
};

struct fsm_st {
    int state;
    int sfd;
    int dfd;
    int len;
    int pos;
    char buf[BUFSIZE];
    char *errstr;
};

static void fsm_driver(struct fsm_st *fsm)
{
    int ret;

    switch (fsm->state) {
    case STATE_R:
        fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
        if (fsm->len == 0)
            fsm->state = STATE_T;
        else if (fsm->len < 0) {
            if (errno == EAGAIN)
                fsm->state = STATE_R;
            else if (errno == EINTR)
                fsm->state = STATE_R;
            else {
                fsm->errstr = "open()";
                fsm->state = STATE_Ex;
            }
        } else {
            fsm->pos = 0;
            fsm->state = STATE_W;
        }
        break;

    case STATE_W:
        ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
        if (ret < 0) {
            if (errno == EAGAIN)
                fsm->state = STATE_W;
            else if (errno == EINTR)
                fsm->state = STATE_W;
            else {
                fsm->errstr = "write()";
                fsm->state = STATE_Ex;
            }
        } else {
            fsm->pos += ret;
            fsm->len -= ret;

            if (fsm->len == 0)
                fsm->state = STATE_R;
            else
                fsm->state = STATE_W;
        }
        break;

    case STATE_Ex:
        perror(fsm->errstr);
        fsm->state = STATE_T;
        break;

    case STATE_T:
        /*do something*/
        break;

    default:
        abort();
        break;

    }

}

static int max(int a, int b) 
{
    if (a > b) 
        return a;
    return b;
}

static void relay(int fd1, int fd2)
{
    int fd1_save, fd2_save;
    struct fsm_st fsm12, fsm21;
    struct pollfd pfd[2];


    /* fcntl 给open加上非阻塞状态 */
    fd1_save = fcntl(fd1, F_GETFL);
    fcntl(fd1, F_SETFL, fd1_save|O_NONBLOCK);

    fd2_save = fcntl(fd2, F_GETFL);
    fcntl(fd1, F_SETFL, fd1_save|O_NONBLOCK);

    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;
    fsm21.state= STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    pfd[0].fd = fd1;
    pfd[1].fd = fd2;

    while (fsm12.state != STATE_T || fsm21.state != STATE_T) {
        // 布置监视任务
        pfd[0].events = 0;
        if (fsm12.state == STATE_R)
            pfd[0].events |= POLLIN;
        if (fsm21.state == STATE_W)
            pfd[0].events |= POLLOUT;

        pfd[1].events = 0;
        if (fsm12.state == STATE_W)
            pfd[1].events |= POLLOUT;
        if (fsm21.state == STATE_R)
            pfd[1].events |= POLLIN;

        // 监视
        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO) {
            while (poll(pfd, 2, -1) < 0) {
                if (errno == EINTR)
                    continue;
                perror("poll()");
                exit(1);
            }
        }
        // 查看监视结果
        
        // 根据监视结果推动状态机
        if (pfd[0].revents & POLLIN || pfd[1].events & POLLOUT \
                || fsm12.state > STATE_AUTO)
            fsm_driver(&fsm12);
        if (pfd[1].revents & POLLIN || pfd[0].revents & POLLOUT || \
                fsm21.state > STATE_AUTO)
            fsm_driver(&fsm21);
    }


    fcntl(fd1, F_SETFL, fd1_save);
    fcntl(fd2, F_SETFL, fd2_save);
}

int main()
{
    int fd1, fd2;

    fd1 = open(TTY1, O_RDWR);
    if (fd1 < 0) {
        perror("open()");
        exit(1);
    }
    write(fd1, "TTY1\n", 5);

    fd2 = open(TTY2, O_RDWR|O_NONBLOCK);
    if (fd2 < 0) {
        perror("open()");
        exit(1);
    }
    write(fd2, "TTY2\n", 5);

    relay(fd1, fd2);

    close(fd2);
    close(fd1);
    exit(0);
}
