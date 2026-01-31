#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

static char* token_strings[]  = {
  // Single-character tokens.
  "LEFT_PAREN",
  "RIGHT_PAREN",
  "LEFT_BRACE",
  "RIGHT_BRACE",
  "COMMA",
  "DOT",
  "MINUS",
  "PLUS",
  "SEMICOLON",
  "SLASH",
  "STAR",

  // One or two character tokens.
  "BANG",
  "BANG_EQUAL",
  "EQUAL",
  "EQUAL_EQUAL",
  "GREATER",
  "GREATER_EQUAL",
  "LESS",
  "LESS_EQUAL",

  // Literals.
  "IDENTIFIER",
  "STRING",
  "NUMBER",

  // Keywords.
  "AND",
  "CLASS",
  "ELSE",
  "FALSE",
  "FUN",
  "FOR",
  "IF",
  "NIL",
  "OR",
  "PRINT",
  "RETURN",
  "SUPER",
  "THIS",
  "TRUE",
  "VAR",
  "WHILE",

  "EOF"
};

typedef struct {
  char* word;
  TokenType type;
} Keyword;

static Keyword keywords[] = {
  {"and",    AND},
  {"class",  CLASS},
  {"else",   ELSE},
  {"false",  FALSE},
  {"for",    FOR},
  {"fun",    FUN},
  {"if",     IF},
  {"nil",    NIL},
  {"or",     OR},
  {"print",  PRINT},
  {"return", RETURN},
  {"super",  SUPER},
  {"this",   THIS},
  {"true",   TRUE},
  {"var",    VAR},
  {"while",  WHILE},
  {NULL, 0}
};

typedef union {
  float float_val;
  char *string_val;
} literal;

typedef struct Token {
  TokenType type;
  char* lexeme;
  int line;
  literal literal;
  int has_literal;
} Token;

typedef struct TokenArray {
  Token* array;
  size_t index;
} TokenArray;

static inline Token* tokens_get(TokenArray* tokens, size_t index) {
  if (index >= tokens->index) return NULL;
  return &tokens->array[index];
}

static inline void tokens_add(TokenArray* tokens, Token element) {
  tokens->array[tokens->index++] = element;
}

static inline int tokens_lookup(const char* word) {
  for (int i = 0; keywords[i].word; i++) {
    if (strcmp(keywords[i].word, word) == 0) {
      return keywords[i].type;
    }
  }
  return -1;
}

static inline char* token_str(TokenType type) {
  printf("type: %d\n", type);
  return token_strings[type];
}

#endif // TOKEN_H_
