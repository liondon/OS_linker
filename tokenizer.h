#include <cstring>
#include <cstdio>
#include "Token.h"
using namespace std;

#define BUFSZ (1024)

Token getToken(FILE *inputFile)
{
  static bool newline = 1;                    // when newline = 1, must read a new line
  static char buffer[BUFSZ];                  // large buffer that we reuse across calls
  static unsigned int lineNo = 0, offset = 0; // keep tracking the position of token in the file
  char *token = NULL;

  while (1)
  {
    // keep reading new line if there's no token extracted
    while (newline)
    {
      if (fgets(buffer, BUFSZ, inputFile) == NULL && feof(inputFile))
      {
        Token tok(NULL, lineNo, offset, NULL);
        return tok; // eof
      }
      lineNo++;
      offset = 1;

      // seed the tokenizer
      token = strtok(buffer, " \t\n");
      if (token != NULL)
      {
        newline = 0; // read the same line next time getToken() is called

        offset = token - buffer + 1;
        Token tok(token, lineNo, offset, buffer);
        offset += strlen(token);
        return tok;
      } // else: Looks like this is an empty line. Keep reading next new line
    }

    token = strtok(NULL, " \t\n"); // use NULL to read from the same line as last time
    if (token != NULL)
    {
      offset = token - buffer + 1;
      Token tok(token, lineNo, offset, buffer);
      offset += strlen(token);
      return tok;
    } // else: Looks like this is the end of this line.

    newline = 1; // Read a new line
  }
}
