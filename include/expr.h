#ifndef EXPR_H_
#define EXPR_H_

#include <stdlib.h>
#include "token.h"

typedef enum {
  EXPR_BINARY,
  EXPR_GROUPING,
  EXPR_LITERAL,
  EXPR_UNARY
} ExprType;

typedef struct Expr {
  ExprType type;
  union {
    struct { struct Expr *left; Token operation; struct Expr *right; } binary;
    struct { struct Expr *expression; } grouping;
    struct { Token *operation; struct Expr *right; } unary;
    struct { Literal value; } literal;
  } data;
} Expr;

static Expr* expr_new(ExprType type) {
  Expr* e = malloc(sizeof(Expr));
  e->type = type;

  return e;
}

static inline Expr* expr_new_binary(Token op, Expr* left, Expr* right) {
  Expr* e = expr_new(EXPR_BINARY);

  e->data.binary.operation = op;
  e->data.binary.left = left;
  e->data.binary.right = right;
}

static inline Expr* expr_new_unary(Token* op, Expr* right) {
  Expr* e = expr_new(EXPR_LITERAL);

  e->data.unary.operation = op;
  e->data.unary.right = right;
}

static inline Expr* expr_new_literal(Literal value) {
  Expr* e = expr_new(EXPR_LITERAL);

  e->data.literal.value = value;
}

static inline Expr* expr_new_grouping(Expr* expr) {
  Expr* e = expr_new(EXPR_GROUPING);

  e->data.grouping.expression = expr;
}

#endif // EXPR_H_
