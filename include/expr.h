#ifndef EXPR_H_
#define EXPR_H_

#include "token.h"
#include <stdlib.h>

typedef enum { EXPR_BINARY, EXPR_GROUPING, EXPR_LITERAL, EXPR_UNARY } ExprType;
typedef enum { L_NUMBER, L_STR, L_FALSE, L_TRUE, L_NIL } LiteralType;

typedef struct Expr {
  ExprType type;
  union {
    struct {
      struct Expr *left;
      Token *operation;
      struct Expr *right;
    } binary;
    struct {
      struct Expr *expression;
    } grouping;
    struct {
      Token *operation;
      struct Expr *right;
    } unary;
    struct {
      Literal value;
      LiteralType type;
    } literal;
  } data;
} Expr;

static Expr *expr_new(ExprType type) {
  Expr *e = malloc(sizeof(Expr));

  e->type = type;
  return e;
}

static inline Expr *expr_new_binary(Token* op, Expr *left, Expr *right) {
  Expr *e = expr_new(EXPR_BINARY);

  e->data.binary.operation = op;
  e->data.binary.left = left;
  e->data.binary.right = right;
  return e;
}

static inline Expr *expr_new_unary(Token *op, Expr *right) {
  Expr *e = expr_new(EXPR_UNARY);

  e->data.unary.operation = op;
  e->data.unary.right = right;
  return e;
}

static inline Expr* expr_new_literal(Token* token) {

  Expr *e = expr_new(EXPR_LITERAL);

  if (token->type == NUMBER) e->data.literal.type = L_NUMBER;
  if (token->type == STRING) e->data.literal.type = L_STR;

  e->data.literal.value = token->literal;

  return e;
}

static inline Expr* expr_new_literal_nil(void) {
  Expr *e = expr_new(EXPR_LITERAL);
  e->data.literal.type = L_NIL;

  return e;
}

static inline Expr* expr_new_literal_false(void) {
  Expr *e = expr_new(EXPR_LITERAL);
  e->data.literal.type = L_FALSE;

  return e;
}

static inline Expr* expr_new_literal_true(void) {
  Expr *e = expr_new(EXPR_LITERAL);
  e->data.literal.type = L_TRUE;

  return e;
}

static inline Expr* expr_new_literal_number(double value) {
  Expr *e = expr_new(EXPR_LITERAL);
  e->data.literal.type = L_NUMBER;
  e->data.literal.value.float_val = value;

  return e;
}

static inline Expr *expr_new_literal_str(char* value) {
  Expr *e = expr_new(EXPR_LITERAL);

  e->data.literal.type = L_STR;
  e->data.literal.value.string_val = value;

  return e;
}

static inline Expr *expr_new_grouping(Expr *expr) {
  Expr *e = expr_new(EXPR_GROUPING);

  e->data.grouping.expression = expr;
  return e;
}

static inline void expr_print_ast(Expr *expr) {
  Expr node = *expr;
  switch (node.type) {
    case EXPR_BINARY:
      printf("(");
      printf("%s ", node.data.binary.operation->lexeme);
      expr_print_ast(node.data.binary.left);
      expr_print_ast(node.data.binary.right);
      printf(")");
      break;
    case EXPR_GROUPING:
      printf("(group ");
      expr_print_ast(node.data.grouping.expression);
      printf(")");
      break;
    case EXPR_LITERAL:
      if (node.data.literal.type == L_STR) {
        printf("%s", node.data.literal.value.string_val);
      } else if (node.data.literal.type == L_FALSE) {
        printf("FALSE");
      } else if (node.data.literal.type == L_TRUE) {
        printf("TRUE");
      } else if (node.data.literal.type == L_NUMBER) {
        printf("%.2f", node.data.literal.value.float_val);
      } else {
        printf("NIL");
      }
      printf(" ");
      break;
    case EXPR_UNARY:
      printf("(");
      printf("%s ", node.data.unary.operation->lexeme);
      expr_print_ast(node.data.unary.right);
      printf(") ");
      break;
  }
}

static inline void print_test(void) {
  Expr *expr = expr_new_binary(
    &(Token){.lexeme = "*", .type = STAR},
    expr_new_unary(
      &(Token){.lexeme = "-", .type = MINUS},
      expr_new_literal_number(123)
    ),
    expr_new_grouping(expr_new_literal_number(45.67))
  );

  expr_print_ast(expr);
  printf("\n");
  return;
}
#endif // EXPR_H_
