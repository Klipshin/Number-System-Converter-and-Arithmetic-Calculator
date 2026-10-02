// bcd.cpp — BCD encoding, 9's/10's complement, BCD addition and subtraction

#include <string>
#include <vector>
#include <cmath>

#include "../include/globals.h"
#include "../include/prototypes.h"

using namespace std;

// ---------------------------------------------------------------------------
// Convert decimal integer string to packed BCD (4-bit groups, space-separated)
// e.g. "42" -> "0100 0010"
// ---------------------------------------------------------------------------
string decToBCD(const string &decStr)
{
    string result = "";
    bool first = true;
    for (size_t i = 0; i < decStr.size(); ++i)
    {
        if (decStr[i] == '-') continue;
        int d = decStr[i] - '0';
        string bits = "";
        for (int b = 3; b >= 0; --b)
            bits += char('0' + ((d >> b) & 1));
        if (!first) result += " ";
        result += bits;
        first = false;
    }
    return result;
}

// ---------------------------------------------------------------------------
// 9's and 10's BCD complement for a single InputNumber
// ---------------------------------------------------------------------------
BCDComplementResult computeBCDComplements(const InputNumber &num)
{
    BCDComplementResult res;
    res.applicable = false;

    if (!num.isValid)
    {
        res.note = "Invalid input — BCD complement not computed.";
        return res;
    }
    if (num.originalStr.find('.') != string::npos)
    {
        res.note = "Fractional numbers — BCD complement not applicable.";
        return res;
    }
    if (num.decimalValue < 0)
    {
        res.note = "Negative numbers — BCD complement not applicable.";
        return res;
    }

    res.applicable   = true;
    string decStr    = num.decStr;
    res.original_dec = decStr;
    res.original_bcd = decToBCD(decStr);

    int n = static_cast<int>(decStr.size());

    // 9's complement: each digit d -> 9 - d
    string ninesDecStr = "";
    for (int i = 0; i < n; ++i)
        ninesDecStr += char('0' + (9 - (decStr[i] - '0')));
    res.ninesComp_dec = ninesDecStr;
    res.ninesComp_bcd = decToBCD(ninesDecStr);
    double ninesVal   = toDecimal(ninesDecStr, 10);
    res.ninesComp_bin = fromDecimal(ninesVal, 2);
    res.ninesComp_oct = fromDecimal(ninesVal, 8);
    res.ninesComp_hex = fromDecimal(ninesVal, 16);

    // 10's complement: 9's complement + 1 (carry through from right)
    string tensDigits = ninesDecStr;
    int carry = 1;
    for (int i = n - 1; i >= 0 && carry; --i)
    {
        int s = (tensDigits[i] - '0') + carry;
        tensDigits[i] = char('0' + (s % 10));
        carry = s / 10;
    }
    string tensDecStr = carry ? ("1" + tensDigits) : tensDigits;
    size_t nzPos = tensDecStr.find_first_not_of('0');
    tensDecStr = (nzPos == string::npos) ? "0" : tensDecStr.substr(nzPos);

    res.tensComp_dec = tensDecStr;
    res.tensComp_bcd = decToBCD(tensDecStr);
    double tensVal   = toDecimal(tensDecStr, 10);
    res.tensComp_bin = fromDecimal(tensVal, 2);
    res.tensComp_oct = fromDecimal(tensVal, 8);
    res.tensComp_hex = fromDecimal(tensVal, 16);

    return res;
}

