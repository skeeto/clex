#include "fa.h"
#include "arena.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#undef EOF

typedef struct Lexer {
    Arena *arena;
    const char *lexerContent;
    size_t lexerPosition;
    Node *lastBeforeParanEntry;
    Node *beforeParanEntry;
    Node *paranEntry;
    bool inPipe;
    bool pipeSeen;
    bool inBackslash;
    char **drawSeen;
    char **getFinishNodeSeen;
} Lexer;

Node *makeNode(Lexer *x, bool isStart, bool isFinish) {
  Node *result = alloc(x->arena, sizeof(Node));
  result->isStart = isStart;
  result->isFinish = isFinish;
  result->transitions = alloc(x->arena, 100*sizeof(Transition *));
  return result;
}

Transition *makeTransition(Lexer *x, char fromValue, char toValue, Node *to) {
  Transition *result = alloc(x->arena, sizeof(Transition));
  result->fromValue = fromValue;
  result->toValue = toValue;
  result->to = to;
  return result;
}

typedef enum TokenKind {
  OPARAN,
  CPARAN,
  OSBRACKET,
  CSBRACKET,
  DASH,
  PIPE,
  STAR,
  PLUS,
  QUESTION,
  BSLASH,
  LITERAL,
  EOF,
} TokenKind;

typedef struct Token {
  TokenKind kind;
  char lexeme;
} Token;

void initLexer(Lexer *x, Arena *a, const char *content) {
  x->arena = a;
  x->lexerContent = content;
  x->lexerPosition = 0;
  x->lastBeforeParanEntry = NULL;
  x->beforeParanEntry = NULL;
  x->paranEntry = NULL;
  x->inPipe = false;
  x->pipeSeen = false;
  x->inBackslash = false;
  x->drawSeen = NULL;
  x->getFinishNodeSeen = NULL;
}

Token *makeToken(Lexer *x, TokenKind kind) {
  Token *result = alloc(x->arena, sizeof(Token));
  result->kind = kind;
  return result;
}

Token *makeLexemeToken(Lexer *x, TokenKind kind, char lexeme) {
  Token *result = alloc(x->arena, sizeof(Token));
  result->kind = kind;
  result->lexeme = lexeme;
  return result;
}

Token *lex(Lexer *x) {
  switch (x->lexerContent[x->lexerPosition]) {
    case '\0':
      return makeToken(x, EOF);
    case '(':
      x->lexerPosition++;
      return makeLexemeToken(x, OPARAN, '(');
    case ')':
      x->lexerPosition++;
      return makeLexemeToken(x, CPARAN, ')');
    case '[':
      x->lexerPosition++;
      return makeLexemeToken(x, OSBRACKET, '[');
    case ']':
      x->lexerPosition++;
      return makeLexemeToken(x, CSBRACKET, ']');
    case '-':
      x->lexerPosition++;
      return makeLexemeToken(x, DASH, '-');
    case '|':
      x->lexerPosition++;
      return makeLexemeToken(x, PIPE, '|');
    case '*':
      x->lexerPosition++;
      return makeLexemeToken(x, STAR, '*');
    case '+':
      x->lexerPosition++;
      return makeLexemeToken(x, PLUS, '+');
    case '?':
      x->lexerPosition++;
      return makeLexemeToken(x, QUESTION, '?');
    case '\\':
      x->lexerPosition++;
      return makeLexemeToken(x, BSLASH, '\\');
  }
  Token *result = makeLexemeToken(x, LITERAL, x->lexerContent[x->lexerPosition]);
  x->lexerPosition++;
  return result;
}

Token *peek(Lexer *x) {
  Token *lexed = lex(x);
  if (lexed->kind != EOF)
    x->lexerPosition--;
  return lexed;
}

bool inArray(char **array, char *key) {
  for (int i = 0; i < 1024; i++)
    if (array[i] && strcmp(array[i], key) == 0)
      return true;
  return false;
}

void insertArray(char **array, char *key) {
  for (int i = 0; i < 1024; i++)
    if (!array[i]) {
      array[i] = key;
      return;
    }
}

char *drawKey(Lexer *x, Node *node1, Node *node2, char fromValue, char toValue) {
  char *result = alloc(x->arena, 1024);
  sprintf(result, "%p%p%c%c", node1, node2, fromValue, toValue);
  return result;
}

char *getFinishNodeKey(Lexer *x, Node *node) {
  char *result = alloc(x->arena, 1024);
  sprintf(result, "%p", node);
  return result;
}

