#include "tokenizer.h"

int main(int argc, char *argv[])
{
  FILE *input = fopen(argv[1], "r");
  while (!feof(input))
  {
    Token tok = getToken(input);
    tok.__printToken();
  }
  return 0;
}
