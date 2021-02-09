#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <net/if.h>

#include <proto.h>
#include <client.h>


/*
 * -M   --mgroup    指定多播组
 * -P   --port      指定接收端口
 * -p   --player    指定播放器
 * -H   --help      显示帮助
 *
 */
struct client_conf_st client_conf = {\
        .rcvport = DEFAULT_RCVPORT, \
        .mgroup = DEFAULT_MGROUP, \
        .player_cmd == DEFAULT_PLAYERCMD
};

static void print_help(char *argv)
{
    printf("Usage: %s [-M mgroup] [-P port] [-p player]\n", argv)
    printf("-M   --mgroup    指定多播组\n");
    printf("-P   --port      指定接收端口\n");
    printf("-p   --player    指定播放器\n");
    printf("-H   --help      显示帮助\n");
}

static void error_pton(int epton)
{
    if (epton <= 0) {
        if (epton == 0)
            fprintf(stderr, "Not in presentation format");
        else
            perror("inet_pton");
        exit(1);
    }
}

int main(int argc, char **argv)
{
    /*
     *  初始化
     *  级别：默认值，配置文件，环境变量，命令行参数
     */
    int c;
    int sd;
    int epton;
    int index = 0;
    struct ip_mreqn mreq;
    struct option argarr[] = {{"port",   1, NULL, 'P'}, \
                              {"mgroup", 1, NULL, 'M'}, \
                              {"player", 1, NULL, 'p'}, \
                              {"help",   0, NULL, 'H'}, \
                              {0,        0, 0,      0}};

    while ((c = getopt_long(argc, argv, "P:M:p:H", , &index)) != -1) {
        switch(c) {
            case 'P':
                client_conf.rcvport = optarg;
                break;
            case 'M':
                client_conf.mgroup = optarg;
                break;
            case 'p':
                client_conf.player_cmd = optarg;
                break;
            case 'H':
                print_help(argv[0]);
                exit(0);
                break;
            default:
                print_help(argv[0]);
                abort();
                break;
        }
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    epton = inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
    error_pton(epton);
    epton = inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    error_pton(epton);
    mreq.imr_ifindex = if_nametoindex("enp4s0f1");
    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt()");
        exit(1);
    }

    bind();

    pipe();

    fork();
    // 子进程：调用解码器

    // 父进程：从网络上收包，发送给子进程


    exit(0);
}