// arithmetic.cpp — Standard operations, binary complements, and custom
//                  expression evaluator (Shunting-Yard + RPN)

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <cctype>
#include <algorithm>

#include "../include/globals.h"
#include "../include/prototypes.h"

using namespace std;

// =====================================================================================
// STANDARD ARITHMETIC
// =====================================================================================

ArithmeticResult processArithmetic(const vector<InputNumber> &numbers,
                                   int operationChoice)
{
    ArithmeticResult result;
    result.isValid  = true;
    result.errorMsg = "";

    switch (operationChoice)
    {
    case 0:
        result.operationName   = "Addition";
        result.operationSymbol = "+";
        result.decimalValue    = 0.0;
        for (const auto &num : numbers) result.decimalValue += num.decimalValue;
        break;
    case 1:
        result.operationName   = "Subtraction";
        result.operationSymbol = "-";
        result.decimalValue    = numbers[0].decimalValue;
        for (size_t i = 1; i < numbers.size(); ++i)
            result.decimalValue -= numbers[i].decimalValue;
        break;
    case 2:
        result.operationName   = "Multiplication";
        result.operationSymbol = "x";
        result.decimalValue    = 1.0;
        for (const auto &num : numbers) result.decimalValue *= num.decimalValue;
        break;
    case 3:
        result.operationName   = "Division";
        result.operationSymbol = "/";
        result.decimalValue    = numbers[0].decimalValue;
        for (size_t i = 1; i < numbers.size(); ++i)
        {
            if (fabs(numbers[i].decimalValue) < 1e-12)
            {
                result.isValid  = false;
                result.errorMsg = "Division by zero is not allowed.";
                break;
            }
            result.decimalValue /= numbers[i].decimalValue;
        }
        break;
    default:
        result.isValid  = false;
        result.errorMsg = "Unknown arithmetic operation.";
        break;
    }

    // Build original-value expression
    stringstream expSS;
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        if (i > 0) expSS << " " << result.operationSymbol << " ";
        expSS << numbers[i].originalStr << " [" << getBaseCode(numbers[i].base) << "]";
    }
    result.expression = expSS.str();

    // Build decimal-equivalent expression
    stringstream decExpSS;
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        if (i > 0) decExpSS << " " << result.operationSymbol << " ";
        decExpSS << numbers[i].decStr;
    }

    if (result.isValid)
    {
        result.binStr  = fromDecimal(result.decimalValue, 2);
        result.octStr  = fromDecimal(result.decimalValue, 8);
        result.decStr  = formatDecimalValue(result.decimalValue);
        result.hexStr  = fromDecimal(result.decimalValue, 16);
        decExpSS << " = " << result.decStr;
    }
    else
        result.binStr = result.octStr = result.decStr = result.hexStr = "UNDEFINED";

    result.decimalExpr = decExpSS.str();
    return result;
}

// =====================================================================================
// BINARY COMPLEMENT HELPERS
// =====================================================================================

string padBinary(const string &bin, int width)
{
    if (static_cast<int>(bin.length()) >= width) return bin;
    return string(width - static_cast<int>(bin.length()), '0') + bin;
}

string onesComplementBin(const string &bin)
{
    string result = bin;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = (result[i] == '0') ? '1' : '0';
    return result;
}

string addOneTobin(const string &bin)
{
    string result = bin;
    int carry = 1;
    for (int i = static_cast<int>(result.size()) - 1; i >= 0 && carry; --i)
    {
        int sum    = (result[i] - '0') + carry;
        result[i]  = char('0' + (sum % 2));
        carry      = sum / 2;
    }
    return result;
}

string twosComplementBin(const string &bin)
{
    return addOneTobin(onesComplementBin(bin));
}

string addBinaryStrings(const string &a, const string &b, bool &carryOut)
{
    int n = static_cast<int>(a.size());
    string result(n, '0');
    int carry = 0;
    for (int i = n - 1; i >= 0; --i)
    {
        int sum   = (a[i] - '0') + (b[i] - '0') + carry;
        result[i] = char('0' + (sum % 2));
        carry     = sum / 2;
    }
    carryOut = (carry == 1);
    return result;
}

