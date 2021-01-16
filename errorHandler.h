#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

void __parseError(const int, const size_t, const size_t);
string __errorMsg(const int, const string);
string __warningMsg(const int, const unsigned int, const string, const vector<string>);

void __parseError(const int errcode, const unsigned int lineNo, const unsigned int offset)
{
  static char *errstr[] = {
      "NUM_EXPECTED",           // 0: Number Expected
      "SYM_EXPECTED",           // 1: Symbol Expected
      "SYM_TOO_LONG",           // 2: Symbol Name too long
      "ADDR_EXPECTED",          // 3: Addressing Expected which is A/E/I/R
      "TOO_MANY_DEF_IN_MODULE", // 4: defCount > 16
      "TOO_MANY_USE_IN_MODULE", // 5: useCount > 16
      "TOO_MANY_INSTR",         // 6: total num_instr exceeds memory size (512)
  };
  printf("Parse Error line %u offset %u: %s\n", lineNo, offset, errstr[errcode]);
  exit(EXIT_FAILURE);
}

string __errorMsg(const int errcode, const string symbol = "NULL")
{
  string errstr[] = {
      "Error: Absolute address exceeds machine size; zero used",                 // 0
      "Error: Relative address exceeds module size; zero used",                  // 1
      "Error: External address exceeds length of uselist; treated as immediate", // 2
      "Error: " + symbol + " is not defined; zero used",                         // 3
      "Error: This variable is multiple times defined; first value used",        // 4
      "Error: Illegal immediate value; treated as 9999",                         // 5
      "Error: Illegal opcode; treated as 9999",                                  // 6
  };
  return errstr[errcode];
}

string __warningMsg(const int errcode, const unsigned int moduleCount,
                    const string symbol, const vector<string> addr = {"NULL", "NULL"})
{
  string warnstr[] = {
      "Warning: Module " + to_string(moduleCount) + ": " + symbol +
          " too big " + addr[0] + " (max=" + addr[1] + ") assume zero relative", // 0
      "Warning: Module " + to_string(moduleCount) + ": " + symbol +
          " appeared in the uselist but was not actually used", // 1
      "Warning: Module " + to_string(moduleCount) + ": " + symbol +
          " was defined but never used", // 2
  };
  return warnstr[errcode];
}