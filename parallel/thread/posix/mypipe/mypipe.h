#ifndef MYPIPE_H__
#define MYPIPE_H__

#define PIPESIZE        1024
#define MYPIPE_READ     0x00000001UL
#define MYPIPE_WRITE    0X00000002UL

typedef void mypipe_t;

mypipe_t *mypipe_init(void);

int mypipe_register(mypipe_t *, int opmap);

int mypipe_unregister(mypipe_t *, int opmap);

int mypipe_read(mypipe_t *, void *buf, size_t size);

int mypipe_write(mypipe_t *, void *buf, const size_t size);

int mypipe_destroy(mypipe_t *);

#endif