// ---------------------------------------------------------------------------
// BCD Addition — digit-by-digit with +6 correction when sum > 9
// ---------------------------------------------------------------------------
BCDAddResult bcdAdd(const InputNumber &A, const InputNumber &B)
{
    BCDAddResult res;
    res.isValid    = false;
    res.finalCarry = 0;

    if (!A.isValid || !B.isValid)
    {
        res.errorMsg = "One or both inputs are invalid.";
        return res;
    }
    if (A.originalStr.find('.') != string::npos ||
        B.originalStr.find('.') != string::npos)
    {
        res.errorMsg = "BCD Addition requires integer inputs (no radix fractions).";
        return res;
    }
    if (A.decimalValue < 0 || B.decimalValue < 0)
    {
        res.errorMsg = "BCD Addition is defined for non-negative integers.";
        return res;
    }

    res.A_dec = A.decStr;
    res.B_dec = B.decStr;
    res.A_bcd = decToBCD(A.decStr);
    res.B_bcd = decToBCD(B.decStr);

    // Pad to equal length
    string da = A.decStr, db = B.decStr;
    while (da.size() < db.size()) da = "0" + da;
    while (db.size() < da.size()) db = "0" + db;
    int nDigits = static_cast<int>(da.size());

    int carry = 0;
    string resultDigits = "";
    for (int i = nDigits - 1; i >= 0; --i)
    {
        BCDDigitStep step;
        step.digitA  = da[i] - '0';
        step.digitB  = db[i] - '0';
        step.carryIn = carry;
        step.rawSum  = step.digitA + step.digitB + step.carryIn;

        // 4-bit (or 5-bit if > 15) binary of rawSum
        string rb = "";
        if (step.rawSum > 15)
        {
            for (int b = 4; b >= 0; --b)
                rb += char('0' + ((step.rawSum >> b) & 1));
        }
        else
        {
            for (int b = 3; b >= 0; --b)
                rb += char('0' + ((step.rawSum >> b) & 1));
        }
        step.rawBin = rb;

        step.needCorrect = (step.rawSum > 9);
        if (step.needCorrect)
        {
            step.corrected   = step.rawSum + 6;
            step.carryOut    = step.corrected / 10;
            step.bcdDigitOut = step.corrected % 10;
        }
        else
        {
            step.corrected   = step.rawSum;
            step.carryOut    = 0;
            step.bcdDigitOut = step.rawSum;
        }

        string cb = "";
        for (int b = 3; b >= 0; --b)
            cb += char('0' + ((step.bcdDigitOut >> b) & 1));
        step.corrBin = cb;

        carry         = step.carryOut;
        resultDigits  = char('0' + step.bcdDigitOut) + resultDigits;
        res.steps.push_back(step);
    }

    res.finalCarry = carry;
    if (carry) resultDigits = char('0' + carry) + resultDigits;

    size_t nzp = resultDigits.find_first_not_of('0');
    resultDigits = (nzp == string::npos) ? "0" : resultDigits.substr(nzp);

    res.result_dec = resultDigits;
    res.result_bcd = decToBCD(resultDigits);
    double rv      = toDecimal(resultDigits, 10);
    res.result_bin = fromDecimal(rv, 2);
    res.result_oct = fromDecimal(rv, 8);
    res.result_hex = fromDecimal(rv, 16);
    res.isValid    = true;
    return res;
}

