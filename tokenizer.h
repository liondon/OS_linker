#include <cstring>
#include <iostream>
using namespace std;

#define BUFSZ (1024)

void printToken(char *, size_t &, size_t &, char *);

// TODO: use `stdin` instead of `inputFile` argument
char *getToken(FILE *inputFile)
{
  static bool newline = 1;              // when newline = 1, must read a new line
  static char buffer[BUFSZ];            // large buffer that we reuse across calls
  static size_t lineNo = 0, offset = 0; // keep tracking the position of token in the file
  char *token = NULL;

  while (1)
  {
    // keep reading new line if there's no token extracted
    while (newline)
    {
      if (fgets(buffer, BUFSZ, inputFile) == NULL && feof(inputFile) != 0)
      {
        cout << "Final Spot in File : line=" << lineNo << " offset=" << offset << endl;
        return NULL; // eof
      }
      lineNo++;

      // seed the tokenizer
      token = strtok(buffer, " \t\n");
      if (token != NULL)
      {
        newline = 0; // read the same line next time getToken() is called

        printToken(token, lineNo, offset, buffer);
        return token;
      } // else: Looks like this is an empty line. Keep reading next new line
    }

    token = strtok(NULL, " \t\n"); // use NULL to read from the same line as last time
    if (token != NULL)
    {
      printToken(token, lineNo, offset, buffer);
      return token;
    }            // else: Looks like this is the end of this line.
    newline = 1; // Read a new line
  }
}

void printToken(char *token, size_t &lineNo, size_t &offset, char *buffer)
{
  offset = token - buffer + 1;
  cout << "Token: " << lineNo << ":" << offset << " : " << token << endl;
  offset = offset + strlen(token);
  return;
}