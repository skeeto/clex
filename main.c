#define assert(c) if (!(c)) *(volatile int *)0 = 0

#ifdef TEST_CLEX

#include "clex.h"
#include <string.h>
#include <stdlib.h>

typedef enum TokenKind {
  AUTO,
  BOOL,
  BREAK,
  CASE,
  CHAR,
  COMPLEX,
  CONST,
  CONTINUE,
  DEFAULT,
  DO,
  DOUBLE,
  ELSE,
  ENUM,
  EXTERN,
  FLOAT,
  FOR,
  GOTO,
  IF,
  IMAGINARY,
  INLINE,
  INT,
  LONG,
  REGISTER,
  RESTRICT,
  RETURN,
  SHORT,
  SIGNED,
  SIZEOF,
  STATIC,
  STRUCT,
  SWITCH,
  TYPEDEF,
  UNION,
  UNSIGNED,
  VOID,
  VOLATILE,
  WHILE,
  ELLIPSIS,
  RIGHT_ASSIGN,
  LEFT_ASSIGN,
  ADD_ASSIGN,
  SUB_ASSIGN,
  MUL_ASSIGN,
  DIV_ASSIGN,
  MOD_ASSIGN,
  AND_ASSIGN,
  XOR_ASSIGN,
  OR_ASSIGN,
  RIGHT_OP,
  LEFT_OP,
  INC_OP,
  DEC_OP,
  PTR_OP,
  AND_OP,
  OR_OP,
  LE_OP,
  GE_OP,
  EQ_OP,
  NE_OP,
  SEMICOL,
  OCURLYBRACE,
  CCURLYBRACE,
  COMMA,
  COLON,
  EQUAL,
  OPARAN,
  CPARAN,
  OSQUAREBRACE,
  CSQUAREBRACE,
  DOT,
  AMPER,
  EXCLAMATION,
  TILDE,
  MINUS,
  PLUS,
  STAR,
  SLASH,
  PERCENT,
  RANGLE,
  LANGLE,
  CARET,
  PIPE,
  QUESTION,
  STRINGLITERAL,
  CONSTANT,
  IDENTIFIER,
} TokenKind;

