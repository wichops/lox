#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "scanner.h"
#include "token.h"
#include "error.h"


static char advance(Scanner *s) {
  return s->source[s->current++];
}

static int is_at_end(Scanner *s) {
  return s->source[s->current] == '\0';
}

static int match(Scanner *s, char expected) {
  if (is_at_end(s)) return 0;
  if (s->source[s->current] != expected) return 0;

  s->current++;
  return 1;
}

static char peek(Scanner *s) {
  if (is_at_end(s)) return '\0';
  return s->source[s->current];
}

static char peek_next(Scanner *s) {
  if (s->current + 1 >= s->len) return '\0';
  return s->source[s->current + 1];
}

static void add_literal_token(Scanner* s, TokenType type, Literal literal, TokenArray* tokens) {
  printf("[DEBUG] add_literal_token()\n");
  size_t length = s->current - s->start;
  char* lexeme = malloc(length + 1);

  printf("[DEBUG] start: %lu. current: %lu\n", s->start, s->current);
  strncpy(lexeme, s->source + s->start, length);
  lexeme[length] = '\0';

  printf("[DEBUG]literal %s\n", lexeme);
  Token t = {0};
  t.lexeme = lexeme;
  t.type = type;
  t.line = s->line;
  t.literal = literal;
  t.has_literal = 1;

  tokens_add(tokens, t);
}

static void add_token(Scanner* s, TokenType type, TokenArray* tokens) {
  printf("[DEBUG] add_token()\n");
  size_t size = s->current - s->start;
  char* lexeme = malloc(size+1);
  printf("[DEBUG] start: %lu. current: %lu\n", s->start, s->current);
  strncpy(lexeme, s->source + s->start, size);
  lexeme[size] = '\0';


  Token t = {0};
  t.lexeme = lexeme;
  t.type = type;
  t.line = s->line;

  tokens_add(tokens, t);
}

static int is_digit(char c) {
  return c >= '0' && c <= '9';
}

static int is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int is_alphanumeric(char c) {
  return is_digit(c) || is_alpha(c);
 }

static void string(Scanner *s, TokenArray* tokens) {
  while (peek(s) != '"' && !is_at_end(s)) {
    if (peek(s) == '\n') s->line++;
    advance(s);
  }

  if (is_at_end(s)) {
    error(s->line, "Unterminated string");
    return;
  }

  advance(s);

  Literal l;
  int size = s->current - s->start - 2;
  char* slice = malloc(size + 1);
  strncpy(slice, &s->source[s->start + 1], size);
  slice[size] = '\0';
  l.string_val = slice;

  printf("[DEBUG] string %s\n", slice);
  add_literal_token(s, STRING, l, tokens);
}

static void number(Scanner *s, TokenArray* tokens) {
  while (is_digit(peek(s))) advance(s);

  if (peek(s) == '.' && is_digit(peek_next(s))) {
    advance(s);
    while (is_digit(peek(s))) advance(s);
  }

  Literal l;
  float f = strtof(&s->source[s->start], NULL);
  if (errno != 0) {
    error(s->line, "Error parsing number token");
    exit(EX_DATAERR);
  }
  l.float_val = f;

  add_literal_token(s, NUMBER, l, tokens);
}

void identifier(Scanner* s, TokenArray* tokens) {
  while(is_alphanumeric(peek(s))) advance(s);

  TokenType type = tokens_lookup(&s->source[s->start]);

  if ((int)type == -1) {
    type = IDENTIFIER;
  }
  add_token(s, type, tokens);
}

void scanner_init(Scanner* s, const char* source) {
  s->len = strlen(source);
  s->source = source;
  s->current = 0;
  s->start = 0;
  s->line = 1;
}

void scanner_scan_tokens(Scanner *s, TokenArray* tokens) {
  printf("[DEBUG] scan_tokens()\n");
  while (!is_at_end(s)) {
    s->start = s->current;
    char c = advance(s);
    printf("[DEBUG] char: %c start: %lu end: %lu \n", c, s->start, s->current);
    switch (c) {
      case '(': add_token(s, LEFT_PAREN, tokens); break;
      case ')': add_token(s, RIGHT_PAREN, tokens); break;
      case '{': add_token(s, LEFT_BRACE, tokens); break;
      case '}': add_token(s, RIGHT_BRACE, tokens); break;
      case ',': add_token(s, COMMA, tokens); break;
      case '.': add_token(s, DOT, tokens); break;
      case '-': add_token(s, MINUS, tokens); break;
      case '+': add_token(s, PLUS, tokens); break;
      case ';': add_token(s, SEMICOLON, tokens); break;
      case '*': add_token(s, STAR, tokens); break;
      case '/':
        if (match(s, '/')) {
          while (peek(s) != '\n' && !is_at_end(s)) {
            advance(s);
          }
        } else {
          add_token(s, SLASH, tokens);
        }
        break;
      case '!':
        add_token(s, match(s, '=') ? BANG_EQUAL : BANG, tokens);
        break;
      case '=':
        add_token(s, match(s, '=') ? EQUAL_EQUAL : EQUAL, tokens);
        break;
      case '<':
        add_token(s, match(s, '=') ? LESS_EQUAL : LESS, tokens);
        break;
      case '>':
        add_token(s, match(s, '=') ? GREATER_EQUAL : GREATER, tokens);
        break;
      case ' ':
      case '\r':
      case '\t':
        break;
      case '\n': s->line++; break;
      case '"': string(s, tokens); break;
      default:
        if (is_digit(c)) {
          number(s, tokens);
        } else if (is_alpha(c)) {
          identifier(s, tokens);
        } else {
          error(s->line, "Unexpected character.");
        }
        break;
    }
  }


  Token eof_token = {0};
  eof_token.type = EF;
  eof_token.has_literal = 0;
  eof_token.line = s->line;

  tokens_add(tokens, eof_token);
}