int chooseBitWidth(int requiredBits)
{
    int width = 8;
    while (width < requiredBits) width += 8;
    return width;
}

ComplementResult computeComplements(const InputNumber &num)
{
    ComplementResult res;
    res.applicable = false;

    if (!num.isValid)
    {
        res.note = "Invalid input — complement not computed.";
        return res;
    }
    if (num.originalStr.find('.') != string::npos)
    {
        res.note = "Fractional numbers — complement not applicable.";
        return res;
    }
    if (!num.binStr.empty() && num.binStr[0] == '-')
    {
        res.note = "Negative numbers — complement display not supported.";
        return res;
    }

    res.applicable = true;
    string rawBin  = num.binStr;
    size_t dot     = rawBin.find('.');
    if (dot != string::npos) rawBin = rawBin.substr(0, dot);

    int requiredBits = static_cast<int>(rawBin.size());
    res.bitWidth  = chooseBitWidth(requiredBits);
    res.paddedBin = padBinary(rawBin, res.bitWidth);

    // 1's complement
    res.onesComp_bin = onesComplementBin(res.paddedBin);
    double onesDecVal = toDecimal(res.onesComp_bin, 2);
    res.onesComp_oct = fromDecimal(onesDecVal, 8);
    res.onesComp_dec = formatDecimalValue(onesDecVal);
    res.onesComp_hex = fromDecimal(onesDecVal, 16);

    // 2's complement
    res.twosComp_bin = twosComplementBin(res.paddedBin);
    double twosDecVal = toDecimal(res.twosComp_bin, 2);
    res.twosComp_oct = fromDecimal(twosDecVal, 8);
    res.twosComp_dec = formatDecimalValue(twosDecVal);
    res.twosComp_hex = fromDecimal(twosDecVal, 16);

    return res;
}

CompSubResult complementSubtract(const InputNumber &A,
                                  const InputNumber &B,
                                  bool useTwos)
{
    CompSubResult res;
    res.isValid    = false;
    res.useTwos    = useTwos;
    res.hasCarry   = false;
    res.isNegative = false;
    res.decimalResult = 0.0;

    if (!A.isValid || !B.isValid)
    {
        res.errorMsg = "One or both inputs are invalid.";
        return res;
    }
    if (A.originalStr.find('.') != string::npos ||
        B.originalStr.find('.') != string::npos)
    {
        res.errorMsg = "Complement subtraction requires integer inputs (no radix fractions).";
        return res;
    }

    string rawA = A.binStr, rawB = B.binStr;
    if (!rawA.empty() && rawA[0] == '-') rawA = rawA.substr(1);
    if (!rawB.empty() && rawB[0] == '-') rawB = rawB.substr(1);

    int reqBits   = static_cast<int>(max(rawA.size(), rawB.size()));
    res.bitWidth  = chooseBitWidth(reqBits);
    res.A_bin     = padBinary(rawA, res.bitWidth);
    res.B_bin     = padBinary(rawB, res.bitWidth);

    res.comp_bin  = useTwos ? twosComplementBin(res.B_bin)
                            : onesComplementBin(res.B_bin);

    bool carry = false;
    res.sum_bin   = addBinaryStrings(res.A_bin, res.comp_bin, carry);
    res.hasCarry  = carry;

    if (useTwos)
    {
        res.adjusted_bin = res.sum_bin;
        res.isNegative   = !carry;
    }
    else
    {
        if (carry)
        {
            bool dummyCarry = false;
            string carryStr = padBinary("1", res.bitWidth);
            res.adjusted_bin = addBinaryStrings(res.sum_bin, carryStr, dummyCarry);
            res.isNegative   = false;
        }
        else
        {
            res.adjusted_bin = onesComplementBin(res.sum_bin);
            res.isNegative   = true;
        }
    }

    double mag = toDecimal(res.adjusted_bin, 2);
    res.decimalResult = res.isNegative ? -mag : mag;
    res.binStr        = (res.isNegative ? "-" : "") + res.adjusted_bin;
    res.octStr        = fromDecimal(res.decimalResult, 8);
    res.decStr        = formatDecimalValue(res.decimalResult);
    res.hexStr        = fromDecimal(res.decimalResult, 16);
    res.isValid       = true;
    return res;
}

