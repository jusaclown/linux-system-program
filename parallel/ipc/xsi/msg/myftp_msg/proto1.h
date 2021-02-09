#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH     "/etc/services"
#define KEYPROJ     'a'

#define PATHMAX     1024
#define DATAMAX     1024

enum {
    MSG_PATH = 1;
    MSG_DATA;
    MSG_EOT;
};

typedef struct msg_path_st {
    long mtype;             /* must be MSG_PATH*/
    char path[PATHMAX];     /* asciiz 带尾0的串*/
}msg_path_t;

typedef struct msg_data_st {
    long mtype;             /* must be MSG_DATA*/
    char data[DATAMAX];
    int datalen;
}msg_data_t;

typedef struct msg_eot_st {
    long mtype;             /* must be MSG_EOT*/ 
}msg_eot_t;

union msg_s2c_un {
    long mtype;
    msg_data_t datamsg;
    msg_eot_t eotmsg
};


#endif