void drawNFA(Node *nfa, Lexer *x) {
  if (!x->drawSeen)
    x->drawSeen = alloc(x->arena, 1024*sizeof(char *));
  for (int i = 0; i < 100; i++)
    if (nfa->transitions[i]) {
      printf("%p -> %p [label=\"%c-%c\"];\n", nfa, nfa->transitions[i]->to, nfa->transitions[i]->fromValue, nfa->transitions[i]->toValue);
      if (!inArray(x->drawSeen, drawKey(x, nfa, nfa->transitions[i]->to, nfa->transitions[i]->fromValue, nfa->transitions[i]->toValue))) {
        insertArray(x->drawSeen, drawKey(x, nfa, nfa->transitions[i]->to, nfa->transitions[i]->fromValue, nfa->transitions[i]->toValue));
        drawNFA(nfa->transitions[i]->to, x);
      }
    }
}

Node *getFinishNode(Node *node, Lexer *x) {
  if (!x->getFinishNodeSeen)
    x->getFinishNodeSeen = alloc(x->arena, 1024*sizeof(char *));
  if (node->isFinish)
    return node;
  for (int i = 0; i < 100; i++)
    if (node->transitions[i] != NULL)
      if (!inArray(x->getFinishNodeSeen, getFinishNodeKey(x, node->transitions[i]->to))) {
        insertArray(x->getFinishNodeSeen, getFinishNodeKey(x, node->transitions[i]->to));
        return getFinishNode(node->transitions[i]->to, x);
      }
  return NULL;
}

