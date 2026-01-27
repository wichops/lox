#include <stdlib.h>
#include <stdio.h>

#ifndef TOKEN_H_
#define TOKEN_H_

typedef enum {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  EF // EOF alias because it is already defined in stdio
} TokenType;

typedef struct Token {
  TokenType type;
  char* lexeme;
  int line;
  union {
    float float_val;
    char* string_val;
  } literal;
} Token;


typedef struct TokenArray {
  Token* array;
  size_t index;
} TokenArray;


inline TokenArray* tokens_create(void) {
  Token* array = malloc(sizeof(Token) * 500);
  TokenArray tokens_array = (TokenArray) { array, 0 };
  TokenArray* tokens = &tokens_array;

  return tokens;
}

inline Token* tokens_get(TokenArray* tokens, size_t index) {
  if (index >= tokens->index) return NULL;
  return &tokens->array[index];
}

inline void tokens_add(TokenArray* tokens, Token element) {
  tokens->array[tokens->index++] = element;
}

#endif // TOKEN_H_
