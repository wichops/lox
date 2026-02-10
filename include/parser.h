#ifndef PARSER_H_
#define PARSER_H_

#include "expr.h"
#include "token.h"

typedef struct {
  TokenArray* tokens;
  size_t current;
} Parser;


void parser_init(Parser* p, TokenArray* tokens);
Expr* parser_parse(Parser* p);
#endif // PARSER_H_