Node *runNFA(Lexer *x) {
  Token *token;
  Node *entry = makeNode(x, true, true);
  Node *last = entry;
  while ((token = lex(x))->kind != EOF) {
    x->getFinishNodeSeen = NULL;
    if (x->inBackslash) {
      x->inBackslash = false;
      Node *node = makeNode(x, false, true);
      last->transitions[0] = makeTransition(x, token->lexeme, token->lexeme, node);
      last->isFinish = false;
      last = node;
      continue;
    }
    if (peek(x)->kind == OPARAN) {
      x->lastBeforeParanEntry = x->beforeParanEntry;
      x->beforeParanEntry = last;
    }
    if (token->kind == BSLASH) {
      x->inBackslash = true;
    }
    if (token->kind == OPARAN) {
      x->paranEntry = last;
    }
    if (token->kind == CPARAN) {
      if (x->inPipe) {
        x->inPipe = false;
        x->pipeSeen = true;
        return entry;
      }
    }
    if (token->kind == LITERAL) {
      Node *node = makeNode(x, false, true);
      last->transitions[0] = makeTransition(x, token->lexeme, token->lexeme, node);
      last->isFinish = false;
      last = node;
    }
    if (token->kind == PIPE) {
      x->inPipe = true;
      if (!x->paranEntry) {
        Node *pastEntry = entry;
        pastEntry->isStart = false;

        entry = makeNode(x, true, false);

        entry->transitions[0] = makeTransition(x, '\0', '\0', pastEntry);
        Node *firstFinish = getFinishNode(pastEntry, x);
        firstFinish->isFinish = false;

        Node *second = runNFA(x);
        Node *secondFinish = getFinishNode(second, x);
        secondFinish->isFinish = false;
        entry->transitions[1] = makeTransition(x, '\0', '\0', second);

        Node *finish = makeNode(x, false, true);
        firstFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);
        secondFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);

        last = finish;
      } else {
        Node *pipeEntry = makeNode(x, x->beforeParanEntry ? false : true, false);
        if (x->lastBeforeParanEntry) {
          x->lastBeforeParanEntry->transitions[0] = makeTransition(x, x->lastBeforeParanEntry->transitions[0]->fromValue, x->lastBeforeParanEntry->transitions[0]->toValue, pipeEntry);
          x->lastBeforeParanEntry = NULL;
        }
        else if (x->beforeParanEntry) {
          x->beforeParanEntry->transitions[0] = makeTransition(x, x->beforeParanEntry->transitions[0]->fromValue, x->beforeParanEntry->transitions[0]->toValue, pipeEntry);
          x->beforeParanEntry = pipeEntry;
        } else {
          entry = pipeEntry;
          x->beforeParanEntry = entry;
        }
        pipeEntry->transitions[0] = makeTransition(x, '\0', '\0', x->paranEntry);

        Node *firstFinish = getFinishNode(x->paranEntry, x);
        firstFinish->isFinish = false;

        Node *second = runNFA(x);
        Node *secondFinish = getFinishNode(second, x);
        secondFinish->isFinish = false;
        pipeEntry->transitions[1] = makeTransition(x, '\0', '\0', second);

        Node *finish = makeNode(x, false, true);
        firstFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);
        secondFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);

        last = finish;
      }
    }
    if (token->kind == STAR) {
      if (!x->paranEntry) {
        Node *pastEntry = entry;
        pastEntry->isStart = false;

        Node *finish = makeNode(x, false, true);
        entry = makeNode(x, true, false);

        entry->transitions[0] = makeTransition(x, '\0', '\0', pastEntry);
        entry->transitions[1] = makeTransition(x, '\0', '\0', finish);
        Node *firstFinish = getFinishNode(pastEntry, x);
        firstFinish->isFinish = false;
        firstFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);
        firstFinish->transitions[1] = makeTransition(x, '\0', '\0', pastEntry);

        last = finish;
      } else {
        Node *starEntry = makeNode(x, x->beforeParanEntry ? false : true, false);
        if (x->lastBeforeParanEntry) {
          x->lastBeforeParanEntry->transitions[0] = makeTransition(x, x->lastBeforeParanEntry->transitions[0]->fromValue, x->lastBeforeParanEntry->transitions[0]->toValue, starEntry);
          x->lastBeforeParanEntry = NULL;
        }
        else if (x->beforeParanEntry)
          x->beforeParanEntry->transitions[0] = makeTransition(x, x->beforeParanEntry->transitions[0]->fromValue, x->beforeParanEntry->transitions[0]->toValue, starEntry);
        else
          entry = starEntry;

        Node *finish = makeNode(x, false, true);

        starEntry->transitions[0] = makeTransition(x, '\0', '\0', x->paranEntry);
        starEntry->transitions[1] = makeTransition(x, '\0', '\0', finish);
        Node *firstFinish = getFinishNode(x->paranEntry, x);
        firstFinish->isFinish = false;
        firstFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);
        firstFinish->transitions[1] = makeTransition(x, '\0', '\0', x->beforeParanEntry && x->pipeSeen ? x->beforeParanEntry : starEntry);

        last = finish;
      }
    }
    if (token->kind == PLUS) {
      Node *finish = getFinishNode(entry, x);
      finish->transitions[1] = makeTransition(x, '\0', '\0', x->beforeParanEntry ? x->beforeParanEntry : entry);
    }
    if (token->kind == QUESTION) {
      if (!x->paranEntry) {
        Node *pastEntry = entry;
        pastEntry->isStart = false;

        entry = makeNode(x, true, false);

        entry->transitions[0] = makeTransition(x, '\0', '\0', pastEntry);
        Node *firstFinish = getFinishNode(pastEntry, x);
        firstFinish->isFinish = false;

        Node *finish = makeNode(x, false, true);
        firstFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);
        entry->transitions[1] = makeTransition(x, '\0', '\0', finish);

        last = finish;
      } else {
        Node *questionEntry = makeNode(x, x->beforeParanEntry ? false : true, false);
        if (x->lastBeforeParanEntry) {
          x->lastBeforeParanEntry->transitions[0] = makeTransition(x, x->lastBeforeParanEntry->transitions[0]->fromValue, x->lastBeforeParanEntry->transitions[0]->toValue, questionEntry);
          x->lastBeforeParanEntry = NULL;
        }
        else if (x->beforeParanEntry)
          x->beforeParanEntry->transitions[0] = makeTransition(x, x->beforeParanEntry->transitions[0]->fromValue, x->beforeParanEntry->transitions[0]->toValue, questionEntry);
        else
          entry = questionEntry;

        questionEntry->transitions[0] = makeTransition(x, '\0', '\0', x->paranEntry);
        Node *firstFinish = getFinishNode(x->paranEntry, x);
        firstFinish->isFinish = false;

        Node *finish = makeNode(x, false, true);
        firstFinish->transitions[0] = makeTransition(x, '\0', '\0', finish);
        questionEntry->transitions[1] = makeTransition(x, '\0', '\0', firstFinish);

        last = finish;
      }
    }
    if (token->kind == OSBRACKET) {
      int index = 0;
      Node *node = makeNode(x, false, true);
      while (peek(x)->kind != CSBRACKET) {
        char fromValue = lex(x)->lexeme;
        if (peek(x)->kind == DASH) {
          lex(x);
          char toValue = lex(x)->lexeme;
          last->transitions[index++] = makeTransition(x, fromValue, toValue, node);
        } else {
          last->transitions[index++] = makeTransition(x, fromValue, fromValue, node);
        }
      }
      lex(x);
      last->isFinish = false;
      last = node;
    }
  }
  return entry;
}

Node *reToNFA(Arena *a, const char *re)
{
    Lexer x[1];
    initLexer(x, a, re);
    return runNFA(x);
}

bool test(Node *nfa, const char *target) {
  for (size_t i = 0; i < strlen(target); i++) {
    for (int j = 0; j < 100; j++)
      if (nfa->transitions[j]) {
        if (nfa->transitions[j]->fromValue <= target[i] && nfa->transitions[j]->toValue >= target[i]) {
          if (test(nfa->transitions[j]->to, target + i + 1))
            return true;
        } else if (nfa->transitions[j]->fromValue == '\0') {
          if (test(nfa->transitions[j]->to, target + i))
            return true;
        }
      }
    return false;
  }
  for (int j = 0; j < 100; j++) {
    if (nfa->transitions[j] && nfa->transitions[j]->fromValue == '\0')
      if (test(nfa->transitions[j]->to, ""))
        return true;
  }
  if (nfa->isFinish)
    return true;
  return false;
}