// =====================================================================================
// CUSTOM EXPRESSION EVALUATOR  (Shunting-Yard + RPN)
// =====================================================================================

Token makeToken(TokenKind k, char o, int idx, const string &r)
{
    Token t;
    t.kind   = k;
    t.op     = o;
    t.varIdx = idx;
    t.raw    = r;
    return t;
}

int operatorPrecedence(char op)
{
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

vector<Token> tokenizeExpr(const string &expr, int numVars, string &errorMsg)
{
    vector<Token> tokens;
    errorMsg = "";

    for (size_t i = 0; i < expr.length(); ++i)
    {
        char c = expr[i];
        if (isspace(static_cast<unsigned char>(c))) continue;

        if (c == '(')
            tokens.push_back(makeToken(TOK_LPAREN, '(', -1, "("));
        else if (c == ')')
            tokens.push_back(makeToken(TOK_RPAREN, ')', -1, ")"));
        else if (c == '+' || c == '-' || c == '*' || c == '/')
            tokens.push_back(makeToken(TOK_OP, c, -1, string(1, c)));
        else if (isalpha(static_cast<unsigned char>(c)))
        {
            int idx = tolower(static_cast<unsigned char>(c)) - 'a';
            if (idx >= numVars)
            {
                errorMsg = string("Variable '") + c + "' is out of range. Only " +
                           to_string(numVars) + " input(s) defined (a" +
                           (numVars > 1 ? string(" to ") + char('a' + numVars - 1) : "") + ").";
                return vector<Token>();
            }
            tokens.push_back(makeToken(TOK_VAR, 0, idx, string(1, c)));
        }
        else
        {
            errorMsg = string("Invalid character '") + c + "' in expression. " +
                       "Use letters a-" + char('a' + numVars - 1) +
                       " and operators + - * / ( ).";
            return vector<Token>();
        }
    }
    tokens.push_back(makeToken(TOK_END, 0, -1, ""));

    // Implicit multiplication pass
    vector<Token> expanded;
    for (size_t j = 0; j < tokens.size(); ++j)
    {
        expanded.push_back(tokens[j]);
        if (j + 1 < tokens.size())
        {
            TokenKind cur  = tokens[j].kind;
            TokenKind next = tokens[j + 1].kind;
            bool needMul =
                (cur == TOK_VAR    && next == TOK_LPAREN)  ||
                (cur == TOK_RPAREN && next == TOK_VAR)     ||
                (cur == TOK_RPAREN && next == TOK_LPAREN)  ||
                (cur == TOK_VAR    && next == TOK_VAR);
            if (needMul)
                expanded.push_back(makeToken(TOK_OP, '*', -1, "*"));
        }
    }
    return expanded;
}

vector<Token> infixToPostfix(const vector<Token> &tokens, string &errorMsg)
{
    vector<Token> output, opStack;
    errorMsg = "";

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const Token &tok = tokens[i];
        if (tok.kind == TOK_END) break;

        if (tok.kind == TOK_VAR)
            output.push_back(tok);
        else if (tok.kind == TOK_OP)
        {
            while (!opStack.empty() &&
                   opStack.back().kind == TOK_OP &&
                   operatorPrecedence(opStack.back().op) >= operatorPrecedence(tok.op))
            {
                output.push_back(opStack.back());
                opStack.pop_back();
            }
            opStack.push_back(tok);
        }
        else if (tok.kind == TOK_LPAREN)
            opStack.push_back(tok);
        else if (tok.kind == TOK_RPAREN)
        {
            bool foundLeft = false;
            while (!opStack.empty())
            {
                if (opStack.back().kind == TOK_LPAREN)
                {
                    opStack.pop_back();
                    foundLeft = true;
                    break;
                }
                output.push_back(opStack.back());
                opStack.pop_back();
            }
            if (!foundLeft)
            {
                errorMsg = "Mismatched parentheses: extra ')' detected.";
                return vector<Token>();
            }
        }
    }
    while (!opStack.empty())
    {
        if (opStack.back().kind == TOK_LPAREN)
        {
            errorMsg = "Mismatched parentheses: unclosed '(' detected.";
            return vector<Token>();
        }
        output.push_back(opStack.back());
        opStack.pop_back();
    }
    return output;
}

