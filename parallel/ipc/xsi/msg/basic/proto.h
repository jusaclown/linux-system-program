#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH     "/etc/services"
#define KEYPROJ     'g'
#define BUFFSIZE    1024


struct msg_st {
    long mtype;
    char name[BUFFSIZE];
    int math;
    int chinese;
};


#endif