// ---------------------------------------------------------------------------
// BCD Subtraction — 9's complement (end-around carry) or 10's (discard carry)
// ---------------------------------------------------------------------------
BCDSubResult bcdSubtract(const InputNumber &A, const InputNumber &B, bool useTens)
{
    BCDSubResult res;
    res.isValid      = false;
    res.useTens      = useTens;
    res.hasCarry     = false;
    res.isNegative   = false;
    res.decimalResult = 0.0;

    if (!A.isValid || !B.isValid)
    {
        res.errorMsg = "One or both inputs are invalid.";
        return res;
    }
    if (A.originalStr.find('.') != string::npos ||
        B.originalStr.find('.') != string::npos)
    {
        res.errorMsg = "BCD subtraction requires integer inputs (no radix fractions).";
        return res;
    }
    if (A.decimalValue < 0 || B.decimalValue < 0)
    {
        res.errorMsg = "BCD subtraction is defined for non-negative integers.";
        return res;
    }

    res.A_dec = A.decStr;
    res.B_dec = B.decStr;
    res.A_bcd = decToBCD(A.decStr);
    res.B_bcd = decToBCD(B.decStr);

    // Pad to equal length
    string da = A.decStr, db = B.decStr;
    while (da.size() < db.size()) da = "0" + da;
    while (db.size() < da.size()) db = "0" + db;
    int nDigits = static_cast<int>(da.size());

    // Build complement of B digit-by-digit
    string compDigits = "";
    for (int i = 0; i < nDigits; ++i)
        compDigits += char('0' + (9 - (db[i] - '0')));
    if (useTens)
    {
        // 10's complement: 9's comp + 1 with carry
        int c = 1;
        for (int i = nDigits - 1; i >= 0 && c; --i)
        {
            int s = (compDigits[i] - '0') + c;
            compDigits[i] = char('0' + (s % 10));
            c = s / 10;
        }
    }
    res.comp_bcd = decToBCD(compDigits);

    // Add A + comp(B) digit-by-digit with BCD correction
    int carry = 0;
    string resultDigits = "";
    for (int i = nDigits - 1; i >= 0; --i)
    {
        BCDSubDigitStep step;
        step.digitA    = da[i] - '0';
        step.compDigit = compDigits[i] - '0';
        step.carryIn   = carry;
        step.rawSum    = step.digitA + step.compDigit + step.carryIn;
        step.needCorrect = (step.rawSum > 9);
        if (step.needCorrect)
        {
            step.corrected   = step.rawSum + 6;
            step.carryOut    = step.corrected / 10;
            step.bcdDigitOut = step.corrected % 10;
        }
        else
        {
            step.corrected   = step.rawSum;
            step.carryOut    = 0;
            step.bcdDigitOut = step.rawSum;
        }
        carry        = step.carryOut;
        resultDigits = char('0' + step.bcdDigitOut) + resultDigits;
        res.steps.push_back(step);
    }
    res.hasCarry = (carry != 0);

    if (!useTens)
    {
        // 9's complement method
        if (res.hasCarry)
        {
            // End-around carry: add 1 to LSDigit with BCD correction
            int eac = 1;
            for (int i = nDigits - 1; i >= 0 && eac; --i)
            {
                int s = (resultDigits[i] - '0') + eac;
                if (s > 9) { s += 6; eac = s / 10; resultDigits[i] = char('0' + (s % 10)); }
                else       { resultDigits[i] = char('0' + s); eac = 0; }
            }
            res.isNegative   = false;
            res.adjusted_bcd = decToBCD(resultDigits);
        }
        else
        {
            // Negative: take 9's complement of raw sum to get magnitude
            for (size_t i = 0; i < resultDigits.size(); ++i)
                resultDigits[i] = char('0' + (9 - (resultDigits[i] - '0')));
            res.isNegative   = true;
            res.adjusted_bcd = decToBCD(resultDigits);
        }
    }
    else
    {
        // 10's complement method
        if (res.hasCarry)
        {
            res.isNegative   = false;
            res.adjusted_bcd = decToBCD(resultDigits);
        }
        else
        {
            // Negative: take 10's complement of raw sum to get magnitude
            for (size_t i = 0; i < resultDigits.size(); ++i)
                resultDigits[i] = char('0' + (9 - (resultDigits[i] - '0')));
            int c10 = 1;
            for (int i = nDigits - 1; i >= 0 && c10; --i)
            {
                int s = (resultDigits[i] - '0') + c10;
                resultDigits[i] = char('0' + (s % 10));
                c10 = s / 10;
            }
            res.isNegative   = true;
            res.adjusted_bcd = decToBCD(resultDigits);
        }
    }

    size_t nzp = resultDigits.find_first_not_of('0');
    resultDigits = (nzp == string::npos) ? "0" : resultDigits.substr(nzp);

    res.result_dec    = resultDigits;
    res.result_bcd    = decToBCD(resultDigits);
    res.decimalResult = res.isNegative ? -toDecimal(resultDigits, 10)
                                       :  toDecimal(resultDigits, 10);
    res.result_bin    = fromDecimal(res.decimalResult, 2);
    res.result_oct    = fromDecimal(res.decimalResult, 8);
    res.result_hex    = fromDecimal(res.decimalResult, 16);
    res.isValid       = true;
    return res;
}
