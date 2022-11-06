#include "clex.h"
#include "fa.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rule {
  const char *re;
  Node *nfa;
  struct Rule *next;
  int kind;
} Rule;

void initClex(Clex *c, void *buf, size_t len)
{
    c->buf = 0;
    c->len = c->off = 0;
    c->head = c->tail = 0;
    c->arena.buf = buf;
    c->arena.len = len;
    c->arena.off = 0;
}

void beginClex(Clex *c, const char *content, size_t len) {
  c->buf = content;
  c->len = len;
  c->off = 0;
}

void registerKind(Clex *c, const char *re, int kind) {
    Rule *rule = alloc(&c->arena, sizeof(*rule));
    rule->re = re;
    rule->nfa = reToNFA(&c->arena, re);
    rule->kind = kind;
    rule->next = 0;
    if (c->head) {
        c->tail->next = rule;
        c->tail = rule;
    } else {
        c->head = c->tail = rule;
    }
}

Token clex(Clex *c) {
  while (isspace(c->buf[c->off])) c->off++;
  size_t start = c->off;
  while (!isspace(c->buf[++c->off]) && c->buf[c->off] != '\0');
  char *part = alloc(&c->arena, c->off-start+1);
  memcpy(part, c->buf+start, c->off-start);

  while (strlen(part)) {
    for (Rule *rule = c->head; rule; rule = rule->next) {
      bool matches = test(rule->nfa, part);
      if (matches) {
        return (Token){.lexeme = part, .kind = rule->kind};
      }
    }
    part[strlen(part) - 1] = '\0';
    c->off--;
    if (isspace(c->buf[c->off]))
      c->off--;
  }
  abort();
}
