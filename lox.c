#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <stdio.h>

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

int HAD_ERROR = 0;

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

  EF // done this way because STDIO
} TokenType;

typedef struct Token {
  TokenType type;
  char* lexeme;
  size_t line;
} Token;

size_t start = 0;
size_t current = 0;
size_t last_index = 0;
size_t line = 1;

void report(int line, char* where, char* message) {
  printf("[DEBUG] report()\n");
  eprintf("Line %d - Error %s: %s", line, where, message);
}

void error(int line, char* message) {
  printf("[DEBUG] error()\n");
  report(line, "", message);
  HAD_ERROR = 1;
}

void print_tokens(Token* tokens) {
  printf("[DEBUG] print_tokens()\n");
  printf("[DEBUG] Tokens count: %lu \n", last_index);
  for (size_t i = 0; i < last_index; i++) {
    Token t = tokens[i];
    printf("Token: %s %lu \n", t.lexeme, t.line);
  }
}
void add_token(TokenType type, Token* tokens, const char* source) {
  size_t length = current - start;
  char* lexeme = malloc(length+1);
  printf("[DEBUG] start: %lu. current: %lu\n", start, current);
  strncpy(lexeme, source + start, length+1);

  tokens[last_index] = (Token) {type, lexeme, line };
  last_index++;
}

void scan_tokens(const char* source, Token* tokens) {
  size_t len = strlen(source);

  printf("[DEBUG] scan_tokens()\n");
  /* while (!is_at_end()) { */
  while (current < len) {
    start = current;
    char c = source[current];
    switch (c) {
      case '(': add_token(LEFT_PAREN, tokens, source); break;
      case ')': add_token(RIGHT_PAREN, tokens, source); break;
      case '{': add_token(LEFT_BRACE, tokens, source); break;
      case '}': add_token(RIGHT_BRACE, tokens, source); break;
      case ',': add_token(COMMA, tokens, source); break;
      case '.': add_token(DOT, tokens, source); break;
      case '-': add_token(MINUS, tokens, source); break;
      case '+': add_token(PLUS, tokens, source); break;
      case ';': add_token(SEMICOLON, tokens, source); break;
      case '*': add_token(STAR, tokens, source); break;
    }
    current++;
  }

  tokens[last_index] = (Token){ EOF, "", line};
}

void run(const char* program) {
  printf("[DEBUG] run() \n %s \n", program);
  Token tokens[500];
  scan_tokens(program, tokens);
  print_tokens(tokens);
}

void run_file(const char* filename) {
  char* buffer;
  long length;

  FILE* f = fopen(filename, "r");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
  }

  run(buffer);
  if (HAD_ERROR) {
    exit(EX_DATAERR);
  }
}

void run_prompt() {
  char buffer[200];
  printf("> ");
  while (scanf("%s", buffer) != EOF) {
    run(buffer);
    HAD_ERROR = 0;
    printf("> ");
  }
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    printf("Usage: jlox [script] \n");
    return EX_USAGE;
  } else if (argc == 2) {
    char* filename = argv[1];
    printf("[DEBUG] runFile(%s) \n", filename);
    run_file(filename);
  } else {
    printf("[DEBUG] runPrompt() \n");
    run_prompt();
  }


  return 0;
}
