#include <iostream>
using namespace std;

class Token
{
public:
  Token();
  Token(char *, unsigned int, unsigned int, char *);
  void __printToken() const;

  // putting data member in 'public' to simplify the codes
  char *token;
  unsigned int lineNo;
  unsigned int offset;

private:
};

Token::Token() : token(NULL),
                 lineNo(0),
                 offset(0) {}

Token::Token(char *token, unsigned int lineNo,
             unsigned int offset, char *buffer) : token(token),
                                                  lineNo(lineNo),
                                                  offset(offset) {}

void Token::__printToken() const
{
  if (this->token == NULL)
  {
    cout << "Final Spot in File : line=" << lineNo << " offset=" << offset << endl;
  }
  else
  {
    cout << "Token: " << this->lineNo << ":" << this->offset << " : " << this->token << endl;
  }
  return;
}