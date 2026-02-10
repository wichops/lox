#include "parser.h"
#include "expr.h"

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

// Forward declarations
static Expr* expression(Parser* p);
static Expr* equality(Parser* p);
static Expr* comparison(Parser* p);
static Expr* term(Parser* p);
static Expr* factor(Parser* p);
static Expr* unary(Parser* p);
static Expr* primary(Parser* p);

// Helpers
static Token* peek(Parser* p) {
  return tokens_get(p->tokens, p->current);
}

static Token* previous(Parser* p) {
  return tokens_get(p->tokens, p->current - 1);
}

static int is_at_end(Parser* p) {
  Token* t = peek(p);

  if (t != NULL) {
    return t->type == EF;
  }

  return 0;
}
static int check(Parser* p,  TokenType type) {
  if (is_at_end(p)) return 0;
  Token* t = peek(p);

  return t->type == type;
}

static Token* advance(Parser* p) {
  if (!is_at_end(p)) p->current++;
  return previous(p);
}

static int match(Parser* p,  TokenType type) {
  if (check(p, type)) {
    advance(p);
    return 1;
  }
  return 0;
}

static Token* consume(Parser* p, TokenType type) {
  if (check(p, type)) return advance(p);

  // TODO: IMPROVE PARSER ERROR HANDLING.
  printf("ERROR PARSING.");
  exit(EXIT_FAILURE);
}

static Expr* primary(Parser* p) {
  if (match(p, FALSE)) return expr_new_literal_false();
  if (match(p, TRUE)) return expr_new_literal_true();
  if (match(p, NIL)) return expr_new_literal_nil();

  if (match(p, NUMBER) || match(p, STRING)) {
    Token *t = previous(p);
    return expr_new_literal(t);
  }

  if (match(p, LEFT_PAREN)) {
   Expr* expr = expression(p);
   consume(p, RIGHT_PAREN);
   Expr* group_expr = expr_new_grouping(expr);

   return group_expr;
  }

  exit(EXIT_FAILURE);
  return NULL;
}

static Expr* unary(Parser* p) {
  if(match(p, BANG) || match(p, MINUS)) {
    Token* operator = previous(p);
    Expr* right = unary(p);
    return expr_new_unary(operator, right);
  }

  return primary(p);
}

static Expr* factor(Parser* p) {
  Expr* expr = unary(p);

  while(match(p, SLASH) || match(p, STAR)) {
    Token* operator = previous(p);
    Expr* right = unary(p);
    expr = expr_new_binary(operator, expr, right);
  }
  return expr;
}

static Expr* term(Parser* p) {
  Expr* expr = factor(p);

  while(match(p, MINUS) || match(p, PLUS)) {
    Token* operator = previous(p);
    Expr* right = factor(p);
    expr = expr_new_binary(operator, expr, right);
  }
  return expr;
}

static Expr* comparison(Parser* p) {
  Expr* expr = term(p);

  while (match(p, GREATER) || match(p, GREATER_EQUAL) || match(p, LESS) || match(p, LESS_EQUAL)) {
    Token* operator = previous(p);
    Expr* right = term(p);
    expr = expr_new_binary(operator, expr, right);
  }
  return expr;
}

static Expr* equality(Parser* p) {
  Expr* expr = comparison(p);
  while (match(p, BANG_EQUAL) || match(p, EQUAL_EQUAL)) {
    Token* operator = previous(p);
    Expr* right = comparison(p);
    expr = expr_new_binary(operator, expr, right);
  }
  return expr;
}

static Expr* expression(Parser *p) {
  return equality(p);
}

// Public API
void parser_init(Parser* p, TokenArray* tokens) {
  p->tokens = tokens;
  p->current = 0;
}

Expr* parser_parse(Parser* p) {
  return expression(p);
}
