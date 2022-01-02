#ifndef IPC_SHARED_H
#define IPC_SHARED_H

#include <stddef.h>

#define E_SM_FOPEN  1
#define E_SM_ALLOC  2
#define E_SM_MAP    3
#define E_SM_FSTAT  4
#define E_SM_UNMAP  5
#define E_SM_FCLOSE 6

typedef struct SharedMem {
    int fd;
    size_t size;
    void *ptr;
} SharedMem;

int sm_open_writer(SharedMem *mem, const char *path, size_t size);

int sm_open_reader(SharedMem *mem, const char *path);

int sm_write(SharedMem *mem, size_t offset, void *data, size_t len);

int sm_read(SharedMem *mem, size_t offset, void *dest, size_t len);

int sm_close(SharedMem *mem);

#endif //IPC_SHARED_H
