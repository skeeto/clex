#include <stdlib.h>
#include <string.h>
#include "arena.h"

void *alloc(Arena *arena, size_t len)
{
    len = (len + 7) & ~7;
    if (arena->len - arena->off < len) {
        return 0;
    }
    void *p = arena->buf + arena->off;
    arena->off += len;
    memset(p, 0, len);
    return p;
}
