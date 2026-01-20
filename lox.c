#include <stdlib.h>
#include <sysexits.h>
#include <stdio.h>

void run(const char* program) {
  printf("[DEBUG] run() \n %s \n", program);
}

void runFile(const char* filename) {
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
}

void runPrompt() {
  char buffer[200];
  printf("> ");
  while (scanf("%s", buffer) != EOF) {
    run(buffer);
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
    runFile(filename);
  } else {
    printf("[DEBUG] runPrompt() \n");
    runPrompt();
  }


  return 0;
}