int main(void) {
  Clex c[1];
  size_t heapsize = 1<<20;
  char *heap = malloc(heapsize);

  initClex(c, heap, heapsize);

  registerKind(c, "auto", AUTO);
  registerKind(c, "_Bool", BOOL);
  registerKind(c, "break", BREAK);
  registerKind(c, "[a-zA-Z_]([a-zA-Z_]|[0-9])*", IDENTIFIER);
  registerKind(c, ";", SEMICOL);

  char input[] = "auto ident1; break;";
  beginClex(c, input, sizeof(input));

  Token token = clex(c);
  assert(token.kind == AUTO);
  assert(strcmp(token.lexeme, "auto") == 0);

  token = clex(c);
  assert(token.kind == IDENTIFIER);
  assert(strcmp(token.lexeme, "ident1") == 0);

  token = clex(c);
  assert(token.kind == SEMICOL);
  assert(strcmp(token.lexeme, ";") == 0);

  token = clex(c);
  assert(token.kind == BREAK);
  assert(strcmp(token.lexeme, "break") == 0);

  token = clex(c);
  assert(token.kind == SEMICOL);
  assert(strcmp(token.lexeme, ";") == 0);

  initClex(c, heap, heapsize);
  registerKind(c, "auto", AUTO);
  registerKind(c, "_Bool", BOOL);
  registerKind(c, "break", BREAK);
  registerKind(c, "case", CASE);
  registerKind(c, "char", CHAR);
  registerKind(c, "_Complex", COMPLEX);
  registerKind(c, "const", CONST);
  registerKind(c, "continue", CONTINUE);
  registerKind(c, "default", DEFAULT);
  registerKind(c, "do", DO);
  registerKind(c, "double", DOUBLE);
  registerKind(c, "else", ELSE);
  registerKind(c, "enum", ENUM);
  registerKind(c, "extern", EXTERN);
  registerKind(c, "float", FLOAT);
  registerKind(c, "for", FOR);
  registerKind(c, "goto", GOTO);
  registerKind(c, "if", IF);
  registerKind(c, "_Imaginary", IMAGINARY);
  registerKind(c, "inline", INLINE);
  registerKind(c, "int", INT);
  registerKind(c, "long", LONG);
  registerKind(c, "register", REGISTER);
  registerKind(c, "restrict", RESTRICT);
  registerKind(c, "return", RETURN);
  registerKind(c, "short", SHORT);
  registerKind(c, "signed", SIGNED);
  registerKind(c, "sizeof", SIZEOF);
  registerKind(c, "static", STATIC);
  registerKind(c, "struct", STRUCT);
  registerKind(c, "switch", SWITCH);
  registerKind(c, "typedef", TYPEDEF);
  registerKind(c, "union", UNION);
  registerKind(c, "unsigned", UNSIGNED);
  registerKind(c, "void", VOID);
  registerKind(c, "volatile", VOLATILE);
  registerKind(c, "while", WHILE);
  registerKind(c, "...", ELLIPSIS);
  registerKind(c, ">>=", RIGHT_ASSIGN);
  registerKind(c, "<<=", LEFT_ASSIGN);
  registerKind(c, "\\+=", ADD_ASSIGN);
  registerKind(c, "-=", SUB_ASSIGN);
  registerKind(c, "\\*=", MUL_ASSIGN);
  registerKind(c, "/=", DIV_ASSIGN);
  registerKind(c, "%=", MOD_ASSIGN);
  registerKind(c, "&=", AND_ASSIGN);
  registerKind(c, "^=", XOR_ASSIGN);
  registerKind(c, "\\|=", OR_ASSIGN);
  registerKind(c, ">>", RIGHT_OP);
  registerKind(c, "<<", LEFT_OP);
  registerKind(c, "\\+\\+", INC_OP);
  registerKind(c, "--", DEC_OP);
  registerKind(c, "->", PTR_OP);
  registerKind(c, "&&", AND_OP);
  registerKind(c, "\\|\\|", OR_OP);
  registerKind(c, "<=", LE_OP);
  registerKind(c, ">=", GE_OP);
  registerKind(c, "==", EQ_OP);
  registerKind(c, "!=", NE_OP);
  registerKind(c, ";", SEMICOL);
  registerKind(c, "{|<%", OCURLYBRACE);
  registerKind(c, "}|%>", CCURLYBRACE);
  registerKind(c, ",", COMMA);
  registerKind(c, ":", COLON);
  registerKind(c, "=", EQUAL);
  registerKind(c, "\\(", OPARAN);
  registerKind(c, "\\)", CPARAN);
  registerKind(c, "\\[|<:", OSQUAREBRACE);
  registerKind(c, "\\]|:>", CSQUAREBRACE);
  registerKind(c, ".", DOT);
  registerKind(c, "&", AMPER);
  registerKind(c, "!", EXCLAMATION);
  registerKind(c, "~", TILDE);
  registerKind(c, "-", MINUS);
  registerKind(c, "\\+", PLUS);
  registerKind(c, "\\*", STAR);
  registerKind(c, "/", SLASH);
  registerKind(c, "%", PERCENT);
  registerKind(c, "<", RANGLE);
  registerKind(c, ">", LANGLE);
  registerKind(c, "^", CARET);
  registerKind(c, "\\|", PIPE);
  registerKind(c, "\\?", QUESTION);
  registerKind(c, "L?\"[ -~]*\"", STRINGLITERAL);
  registerKind(c, "0[xX][a-fA-F0-9]+([uU])?([lL])?([lL])?", CONSTANT);
  registerKind(c, "0[0-7]*([uU])?([lL])?([lL])?", CONSTANT);
  registerKind(c, "[1-9][0-9]*([uU])?([lL])?([lL])?", CONSTANT);
  registerKind(c, "L?'[ -~]*'", CONSTANT);
  registerKind(c, "[0-9]+[Ee][+-]?[0-9]+[fFlL]", CONSTANT);
  registerKind(c, "[0-9]*.[0-9]+[Ee][+-]?[fFlL]", CONSTANT);
  registerKind(c, "[0-9]+.[0-9]*[Ee][+-]?[fFlL]", CONSTANT);
  registerKind(c, "0[xX][a-fA-F0-9]+[Pp][+-]?[0-9]+([fFlL])?", CONSTANT);
  registerKind(c, "0[xX][a-fA-F0-9]*.[a-fA-F0-9]+[Pp][+-]?[0-9]+([fFlL])?", CONSTANT);
  registerKind(c, "0[xX][a-fA-F0-9]+.[a-fA-F0-9]+[Pp][+-]?[0-9]+([fFlL])?", CONSTANT);
  registerKind(c, "[a-zA-Z_]([a-zA-Z_]|[0-9])*", IDENTIFIER);
  // TODO: Add comment // and /* */
  // TODO: Add #

  /*
  FILE *f = fopen("./fa.c", "r");
  fseek(f, 0L, SEEK_END);
  size_t fsize = ftell(f);
  rewind(f);

  char *fcontent = malloc(fsize + 1);
  fread(fcontent, fsize, 1, f);
  fcontent[fsize] = '\0';
  fclose(f);
  */

  char input2[] = "int main(int argc, char *argv[]) {\nreturn 23;\n}";
  beginClex(c, input2, sizeof(input2));

  token = clex(c);
  assert(token.kind == INT);
  assert(strcmp(token.lexeme, "int") == 0);

  token = clex(c);
  assert(token.kind == IDENTIFIER);
  assert(strcmp(token.lexeme, "main") == 0);

  token = clex(c);
  assert(token.kind == OPARAN);
  assert(strcmp(token.lexeme, "(") == 0);

  token = clex(c);
  assert(token.kind == INT);
  assert(strcmp(token.lexeme, "int") == 0);

  token = clex(c);
  assert(token.kind == IDENTIFIER);
  assert(strcmp(token.lexeme, "argc") == 0);

  token = clex(c);
  assert(token.kind == COMMA);
  assert(strcmp(token.lexeme, ",") == 0);

  token = clex(c);
  assert(token.kind == CHAR);
  assert(strcmp(token.lexeme, "char") == 0);

  token = clex(c);
  assert(token.kind == STAR);
  assert(strcmp(token.lexeme, "*") == 0);

  token = clex(c);
  assert(token.kind == IDENTIFIER);
  assert(strcmp(token.lexeme, "argv") == 0);

  token = clex(c);
  assert(token.kind == OSQUAREBRACE);
  assert(strcmp(token.lexeme, "[") == 0);

  token = clex(c);
  assert(token.kind == CSQUAREBRACE);
  assert(strcmp(token.lexeme, "]") == 0);

  token = clex(c);
  assert(token.kind == CPARAN);
  assert(strcmp(token.lexeme, ")") == 0);

  token = clex(c);
  assert(token.kind == OCURLYBRACE);
  assert(strcmp(token.lexeme, "{") == 0);

  token = clex(c);
  assert(token.kind == RETURN);
  assert(strcmp(token.lexeme, "return") == 0);

  token = clex(c);
  assert(token.kind == CONSTANT);
  assert(strcmp(token.lexeme, "23") == 0);

  token = clex(c);
  assert(token.kind == SEMICOL);
  assert(strcmp(token.lexeme, ";") == 0);

  token = clex(c);
  assert(token.kind == CCURLYBRACE);
  assert(strcmp(token.lexeme, "}") == 0);

  free(heap);
}
#endif

