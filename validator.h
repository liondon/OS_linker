#include <cstring>
#include <cctype>
#include "tokenizer.h"
#include "errorHandler.h"
using namespace std;

Token readInt(FILE *, bool);
Token readSymbol(FILE *);
Token readIEAR(FILE *);

Token readInt(FILE *inputFile, bool newSection = 0)
{
  Token tok = getToken(inputFile);
  const char *token = tok.token;

  if (token == NULL && newSection == 1) // input file correctly ended
  {
    // tok.__printToken();
    return tok;
  }
  if (token == NULL) // missing number token
  {
    __parseError(0, tok.lineNo, tok.offset);
  }
  for (size_t i = 0; i < strlen(token); i++)
  {
    if (!isdigit(token[i])) // number token contains non-digit character
    {
      __parseError(0, tok.lineNo, tok.offset);
    }
  }
  // tok.__printToken();
  return tok;
}

Token readSymbol(FILE *inputFile)
{
  Token tok = getToken(inputFile);
  const char *token = tok.token;

  if (token == NULL || !isalpha(token[0]))
  {
    __parseError(1, tok.lineNo, tok.offset);
  }
  if (strlen(token) > 16)
  {
    __parseError(2, tok.lineNo, tok.offset);
  }
  if (strlen(token) > 1)
  {
    for (size_t i = 1; i < strlen(token); i++)
    {
      if (!isalnum(token[i]))
      {
        __parseError(1, tok.lineNo, tok.offset);
      }
    }
  }
  // tok.__printToken();
  return tok;
}

Token readIEAR(FILE *inputFile)
{
  char *type = "IEAR";
  Token tok = getToken(inputFile);
  const char *token = tok.token;

  if (token == NULL || strchr(type, token[0]) == NULL)
  {
    __parseError(3, tok.lineNo, tok.offset);
  }
  // tok.__printToken();
  return tok;
}
