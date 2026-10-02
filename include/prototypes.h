#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include <string>
#include <vector>
#include "types.h"
#include "globals.h"

using namespace std;

// ---------------------------------------------------------------------------
// ui.cpp
// ---------------------------------------------------------------------------
string getBaseColor(int base);
void   enableVirtualTerminal();
void   clearScreen();
string stripAnsi(const string &str);
string centerText(const string &text, int width);
void   printCentered(const string &text, int width = TERMINAL_WIDTH);
void   printDivider(char ch, const string &color, int width = TERMINAL_WIDTH);
int    promptScrollableMenu(const string &title, const vector<string> &options, int defaultIdx = 0);
void   pauseConsole();
int    getValidatedInt(const string &prompt, int minVal, int maxVal);

// ---------------------------------------------------------------------------
// conversion.cpp
// ---------------------------------------------------------------------------
string getBaseName(int base);
string getBaseCode(int base);
bool   validateInput(const string &inputStr, int base, string &errorMsg);
double toDecimal(const string &inputStr, int base);
string fromDecimal(double decVal, int targetBase, int maxFrac = 6);
string formatDecimalValue(double value);
InputNumber processConversion(const string &inputStr, int base);

// ---------------------------------------------------------------------------
// arithmetic.cpp  (standard ops + binary complements + custom expression)
// ---------------------------------------------------------------------------
ArithmeticResult  processArithmetic(const vector<InputNumber> &numbers,
                                    int operationChoice);
string            padBinary(const string &bin, int width);
string            onesComplementBin(const string &bin);
string            addOneTobin(const string &bin);
string            twosComplementBin(const string &bin);
string            addBinaryStrings(const string &a, const string &b,
                                   bool &carryOut);
int               chooseBitWidth(int requiredBits);
ComplementResult  computeComplements(const InputNumber &num);
CompSubResult     complementSubtract(const InputNumber &A,
                                     const InputNumber &B,
                                     bool useTwos);
Token             makeToken(TokenKind k, char o, int idx, const string &r);
int               operatorPrecedence(char op);
vector<Token>     tokenizeExpr(const string &expr, int numVars,
                               string &errorMsg);
vector<Token>     infixToPostfix(const vector<Token> &tokens,
                                 string &errorMsg);
RpnResult         makeRpnResult(bool ok, const string &msg, double val);
RpnResult         evalPostfix(const vector<Token> &postfix,
                              const vector<InputNumber> &numbers);
CustomExprResult  processCustomExpression(const string &exprStr,
                                          const vector<InputNumber> &numbers);

// ---------------------------------------------------------------------------
// bcd.cpp
// ---------------------------------------------------------------------------
string              decToBCD(const string &decStr);
BCDComplementResult computeBCDComplements(const InputNumber &num);
BCDAddResult        bcdAdd(const InputNumber &A, const InputNumber &B);
BCDSubResult        bcdSubtract(const InputNumber &A, const InputNumber &B,
                                bool useTens);

// ---------------------------------------------------------------------------
// display.cpp  (print* functions + interactive menus)
// ---------------------------------------------------------------------------
void displayAsciiIntro();
void displayHeader();
void printResultsTable(const vector<InputNumber> &numbers);
void printDetailedSteps(const InputNumber &num, int index);
void printArithmeticResult(const ArithmeticResult &result);
void printCustomExprResult(const CustomExprResult &result);
void printComplementTable(const vector<InputNumber> &numbers);
void printCompSubResult(const CompSubResult &result,
                        const InputNumber &A,
                        const InputNumber &B);
void printBCDComplementTable(const vector<InputNumber> &numbers);
void printBCDAddResult(const BCDAddResult &result);
void printBCDSubResult(const BCDSubResult &result,
                       const InputNumber & /*A*/,
                       const InputNumber & /*B*/);
CustomExprResult promptCustomExpression(const vector<InputNumber> &numbers,
                                        const string &varLegend,
                                        int count);
void runComplementSubmenu(const vector<InputNumber> &numbers, int count);
void runInteractiveConverter();
void runPresetCombinations();

#endif // PROTOTYPES_H
