#ifndef PROTO_H__
#define PROTO_H__

#define RCVPORT         "1989"
#define NAMESIZE        11

struct msg_st {
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed));       // 告诉编译器不对齐



#endif