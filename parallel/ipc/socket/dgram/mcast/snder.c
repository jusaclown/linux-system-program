#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>

#include "proto.h"

int main()
{
    int sd;
    struct msg_st sbuf;
    struct sockaddr_in raddr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("scoket()");
        exit(1);
    }

    struct ip_mreqn mreq;
    inet_pton(AF_INET, MGROUP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("enp4s0f1");

    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq))< 0) {
        perror("setsockopt()");
        exit(1);
    }

    //bind();
    memset(&sbuf, '\0', sizeof(sbuf));
    strcpy(sbuf.name, "Alan");
    sbuf.math = htonl(rand()%100);
    sbuf.chinese = htonl(rand()%100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, MGROUP, &raddr.sin_addr);
    if (sendto(sd, &sbuf, sizeof(sbuf), 0, (void *)&raddr, sizeof(raddr)) < 0) {
        perror("sendto()");
        exit(1);
    }
    puts("ok");

    close(sd);

    exit(0);
}