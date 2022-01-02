#include "shared_queue.h"

#define header_size sizeof queue->sequence

long closed = -1;

void sq_init(SharedQueue *queue, SharedMem *mem, size_t msg_size) {
    queue->mem = mem;
    queue->msg_size = msg_size;
    queue->sequence = 0;
    queue->prev = -1;
}

long sq_offset(SharedQueue *queue) {
    long offset = (queue->msg_size + header_size) * queue->sequence;
    long next_offset = offset + header_size + queue->msg_size;

    if (next_offset > queue->mem->size) {
        offset = 0;
    }

    return offset;
}

int sq_publish(SharedQueue *queue, void *msg) {
    long offset = sq_offset(queue);

    int errn = sm_write(queue->mem, offset, &queue->sequence, header_size);
    if (errn != 0) {
        return SQ_ERR_WRITE;
    }

    errn = sm_write(queue->mem, offset + header_size, msg, queue->msg_size);
    if (errn != 0) {
        return SQ_ERR_WRITE;
    }

    queue->sequence++;

    return 0;
}

int sq_consume(SharedQueue *queue, void *dest) {
    long offset = sq_offset(queue);
    long producer_sequence;

    int errn = sm_read(queue->mem, offset, &producer_sequence, header_size);
    if (errn != 0) {
        return SQ_ERR_READ;
    }

    if (producer_sequence == closed) {
        return SQ_CLOSED;
    }

    if (producer_sequence == queue->prev || (queue->prev != -1 && producer_sequence != queue->sequence)) {
        return SQ_NO_CONTENT;
    }

    errn = sm_read(queue->mem, offset + header_size, dest, queue->msg_size);
    if (errn != 0) {
        return SQ_ERR_READ;
    }

    queue->prev = producer_sequence;
    queue->sequence++;

    return 0;
}

int sq_close_producer(SharedQueue *queue) {
    int errn = sm_write(queue->mem, 0, &closed, header_size);
    return errn ? SQ_ERR_WRITE : 0;
}