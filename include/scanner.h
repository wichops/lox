#include <stdlib.h>
#include "token.h"

typedef struct {
  const char* source;
  size_t start;
  size_t current;
  size_t line;
  size_t last_index;    // Token list last index. TODO: refactor token list cursor.
} Scanner;

void scanner_init(Scanner* s, const char* source);
void scanner_scan_tokens(Scanner* s, Token* tokens);
