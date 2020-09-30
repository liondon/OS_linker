#include "tokenizer.h"

int main()
{
  FILE *input = fopen("TestData/testInput.txt", "r");
  for (int i = 0; i < 55; i++)
  {
    getToken(input);
  }
  return 0;
}