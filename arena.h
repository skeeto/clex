#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct Arena {
    char *buf;
    size_t len;
    size_t off;
} Arena;

void *alloc(Arena *, size_t);

#endif
