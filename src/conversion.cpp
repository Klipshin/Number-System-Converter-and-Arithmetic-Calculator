// conversion.cpp — Input validation, base-N <-> decimal conversion

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>

#include "../include/globals.h"
#include "../include/prototypes.h"

using namespace std;

string getBaseName(int base)
{
    switch (base)
    {
    case 2:  return "Binary (Base 2)";
    case 8:  return "Octal (Base 8)";
    case 10: return "Decimal (Base 10)";
    case 16: return "Hexadecimal (Base 16)";
    default: return "Unknown Base";
    }
}

string getBaseCode(int base)
{
    switch (base)
    {
    case 2:  return "BIN";
    case 8:  return "OCT";
    case 10: return "DEC";
    case 16: return "HEX";
    default: return "UNK";
    }
}

bool validateInput(const string &inputStr, int base, string &errorMsg)
{
    if (inputStr.empty())
    {
        errorMsg = "Input cannot be empty.";
        return false;
    }

    int dotCount = 0;
    for (size_t i = 0; i < inputStr.length(); ++i)
    {
        char c = inputStr[i];
        if (c == '.')
        {
            dotCount++;
            if (dotCount > 1)
            {
                errorMsg = "Multiple radix points (.) are not allowed.";
                return false;
            }
            continue;
        }
        char upperC  = toupper(static_cast<unsigned char>(c));
        size_t digitVal = DIGITS.find(upperC);
        if (digitVal == string::npos || digitVal >= static_cast<size_t>(base))
        {
            stringstream ss;
            ss << "Invalid character '" << c << "' for " << getBaseName(base) << ".";
            errorMsg = ss.str();
            return false;
        }
    }

    if (inputStr == ".")
    {
        errorMsg = "Input cannot be just a radix point.";
        return false;
    }
    return true;
}

double toDecimal(const string &inputStr, int base)
{
    string str = inputStr;
    for (char &c : str) c = toupper(static_cast<unsigned char>(c));

    size_t dotPos  = str.find('.');
    string intPart = (dotPos == string::npos) ? str : str.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : str.substr(dotPos + 1);
    if (intPart.empty()) intPart = "0";

    double decValue = 0.0;
    int intLen = static_cast<int>(intPart.length());
    for (int i = 0; i < intLen; ++i)
    {
        int digit = static_cast<int>(DIGITS.find(intPart[i]));
        int power = intLen - 1 - i;
        decValue += digit * pow(base, power);
    }
    for (size_t j = 0; j < fracPart.length(); ++j)
    {
        int digit = static_cast<int>(DIGITS.find(fracPart[j]));
        int power = -(static_cast<int>(j) + 1);
        decValue += digit * pow(base, power);
    }
    return decValue;
}

string fromDecimal(double decVal, int targetBase, int maxFrac)
{
    if (decVal == 0.0) return "0";

    bool isNegative = decVal < 0;
    double absVal   = fabs(decVal);
    long long intPart = static_cast<long long>(floor(absVal));
    double fracPart   = absVal - intPart;

    string intResult = "";
    if (intPart == 0)
        intResult = "0";
    else
    {
        long long temp = intPart;
        while (temp > 0)
        {
            int rem    = static_cast<int>(temp % targetBase);
            intResult  = DIGITS[rem] + intResult;
            temp      /= targetBase;
        }
    }

    string fracResult = "";
    if (fracPart > 1e-9)
    {
        double tempFrac = fracPart;
        int count = 0;
        while (tempFrac > 1e-9 && count < maxFrac)
        {
            tempFrac *= targetBase;
            int digit = static_cast<int>(floor(tempFrac + 1e-12));
            if (digit >= targetBase) digit = targetBase - 1;
            fracResult += DIGITS[digit];
            tempFrac   -= digit;
            count++;
        }
    }

    if (isNegative) intResult = "-" + intResult;
    return fracResult.empty() ? intResult : intResult + "." + fracResult;
}

string formatDecimalValue(double value)
{
    if (fabs(value) < 1e-9) value = 0.0;
    stringstream ss;
    if (fabs(value - round(value)) < 1e-9)
        ss << static_cast<long long>(round(value));
    else
    {
        ss << fixed << setprecision(6) << value;
        string result = ss.str();
        while (!result.empty() && result.back() == '0') result.pop_back();
        if (!result.empty() && result.back() == '.') result.pop_back();
        return result;
    }
    return ss.str();
}

InputNumber processConversion(const string &inputStr, int base)
{
    InputNumber num;
    num.originalStr = inputStr;
    num.base        = base;

    string errorMsg;
    num.isValid = validateInput(inputStr, base, errorMsg);
    if (!num.isValid)
    {
        num.decimalValue = 0.0;
        num.binStr = num.octStr = num.decStr = num.hexStr = "INVALID";
        return num;
    }

    num.decimalValue = toDecimal(inputStr, base);
    num.binStr = fromDecimal(num.decimalValue, 2);
    num.octStr = fromDecimal(num.decimalValue, 8);
    num.decStr = formatDecimalValue(num.decimalValue);
    num.hexStr = fromDecimal(num.decimalValue, 16);
    return num;
}
