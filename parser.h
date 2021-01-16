#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include "validator.h"
using namespace std;

// Pass1: create Symbol Table, verify syntax
unordered_map<string, int> Pass1(FILE *input)
{
  vector<string> symbolOrder;
  unordered_map<string, int> symbolTable;
  unordered_map<string, string> symbolError;
  vector<string> warnings;
  unsigned int moduleCount = 0;
  unsigned int moduleOffset = 0;
  Token tok;
  string errorMsg;

  while (!feof(input))
  {
    // cout << "Read def" << endl;
    tok = readInt(input, 1);
    if (feof(input))
    {
      break;
    }
    moduleCount++;

    int defCount = atoi(tok.token);
    if (defCount > 16)
    {
      __parseError(4, tok.lineNo, tok.offset);
    }
    for (int i = 0; i < defCount; i++)
    {
      tok = readSymbol(input);
      string symbol(tok.token);
      tok = readInt(input);
      int val = atoi(tok.token);

      if (symbolTable.count(symbol))
      {
        errorMsg = __errorMsg(4);
        symbolError.emplace(symbol, errorMsg);
      }
      else
      {
        symbolOrder.emplace_back(symbol);
        symbolTable.emplace(symbol, val + moduleOffset);
      }
    }

    // cout << "Read use" << endl;
    tok = readInt(input);
    int useCount = atoi(tok.token);
    if (useCount > 16)
    {
      __parseError(5, tok.lineNo, tok.offset);
    }
    for (int i = 0; i < useCount; i++)
    {
      tok = readSymbol(input);
      string symbol(tok.token);
    }

    // cout << "Read inst" << endl;
    tok = readInt(input);
    int codeCount = atoi(tok.token);
    if (codeCount + moduleOffset > 512)
    {
      __parseError(6, tok.lineNo, tok.offset);
    }

    for (auto symbol : symbolOrder)
    {
      int symbolAddr = symbolTable[symbol];
      int maxAddr = moduleOffset + codeCount - 1;
      if (symbolAddr > maxAddr)
      {
        vector<string> addr = {to_string(symbolAddr), to_string(maxAddr)};
        warnings.emplace_back(__warningMsg(0, moduleCount, symbol, addr));
        symbolTable[symbol] = moduleOffset;
      }
    }

    for (int i = 0; i < codeCount; i++)
    {
      tok = readIEAR(input);
      char addressMode = tok.token[0];
      tok = readInt(input);
      int instr = atoi(tok.token);
      int opcode = instr / 1000;
      int operand = instr % 1000;
    }

    for (size_t i = 0; i < warnings.size(); i++)
    {
      symbolOrder.emplace_back(warnings[i]);
    }

    moduleOffset += codeCount;
  }
  fclose(input);

  // print Symbol Table
  cout << "Symbol Table" << endl;
  for (auto symbol : symbolOrder)
    if (symbolTable.count(symbol) == 0)
    {
      cout << symbol << endl; // prints out warning message
    }
    else
    {
      cout << symbol << "=" << symbolTable[symbol] << "  " << symbolError[symbol] << endl;
    }
  cout << endl;

  return symbolTable;
}

// Pass2: create Memory Map, verify semantics
vector<int> Pass2(FILE *input, unordered_map<string, int> symbolTable)
{
  vector<string> defOrder;
  unordered_map<string, unsigned int> defListChecker;
  vector<string> passWarnings;
  vector<int> memoryMap;
  unordered_map<int, vector<string>> errorMap;
  int memoryMapIdx = 0;
  unsigned int moduleCount = 0;
  unsigned int moduleOffset = 0;
  Token tok;

  cout << "Memory Map" << endl;
  while (!feof(input))
  {
    vector<string> moduleWarnings;
    vector<string> symbolList;
    vector<bool> symbolListChecker;

    tok = readInt(input, 1);
    if (feof(input))
    {
      break;
    }
    int defCount = atoi(tok.token);
    moduleCount++;

    for (int i = 0; i < defCount; i++)
    {
      tok = readSymbol(input);
      string symbol(tok.token);
      tok = readInt(input);
      int val = atoi(tok.token);
      if (defListChecker.count(symbol) == 0)
      {
        defOrder.emplace_back(symbol);
        defListChecker.emplace(symbol, moduleCount);
      }
    }

    tok = readInt(input);
    int useCount = atoi(tok.token);
    for (int i = 0; i < useCount; i++)
    {
      tok = readSymbol(input);
      string symbol(tok.token);
      symbolList.emplace_back(symbol);
      symbolListChecker.emplace_back(1);
      defListChecker[symbol] = 0;
    }

    tok = readInt(input);
    int codeCount = atoi(tok.token);
    for (int i = 0; i < codeCount; i++)
    {
      vector<string> errorMsg;

      tok = readIEAR(input);
      char addressMode = tok.token[0];
      tok = readInt(input);
      int instr = atoi(tok.token);

      if (addressMode == 'I' && instr >= 10000)
      {
        errorMsg.emplace_back(__errorMsg(5));
        instr = 9999;
      }

      int opcode = instr / 1000;
      int operand = instr % 1000;
      if (opcode >= 10)
      {
        errorMsg.emplace_back(__errorMsg(6));
        opcode = 9;
        operand = 999;
      }
      else
      {
        switch (addressMode)
        {
        case 'I':
          break;
        case 'A':
          if (operand >= 512)
          {
            errorMsg.emplace_back(__errorMsg(0));
            operand = 0;
          }
          break;
        case 'R':
          if (operand > codeCount - 1)
          {
            errorMsg.emplace_back(__errorMsg(1));
            operand = 0;
          }
          operand += moduleOffset;
          break;
        case 'E':
          if (operand >= symbolList.size())
          {
            errorMsg.emplace_back(__errorMsg(2));
            break;
          }
          string symbol = symbolList[operand];
          symbolListChecker[operand] = 0;
          if (!symbolTable.count(symbolList[operand]))
          {
            errorMsg.emplace_back(__errorMsg(3, symbolList[operand]));
            operand = 0;
          }
          else
          {
            operand = symbolTable[symbol];
          }
          break;
        }
      }

      instr = opcode * 1000 + operand;
      memoryMap.emplace_back(instr);
      errorMap.emplace(memoryMap.size() - 1, errorMsg);
    }

    for (size_t i = 0; i < symbolListChecker.size(); i++)
    {
      if (symbolListChecker[i])
      {
        moduleWarnings.emplace_back(__warningMsg(1, moduleCount, symbolList[i]));
      }
    }

    // print Memory Map after parsing every module
    for (size_t i = memoryMapIdx; i < memoryMap.size(); i++)
    {
      memoryMapIdx++;
      cout << setfill('0') << setw(3) << i << ": " << setfill('0') << setw(4) << memoryMap[i];
      for (string err : errorMap[i])
      {
        cout << "  " << err;
      }
      cout << endl;
    }

    for (size_t i = 0; i < moduleWarnings.size(); i++)
    {
      cout << moduleWarnings[i] << endl;
    }

    moduleOffset += codeCount;
  }
  fclose(input);

  for (string def : defOrder)
  {
    if (defListChecker[def])
    {
      passWarnings.emplace_back(__warningMsg(2, defListChecker[def], def));
    }
  }
  for (string warningMsg : passWarnings)
  {
    cout << warningMsg << endl;
  }

  return memoryMap;
}