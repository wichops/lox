#include <stdio.h>
#include <string.h>

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

/* static void string(Scanner *s) { */
/*   while (peek(s) != '"' && is_at_end(s)) { */
/*     if (peek(s) == '\n') s->line++; */
/*     advance(s); */
/*   } */

/*   if (is_at_end(s)) { */
/*     error(s->line, "Unterminated string"); */
/*     return; */
/*   } */

/*   advance(s); */
/* } */

static void add_token(Scanner* s, TokenType type, Token* tokens) {
  printf("[DEBUG] add_token()\n");
  size_t length = s->current - s->start;
  char* lexeme = malloc(length);
  printf("[DEBUG] start: %lu. current: %lu\n", s->start, s->current);
  strncpy(lexeme, s->source + s->start, length);

  tokens[s->last_index] = (Token) {type, lexeme, s->line };
  s->last_index++;
}

void scanner_init(Scanner* s, const char* source) {
  s->source = source;
  s->current = 0;
  s->start = 0;
  s->line = 1;
  s->last_index = 0;
}

void scanner_scan_tokens(Scanner *s, Token* tokens) {
  printf("[DEBUG] scan_tokens()\n");
  /* while (current < len) { */
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
      default: error(s->line, "Unexpected character."); break;
    }
    s->line++;
  }

  tokens[s->last_index] = (Token){ EOF, "", s->line};
}