#ifdef TEST_REGEX

#include "fa.h"
#include <stdlib.h>

int main(void) {
  Arena a[1] = {{malloc(1<<20), 1<<20, 0}};

  Node *nfa = reToNFA(a, "a");
  assert(test(nfa, "a") == true);
  assert(test(nfa, "b") == false);

  nfa = reToNFA(a, "ab");
  assert(test(nfa, "ab") == true);
  assert(test(nfa, "a") == false);
  assert(test(nfa, "b") == false);

  nfa = reToNFA(a, "ab|c");
  assert(test(nfa, "ab") == true);
  assert(test(nfa, "c") == true);
  assert(test(nfa, "abc") == false);

  nfa = reToNFA(a, "ab*c");
  assert(test(nfa, "c") == true);
  assert(test(nfa, "abc") == true);
  assert(test(nfa, "ababc") == true);
  assert(test(nfa, "ab") == false);
  assert(test(nfa, "abd") == false);
  assert(test(nfa, "acc") == false);

  nfa = reToNFA(a, "ab+c");
  assert(test(nfa, "c") == false);
  assert(test(nfa, "abc") == true);
  assert(test(nfa, "ababc") == true);
  assert(test(nfa, "ab") == false);
  assert(test(nfa, "abd") == false);
  assert(test(nfa, "acc") == false);

  nfa = reToNFA(a, "ab?c");
  assert(test(nfa, "c") == true);
  assert(test(nfa, "abc") == true);
  assert(test(nfa, "ababc") == false);
  assert(test(nfa, "ab") == false);
  assert(test(nfa, "abd") == false);
  assert(test(nfa, "acc") == false);

  nfa = reToNFA(a, "[ab]c");
  assert(test(nfa, "c") == false);
  assert(test(nfa, "ac") == true);
  assert(test(nfa, "bc") == true);
  assert(test(nfa, "abc") == false);
  assert(test(nfa, "bd") == false);
  assert(test(nfa, "acc") == false);

  nfa = reToNFA(a, "[A-Za-z]c");
  assert(test(nfa, "c") == false);
  assert(test(nfa, "ac") == true);
  assert(test(nfa, "bc") == true);
  assert(test(nfa, "Ac") == true);
  assert(test(nfa, "Zd") == false);
  assert(test(nfa, "Zc") == true);

  nfa = reToNFA(a, "[A-Za-z]*c");
  assert(test(nfa, "AZazc") == true);
  assert(test(nfa, "AZaz") == false);

  nfa = reToNFA(a, "[A-Za-z]?c");
  assert(test(nfa, "Ac") == true);
  assert(test(nfa, "c") == true);
  assert(test(nfa, "A") == false);

  nfa = reToNFA(a, "a(bc|de)f");
  assert(test(nfa, "abcf") == true);
  assert(test(nfa, "adef") == true);
  assert(test(nfa, "af") == false);
  assert(test(nfa, "abf") == false);
  assert(test(nfa, "abcdef") == false);
  assert(test(nfa, "abccf") == false);
  assert(test(nfa, "bcf") == false);
  assert(test(nfa, "abc") == false);

  nfa = reToNFA(a, "(bc|de)f");
  assert(test(nfa, "bcf") == true);
  assert(test(nfa, "def") == true);

  nfa = reToNFA(a, "a(bc)*f");
  assert(test(nfa, "af") == true);
  assert(test(nfa, "abcf") == true);
  assert(test(nfa, "abcbcf") == true);
  assert(test(nfa, "abcbf") == false);

  nfa = reToNFA(a, "(bc)*f");
  assert(test(nfa, "f") == true);
  assert(test(nfa, "bcf") == true);
  assert(test(nfa, "bcbcf") == true);
  assert(test(nfa, "bcbf") == false);
  assert(test(nfa, "bc") == false);

  nfa = reToNFA(a, "a(bc|de)*f");
  assert(test(nfa, "af") == true);
  assert(test(nfa, "abcf") == true);
  assert(test(nfa, "adef") == true);
  assert(test(nfa, "abcbcf") == true);
  assert(test(nfa, "adedef") == true);
  assert(test(nfa, "abcdef") == true);
  assert(test(nfa, "abf") == false);
  assert(test(nfa, "abccf") == false);
  assert(test(nfa, "bcf") == false);
  assert(test(nfa, "abc") == false);

  nfa = reToNFA(a, "a(bc|de)+f");
  assert(test(nfa, "af") == false);
  assert(test(nfa, "abcf") == true);
  assert(test(nfa, "adef") == true);
  assert(test(nfa, "abcbcf") == true);
  assert(test(nfa, "adedef") == true);
  assert(test(nfa, "abcdef") == true);
  assert(test(nfa, "abf") == false);
  assert(test(nfa, "abccf") == false);
  assert(test(nfa, "bcf") == false);
  assert(test(nfa, "abc") == false);

  nfa = reToNFA(a, "a(bc|de)?f");
  assert(test(nfa, "af") == true);
  assert(test(nfa, "abcf") == true);
  assert(test(nfa, "adef") == true);
  assert(test(nfa, "abcbcf") == false);
  assert(test(nfa, "adedef") == false);
  assert(test(nfa, "abcdef") == false);
  assert(test(nfa, "abf") == false);
  assert(test(nfa, "abccf") == false);
  assert(test(nfa, "bcf") == false);
  assert(test(nfa, "abc") == false);

  nfa = reToNFA(a, "([a-zA-Z_])*");
  assert(test(nfa, "valid") == true);
  assert(test(nfa, "Valid") == true);
  assert(test(nfa, "_var1") == false);
  assert(test(nfa, "vv1") == false);
  assert(test(nfa, "v1") == false);

  nfa = reToNFA(a, "([a-zA-Z_]|[0-9])*");
  assert(test(nfa, "valid") == true);
  assert(test(nfa, "Valid") == true);
  assert(test(nfa, "_var1") == true);
  assert(test(nfa, "vv1") == true);
  assert(test(nfa, "v1") == true);

  nfa = reToNFA(a, "[a-zA-Z_]([a-zA-Z_]|[0-9])*");
  assert(test(nfa, "valid") == true);
  assert(test(nfa, "Valid") == true);
  assert(test(nfa, "_var1") == true);
  assert(test(nfa, "vv1") == true);
  assert(test(nfa, "v1") == true);

  nfa = reToNFA(a, "\\|");
  assert(test(nfa, "|") == true);

  nfa = reToNFA(a, "\\[");
  assert(test(nfa, "[") == true);

  nfa = reToNFA(a, "\\]");
  assert(test(nfa, "]") == true);

  nfa = reToNFA(a, "\\+");
  assert(test(nfa, "+") == true);

  nfa = reToNFA(a, "\\*");
  assert(test(nfa, "*") == true);

  nfa = reToNFA(a, "\\?");
  assert(test(nfa, "?") == true);

  nfa = reToNFA(a, "(u)(a)");
  assert(test(nfa, "ua") == true);

  nfa = reToNFA(a, "[1-9][0-9]*([uU])?([lL])?([lL])?");
  assert(test(nfa, "23") == true);
  assert(test(nfa, "23u") == true);
  assert(test(nfa, "23UlL") == true);

  free(a->buf);
}
#endif
