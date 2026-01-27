#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <stdio.h>

#include "token.h"
#include "scanner.h"
#include "error.h"

void print_tokens(TokenArray* tokens) {
  printf("[DEBUG] print_tokens()\n");
  printf("[DEBUG] Tokens count: %lu \n", tokens->index);
  for (size_t i = 0; i < tokens->index; i++) {
    Token* t = tokens_get(tokens, i);
    if (t != NULL) {
      printf("%d | Token: %s %d", t->line, t->lexeme, t->line);
      if (t->has_literal) {
        switch(t->type) {
          case STRING:
            printf(" -> %s", t->literal.string_val);
            break;
          default:
            break;
        }
      }
      printf("\n");
    }
  }
}

void run(const char* source) {
  printf("[DEBUG] run() \n %s \n", source);
  Token* array = malloc(sizeof(Token) * 500);
  TokenArray tokens = (TokenArray) { array, 0};

  Scanner scanner;
  Scanner* s = &scanner;
  scanner_init(s, source);
  scanner_scan_tokens(s, &tokens);
  print_tokens(&tokens);
}

void run_file(const char* filename) {
  char* buffer = 0;
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

void run_prompt(void) {
  char buffer[200];
  printf("> ");
  while (fgets(buffer, 199, stdin) != NULL) {
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
