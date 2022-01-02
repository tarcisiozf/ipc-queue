#ifndef IPC_SHARED_QUEUE_H
#define IPC_SHARED_QUEUE_H

#include <stddef.h>
#include "shared_mem.h"

#define SQ_CLOSED       -2
#define SQ_NO_CONTENT   -1
#define SQ_ERR_WRITE    1
#define SQ_ERR_READ     2

typedef struct SharedQueue {
    SharedMem *mem;
    long sequence;
    long prev;
    size_t msg_size;
} SharedQueue;

void sq_init(SharedQueue *queue, SharedMem *mem, size_t msg_size);

int sq_publish(SharedQueue *queue, void *msg);

int sq_consume(SharedQueue *queue, void *dest);

int sq_close_producer(SharedQueue *queue);

#endif //IPC_SHARED_QUEUE_H
