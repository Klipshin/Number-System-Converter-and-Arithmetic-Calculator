#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// Core conversion / arithmetic structures
// ---------------------------------------------------------------------------

struct InputNumber
{
    string originalStr;
    int    base;
    double decimalValue;
    string binStr;
    string octStr;
    string decStr;
    string hexStr;
    bool   isValid;
};

struct ArithmeticResult
{
    string operationName;
    string operationSymbol;
    string expression;
    string decimalExpr;
    bool   isValid;
    string errorMsg;
    double decimalValue;
    string binStr;
    string octStr;
    string decStr;
    string hexStr;
};

struct CustomExprResult
{
    bool   isValid;
    string errorMsg;
    string originalExpr;
    string substitutedExpr;
    string decimalExpr;
    double decimalValue;
    string binStr;
    string octStr;
    string decStr;
    string hexStr;
};

// ---------------------------------------------------------------------------
// Binary complement structures
// ---------------------------------------------------------------------------

struct ComplementResult
{
    bool   applicable;
    string note;
    int    bitWidth;
    string paddedBin;
    // 1's complement
    string onesComp_bin;
    string onesComp_oct;
    string onesComp_dec;
    string onesComp_hex;
    // 2's complement
    string twosComp_bin;
    string twosComp_oct;
    string twosComp_dec;
    string twosComp_hex;
};

struct CompSubResult
{
    bool   isValid;
    string errorMsg;
    bool   useTwos;
    int    bitWidth;
    string A_bin;
    string B_bin;
    string comp_bin;
    string sum_bin;
    bool   hasCarry;
    string adjusted_bin;
    bool   isNegative;
    double decimalResult;
    string binStr;
    string octStr;
    string decStr;
    string hexStr;
};

// ---------------------------------------------------------------------------
// BCD structures
// ---------------------------------------------------------------------------

struct BCDDigitStep
{
    int    digitA;
    int    digitB;
    int    carryIn;
    int    rawSum;
    string rawBin;        // 4-bit binary of rawSum
    bool   needCorrect;
    int    corrected;
    string corrBin;       // 4-bit binary of output digit
    int    carryOut;
    int    bcdDigitOut;
};

struct BCDSubDigitStep
{
    int  digitA;
    int  compDigit;
    int  carryIn;
    int  rawSum;
    bool needCorrect;
    int  corrected;
    int  carryOut;
    int  bcdDigitOut;
};

struct BCDAddResult
{
    bool   isValid;
    string errorMsg;
    string A_dec;
    string B_dec;
    string A_bcd;
    string B_bcd;
    vector<BCDDigitStep> steps;
    int    finalCarry;
    string result_dec;
    string result_bcd;
    string result_bin;
    string result_oct;
    string result_hex;
};

struct BCDSubResult
{
    bool   isValid;
    string errorMsg;
    bool   useTens;       // false = 9's comp, true = 10's comp
    string A_dec;
    string B_dec;
    string A_bcd;
    string B_bcd;
    string comp_bcd;
    vector<BCDSubDigitStep> steps;
    bool   hasCarry;
    bool   isNegative;
    string adjusted_bcd;
    double decimalResult;
    string result_dec;
    string result_bcd;
    string result_bin;
    string result_oct;
    string result_hex;
};

struct BCDComplementResult
{
    bool   applicable;
    string note;
    string original_dec;
    string original_bcd;
    // 9's complement
    string ninesComp_dec;
    string ninesComp_bcd;
    string ninesComp_bin;
    string ninesComp_oct;
    string ninesComp_hex;
    // 10's complement
    string tensComp_dec;
    string tensComp_bcd;
    string tensComp_bin;
    string tensComp_oct;
    string tensComp_hex;
};

// ---------------------------------------------------------------------------
// Expression evaluator token
// ---------------------------------------------------------------------------

enum TokenKind
{
    TOK_VAR,
    TOK_OP,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_END,
    TOK_INVALID
};

struct Token
{
    TokenKind kind;
    char      op;
    int       varIdx;
    string    raw;
};

struct RpnResult
{
    bool   ok;
    string msg;
    double val;
};

#endif // TYPES_H
