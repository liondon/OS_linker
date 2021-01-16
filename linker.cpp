#include <cstdio>
#include "parser.h"
using namespace std;

int main(int argc, char *argv[])
{
  FILE *input = fopen(argv[1], "r");
  unordered_map<string, int> symbolTable = Pass1(input);

  input = fopen(argv[1], "r");
  vector<int> memoryMap = Pass2(input, symbolTable);

  return 0;
}
