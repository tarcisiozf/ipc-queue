#include <assert.h>
#include <unistd.h>
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

    int n;
    const size_t size = 4096;
    size_t msg_size = sizeof n;

    SharedMem mem;
    SharedQueue queue;

    int errn = sm_open_writer(&mem, "./data.bin", size);
    assert(errn == 0);

    sq_init(&queue, &mem, msg_size);

    while (running) {
        errn = sq_publish(&queue, &n);
        assert(errn == 0);

        printf("n: %d\n", n);
        n++;

        usleep(200000);
    }

    errn = sq_close_producer(&queue);
    assert(errn == 0);

    errn = sm_close(&mem);
    assert(errn == 0);

    return 0;
}