RpnResult makeRpnResult(bool ok, const string &msg, double val)
{
    RpnResult r;
    r.ok  = ok;
    r.msg = msg;
    r.val = val;
    return r;
}

RpnResult evalPostfix(const vector<Token> &postfix,
                      const vector<InputNumber> &numbers)
{
    vector<double> stk;
    for (size_t i = 0; i < postfix.size(); ++i)
    {
        const Token &tok = postfix[i];
        if (tok.kind == TOK_VAR)
            stk.push_back(numbers[tok.varIdx].decimalValue);
        else if (tok.kind == TOK_OP)
        {
            if (stk.size() < 2)
                return makeRpnResult(false,
                    string("Not enough operands for operator '") + tok.op + "'.", 0.0);
            double b = stk.back(); stk.pop_back();
            double a = stk.back(); stk.pop_back();

            if      (tok.op == '+') stk.push_back(a + b);
            else if (tok.op == '-') stk.push_back(a - b);
            else if (tok.op == '*') stk.push_back(a * b);
            else if (tok.op == '/')
            {
                if (fabs(b) < 1e-12)
                    return makeRpnResult(false,
                        "Division by zero: divisor evaluates to 0.", 0.0);
                stk.push_back(a / b);
            }
        }
    }
    if (stk.empty())
        return makeRpnResult(false, "Expression is empty or produced no result.", 0.0);
    if (stk.size() > 1)
        return makeRpnResult(false, "Malformed expression: too many values left unevaluated.", 0.0);
    return makeRpnResult(true, "", stk[0]);
}

CustomExprResult processCustomExpression(const string &exprStr,
                                          const vector<InputNumber> &numbers)
{
    CustomExprResult res;
    res.isValid       = false;
    res.originalExpr  = exprStr;
    res.decimalValue  = 0.0;
    res.binStr = res.octStr = res.decStr = res.hexStr = "UNDEFINED";

    if (exprStr.empty())
    {
        res.errorMsg = "Expression cannot be empty.";
        return res;
    }

    string errMsg;
    vector<Token> tokens = tokenizeExpr(exprStr, static_cast<int>(numbers.size()), errMsg);
    if (!errMsg.empty()) { res.errorMsg = errMsg; return res; }
    if (tokens.empty() || tokens[0].kind == TOK_END)
    {
        res.errorMsg = "Expression is empty after parsing.";
        return res;
    }

    vector<Token> postfix = infixToPostfix(tokens, errMsg);
    if (!errMsg.empty()) { res.errorMsg = errMsg; return res; }

    RpnResult rr = evalPostfix(postfix, numbers);
    if (!rr.ok) { res.errorMsg = rr.msg; return res; }

    res.isValid      = true;
    res.decimalValue = rr.val;
    res.decStr       = formatDecimalValue(res.decimalValue);
    res.binStr       = fromDecimal(res.decimalValue, 2);
    res.octStr       = fromDecimal(res.decimalValue, 8);
    res.hexStr       = fromDecimal(res.decimalValue, 16);

    // Build substituted and decimal-equivalent expressions
    stringstream subSS, decSS;
    for (size_t i = 0; i < exprStr.length(); ++i)
    {
        char c = exprStr[i];
        if (isspace(static_cast<unsigned char>(c))) continue;
        if (isalpha(static_cast<unsigned char>(c)))
        {
            int idx = tolower(static_cast<unsigned char>(c)) - 'a';
            subSS << numbers[idx].originalStr << "[" << getBaseCode(numbers[idx].base) << "]";
            decSS << numbers[idx].decStr;
        }
        else if (c == '+') { subSS << " + "; decSS << " + "; }
        else if (c == '-') { subSS << " - "; decSS << " - "; }
        else if (c == '*') { subSS << " x "; decSS << " x "; }
        else if (c == '/') { subSS << " / "; decSS << " / "; }
        else               { subSS << c;     decSS << c;     }
    }
    decSS << " = " << res.decStr;
    res.substitutedExpr = subSS.str();
    res.decimalExpr     = decSS.str();
    return res;
}
