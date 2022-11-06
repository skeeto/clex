#ifndef FA_H
#define FA_H

#include <stdbool.h>
#include "arena.h"

typedef struct Node Node;
typedef struct Lexer Lexer;

typedef struct Transition {
  char fromValue;
  char toValue;
  Node *to;
} Transition;

typedef struct Node {
  bool isStart;
  bool isFinish;
  Transition **transitions;
} Node;

Node *reToNFA(Arena *, const char *re);
bool test(Node *nfa, const char *target);
void drawNFA(Node *nfa, Lexer *x);

#endif
