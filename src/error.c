#include <stdio.h>
#include "error.h"

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

int HAD_ERROR = 0;

void report(int line, char* where, char* message) {
  printf("[DEBUG] report()\n");
  eprintf("Line %d - Error %s: %s", line, where, message);
}

void error(int line, char* message) {
  printf("[DEBUG] error()\n");
  report(line, "", message);
  HAD_ERROR = 1;
}
