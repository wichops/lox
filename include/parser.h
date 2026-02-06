#ifndef PARSER_H_
#define PARSER_H_

#include "token.h"

typedef struct {
  TokenArray* Token;
  size_t current;
} ParserList;


#endif // PARSER_H_
