#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "proto.h"

int main(int argc, char *argv[])
{
    int sd;
    struct msg_st sbuf;
    struct sockaddr_in raddr;

    if (argc < 2) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("scoket()");
        exit(1);
    }

    //bind();
    strcpy(sbuf.name, "Alan");                       // 为什么这里不用转化为网络字节序
    sbuf.math = htonl(rand()%100);
    sbuf.chinese = htonl(rand()%100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, argv[1], &raddr.sin_addr);   // 为什么这里不用转化为网络字节序
    if (sendto(sd, &sbuf, sizeof(sbuf), 0, (void *)&raddr, sizeof(raddr)) < 0) {
        perror("sendto()");
        exit(1);
    }
    puts("ok");

    close(sd);

    exit(0);
}