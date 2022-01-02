#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "shared_mem.h"

int sm_open_writer(SharedMem *mem, const char *path, size_t size) {
    mem->size = size;
    mem->fd = open(path, O_RDWR | O_CREAT, (mode_t) 0600);
    if (mem->fd < 0) {
        return E_SM_FOPEN;
    }

    // ensure file space is allocated
    int err = posix_fallocate(mem->fd, 0, (off_t) size);
    if (err != 0) {
        return E_SM_ALLOC;
    }

    mem->ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, mem->fd, 0);
    if (mem->ptr == MAP_FAILED) {
        return E_SM_MAP;
    }

    return 0;
}

int sm_open_reader(SharedMem *mem, const char *path) {
    mem->fd = open(path, O_RDWR, (mode_t) 0600);
    if (mem->fd < 0) {
        return E_SM_FOPEN;
    }

    struct stat statbuf;
    int err = fstat(mem->fd, &statbuf);
    if (err < 0) {
        return E_SM_FSTAT;
    }
    mem->size = statbuf.st_size;

    mem->ptr = mmap(0, mem->size, PROT_READ, MAP_SHARED, mem->fd, 0);
    if (mem->ptr == MAP_FAILED) {
        return E_SM_MAP;
    }

    return 0;
}

int sm_write(SharedMem *mem, size_t offset, void *data, size_t len) {
    memcpy(mem->ptr + offset, data, len);
    return 0;
}

int sm_read(SharedMem *mem, size_t offset, void *dest, size_t len) {
    memcpy(dest, mem->ptr + offset, len);
    return 0;
}

int sm_close(SharedMem *mem) {
    int err = munmap(mem->ptr, mem->size);
    if (err != 0) {
        return E_SM_UNMAP;
    }

    err = close(mem->fd);
    if (err != 0) {
        return E_SM_FCLOSE;
    }

    return 0;
}