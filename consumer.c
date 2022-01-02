#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include "shared_mem.h"
#include "shared_queue.h"

static volatile int running = 1;

void signalHandler() {
    running = 0;
}

int main() {
    signal(SIGINT, signalHandler);

    SharedMem mem;
    SharedQueue queue;

    int errn = sm_open_reader(&mem, "./data.bin");
    assert(errn == 0);

    int n;
    size_t msg_size = sizeof n;
    sq_init(&queue, &mem, msg_size);

    while (running) {
        if (sq_consume(&queue, &n) == 0) {
            printf("n: %d\n", n);
        }
    }

    errn = sm_close(&mem);
    assert(errn == 0);

    return 0;
}