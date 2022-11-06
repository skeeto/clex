#ifndef CLEX_H
#define CLEX_H

#include <stddef.h>
#include "arena.h"

typedef struct Rule Rule;

typedef struct Clex {
    const char *buf;
    size_t len;
    size_t off;
    Rule *head;
    Rule *tail;
    Arena arena;
} Clex;

typedef struct Token {
  int kind;
  char *lexeme;
} Token;

void initClex(Clex *, void *, size_t);
void beginClex(Clex *, const char *, size_t);
void registerKind(Clex *, const char *re, int kind);
Token clex(Clex *);

#endif
