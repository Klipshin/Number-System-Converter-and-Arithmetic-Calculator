// display.cpp -- All print* display functions, complement submenu, interactive
//                converter runner, preset combinations, and custom expression prompt

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <algorithm>

#include "../include/globals.h"
#include "../include/prototypes.h"

using namespace std;
void printBCDComplementTable(const vector<InputNumber> &numbers)
{
    cout << "\n";
    printDivider('=', CLR_CYAN);
    printCentered(CLR_WHITE + "9'S COMPLEMENT  &  10'S COMPLEMENT TABLE (BCD)" + CLR_RESET);
    printDivider('=', CLR_CYAN);
    cout << "\n";

    for (size_t i = 0; i < numbers.size(); ++i)
    {
        const InputNumber &num = numbers[i];
        BCDComplementResult comp = computeBCDComplements(num);

        string varLabel = string(1, char('a' + i));
        printCentered(CLR_YELLOW + "--- Input #" + to_string(i + 1) + "  [" + varLabel +
                      " = " + num.originalStr + " " + getBaseCode(num.base) + "] ---" + CLR_RESET);
        cout << "\n";

        if (!comp.applicable)
        {
            printCentered(CLR_RED + "  Note: " + comp.note + CLR_RESET);
            cout << "\n";
            continue;
        }

        printCentered(CLR_GRAY + "  Decimal Value : " + CLR_WHITE + comp.original_dec + CLR_RESET);
        printCentered(CLR_GRAY + "  BCD Encoding  : " + CLR_CYAN  + comp.original_bcd + CLR_RESET);
        cout << "\n";

        // 9's complement
        printCentered(CLR_GREEN + "  [ 9's Complement  —  each digit d -> (9 - d) ]" + CLR_RESET);
        {
            stringstream ss;
            ss << left
               << CLR_MAGENTA << setw(22) << ("  DEC: " + comp.ninesComp_dec)
               << CLR_CYAN    << setw(30) << ("BCD: " + comp.ninesComp_bcd)
               << CLR_RESET;
            printCentered(ss.str());
        }
        {
            stringstream ss;
            ss << left
               << CLR_CYAN   << setw(26) << ("  BIN: " + comp.ninesComp_bin)
               << CLR_YELLOW << setw(18) << ("OCT: " + comp.ninesComp_oct)
               << CLR_GREEN  << setw(14) << ("HEX: " + comp.ninesComp_hex)
               << CLR_RESET;
            printCentered(ss.str());
        }
        cout << "\n";

        // 10's complement
        printCentered(CLR_YELLOW + "  [ 10's Complement  —  9's comp + 1 ]" + CLR_RESET);
        {
            stringstream ss;
            ss << left
               << CLR_MAGENTA << setw(22) << ("  DEC: " + comp.tensComp_dec)
               << CLR_CYAN    << setw(30) << ("BCD: " + comp.tensComp_bcd)
               << CLR_RESET;
            printCentered(ss.str());
        }
        {
            stringstream ss;
            ss << left
               << CLR_CYAN   << setw(26) << ("  BIN: " + comp.tensComp_bin)
               << CLR_YELLOW << setw(18) << ("OCT: " + comp.tensComp_oct)
               << CLR_GREEN  << setw(14) << ("HEX: " + comp.tensComp_hex)
               << CLR_RESET;
            printCentered(ss.str());
        }
        cout << "\n";
        printDivider('-', CLR_GRAY);
        cout << "\n";
    }
    printDivider('=', CLR_CYAN);
    cout << "\n";
}

void printBCDAddResult(const BCDAddResult &result)
{
    cout << "\n";
    printDivider('=', CLR_GREEN);
    printCentered(CLR_WHITE + "BCD ADDITION" + CLR_RESET);
    printCentered(CLR_GRAY + "  A = " + CLR_WHITE + result.A_dec +
                  CLR_GRAY + "   B = " + CLR_WHITE + result.B_dec + "  (Decimal)" + CLR_RESET);
    printDivider('=', CLR_GREEN);

    if (!result.isValid)
    {
        cout << "\n";
        printCentered(CLR_RED + "[!] ERROR: " + result.errorMsg + CLR_RESET);
        printDivider('=', CLR_GREEN);
        cout << "\n";
        return;
    }

    cout << "\n";
    printCentered(CLR_CYAN  + "  A BCD : " + CLR_WHITE + result.A_bcd + CLR_RESET);
    printCentered(CLR_CYAN  + "  B BCD : " + CLR_WHITE + result.B_bcd + CLR_RESET);
    cout << "\n";
    printDivider('-', CLR_GRAY);
    printCentered(CLR_YELLOW + "Step-by-Step BCD Addition  (LSDigit -> MSDigit)" + CLR_RESET);
    printDivider('-', CLR_GRAY);

    int nSteps = static_cast<int>(result.steps.size());
    for (int s = nSteps - 1; s >= 0; --s)   // display MSDigit first
    {
        const BCDDigitStep &st = result.steps[s];
        int pos = nSteps - 1 - s;            // digit position from right (0 = LSDigit)
        cout << "\n";
        printCentered(CLR_CYAN + "Digit position " + to_string(pos) +
                      " (from right)" + CLR_RESET);
        printCentered(CLR_GRAY + "  A digit : " + CLR_WHITE + to_string(st.digitA) + CLR_RESET);
        printCentered(CLR_GRAY + "  B digit : " + CLR_WHITE + to_string(st.digitB) + CLR_RESET);
        printCentered(CLR_GRAY + "  Carry-in: " + CLR_WHITE + to_string(st.carryIn) + CLR_RESET);
        printCentered(CLR_GRAY + "  Raw sum  : " + CLR_WHITE + to_string(st.digitA) + " + " +
                      to_string(st.digitB) + " + " + to_string(st.carryIn) +
                      " = " + to_string(st.rawSum) +
                      CLR_GRAY + "  (" + st.rawBin + " in binary)" + CLR_RESET);
        if (st.needCorrect)
        {
            printCentered(CLR_YELLOW + "  > Sum > 9 -> Apply BCD correction (+6)" + CLR_RESET);
            printCentered(CLR_GRAY + "  Corrected: " + to_string(st.rawSum) + " + 6 = " +
                          CLR_GREEN + to_string(st.corrected) + CLR_RESET);
            printCentered(CLR_GRAY + "  BCD digit : " +
                          CLR_GREEN + to_string(st.bcdDigitOut) +
                          CLR_GRAY + "   Carry-out: " +
                          CLR_YELLOW + to_string(st.carryOut) + CLR_RESET);
        }
        else
        {
            printCentered(CLR_GRAY + "  Sum <= 9  -> No correction needed." + CLR_RESET);
            printCentered(CLR_GRAY + "  BCD digit : " +
                          CLR_GREEN + to_string(st.bcdDigitOut) +
                          CLR_GRAY + "   Carry-out: " +
                          CLR_WHITE + "0" + CLR_RESET);
        }
    }
    cout << "\n";
    if (result.finalCarry)
        printCentered(CLR_YELLOW + "  Final carry-out: " + to_string(result.finalCarry) +
                      "  (overflow digit prepended)" + CLR_RESET);

    printDivider('-', CLR_GRAY);
    printCentered(CLR_YELLOW + "RESULT: A + B = " + result.result_dec + CLR_RESET);
    printCentered(CLR_CYAN   + "BCD  : " + result.result_bcd + CLR_RESET);
    cout << "\n";

    stringstream hdrSS;
    hdrSS << left
          << setw(22) << "Binary (Base 2)"
          << setw(18) << "Octal (Base 8)"
          << setw(18) << "Decimal (10)"
          << setw(18) << "Hexadecimal (16)";
    printCentered(CLR_YELLOW + "RESULT IN ALL BASES:" + CLR_RESET);
    printCentered(CLR_YELLOW + hdrSS.str() + CLR_RESET);
    printDivider('-', CLR_GRAY);
    stringstream rowSS;
    rowSS << left
          << CLR_CYAN    << setw(22) << result.result_bin
          << CLR_YELLOW  << setw(18) << result.result_oct
          << CLR_MAGENTA << setw(18) << result.result_dec
          << CLR_GREEN   << setw(18) << result.result_hex
          << CLR_RESET;
    printCentered(rowSS.str());
    printDivider('=', CLR_GREEN);
    cout << "\n";
}

void printBCDSubResult(const BCDSubResult &result,
                       const InputNumber &A,
                       const InputNumber &B)
{
    string methodName = result.useTens ? "10's Complement" : "9's Complement";
    string compLabel  = result.useTens ? "10's comp(B)"    : "9's comp(B)";

    cout << "\n";
    printDivider('=', CLR_MAGENTA);
    printCentered(CLR_WHITE + "BCD SUBTRACTION USING " + methodName + CLR_RESET);
    printCentered(CLR_GRAY + "  A = " + CLR_WHITE + result.A_dec +
                  CLR_GRAY + "   B = " + CLR_WHITE + result.B_dec + "  (Decimal)" + CLR_RESET);
    printDivider('=', CLR_MAGENTA);

    if (!result.isValid)
    {
        cout << "\n";
        printCentered(CLR_RED + "[!] ERROR: " + result.errorMsg + CLR_RESET);
        printDivider('=', CLR_MAGENTA);
        cout << "\n";
        return;
    }

    cout << "\n";
    printCentered(CLR_YELLOW + "Step-by-Step BCD Subtraction  A - B" + CLR_RESET);
    cout << "\n";

    // Step 1: BCD encodings
    printCentered(CLR_CYAN + "Step 1: BCD Representations" + CLR_RESET);
    printCentered(CLR_GRAY + "  A BCD : " + CLR_WHITE + result.A_bcd + CLR_RESET);
    printCentered(CLR_GRAY + "  B BCD : " + CLR_WHITE + result.B_bcd + CLR_RESET);
    cout << "\n";

    // Step 2: Complement of B
    printCentered(CLR_CYAN + "Step 2: Compute " + compLabel + CLR_RESET);
    if (!result.useTens)
        printCentered(CLR_GRAY + "  Each digit d of B -> (9 - d)" + CLR_RESET);
    else
        printCentered(CLR_GRAY + "  Each digit d of B -> (9 - d),  then add 1 with carry" + CLR_RESET);
    printCentered(CLR_GRAY + "  " + compLabel + " BCD : " + CLR_GREEN + result.comp_bcd + CLR_RESET);
    cout << "\n";

    // Step 3: Digit-by-digit addition
    printCentered(CLR_CYAN + "Step 3: Add  A + " + compLabel + "  (BCD digit by digit)" + CLR_RESET);
    printDivider('-', CLR_GRAY);
    int nSteps = static_cast<int>(result.steps.size());
    for (int s = nSteps - 1; s >= 0; --s)
    {
        const BCDSubDigitStep &st = result.steps[s];
        int pos = nSteps - 1 - s;
        cout << "\n";
        printCentered(CLR_CYAN + "Digit position " + to_string(pos) + CLR_RESET);
        printCentered(CLR_GRAY + "  A digit      : " + CLR_WHITE  + to_string(st.digitA) + CLR_RESET);
        printCentered(CLR_GRAY + "  " + compLabel + " digit: " + CLR_GREEN + to_string(st.compDigit) + CLR_RESET);
        printCentered(CLR_GRAY + "  Carry-in     : " + CLR_WHITE  + to_string(st.carryIn) + CLR_RESET);
        printCentered(CLR_GRAY + "  Raw sum      : " + CLR_WHITE  +
                      to_string(st.digitA) + " + " + to_string(st.compDigit) +
                      " + " + to_string(st.carryIn) + " = " + to_string(st.rawSum) + CLR_RESET);
        if (st.needCorrect)
        {
            printCentered(CLR_YELLOW + "  > Sum > 9 -> BCD correction (+6)" + CLR_RESET);
            printCentered(CLR_GRAY + "  Corrected  : " + to_string(st.rawSum) + " + 6 = " +
                          CLR_GREEN + to_string(st.corrected) + CLR_RESET);
        }
        printCentered(CLR_GRAY + "  BCD digit out: " +
                      CLR_GREEN + to_string(st.bcdDigitOut) +
                      CLR_GRAY + "   Carry-out: " +
                      CLR_YELLOW + to_string(st.carryOut) + CLR_RESET);
    }
    cout << "\n";

    // Step 4: Carry adjustment
    printCentered(CLR_CYAN + "Step 4: Carry Adjustment" + CLR_RESET);
    if (!result.useTens)
    {
        if (result.hasCarry)
        {
            printCentered(CLR_GRAY + "  Carry-out detected -> End-Around Carry: add carry back to LSDigit." + CLR_RESET);
            printCentered(CLR_GRAY + "  Result is POSITIVE." + CLR_RESET);
        }
        else
        {
            printCentered(CLR_GRAY + "  No carry-out -> Result is NEGATIVE." + CLR_RESET);
            printCentered(CLR_GRAY + "  Take 9's complement of raw sum to get magnitude." + CLR_RESET);
        }
    }
    else
    {
        if (result.hasCarry)
        {
            printCentered(CLR_GRAY + "  Carry-out detected -> Discard carry. Result is POSITIVE." + CLR_RESET);
        }
        else
        {
            printCentered(CLR_GRAY + "  No carry-out -> Result is NEGATIVE." + CLR_RESET);
            printCentered(CLR_GRAY + "  Take 10's complement of raw sum to get magnitude." + CLR_RESET);
        }
    }
    printCentered(CLR_GRAY + "  Adjusted BCD: " + CLR_GREEN + result.adjusted_bcd + CLR_RESET);
    cout << "\n";

    // Final result
    printDivider('-', CLR_GRAY);
    string signStr = result.isNegative ? "-" : "";
    printCentered(CLR_YELLOW + "RESULT: A - B = " + signStr + result.result_dec + CLR_RESET);
    if (result.isNegative)
        printCentered(CLR_RED + "  (Negative result)" + CLR_RESET);
    printCentered(CLR_CYAN + "BCD  : " + (result.isNegative ? "-" : "") + result.result_bcd + CLR_RESET);
    cout << "\n";

    stringstream hdrSS;
    hdrSS << left
          << setw(22) << "Binary (Base 2)"
          << setw(18) << "Octal (Base 8)"
          << setw(18) << "Decimal (10)"
          << setw(18) << "Hexadecimal (16)";
    printCentered(CLR_YELLOW + "RESULT IN ALL BASES:" + CLR_RESET);
    printCentered(CLR_YELLOW + hdrSS.str() + CLR_RESET);
    printDivider('-', CLR_GRAY);
    string dispBin = (result.isNegative ? "-" : "") + result.result_bin;
    stringstream rowSS;
    rowSS << left
          << CLR_CYAN    << setw(22) << dispBin
          << CLR_YELLOW  << setw(18) << result.result_oct
          << CLR_MAGENTA << setw(18) << (signStr + result.result_dec)
          << CLR_GREEN   << setw(18) << result.result_hex
          << CLR_RESET;
    printCentered(rowSS.str());
    printDivider('=', CLR_MAGENTA);
    cout << "\n";
}

void printResultsTable(const vector<InputNumber> &numbers)
{
    cout << "\n";
    printDivider('=', CLR_CYAN);
    printCentered(CLR_WHITE + "CONVERSION RESULTS TABLE" + CLR_RESET);
    printDivider('=', CLR_CYAN);

    // Header row
    stringstream headerSS;
    headerSS << left
             << setw(4) << "#"
             << setw(15) << "Original Input"
             << setw(8) << "Base"
             << setw(20) << "Binary (Base 2)"
             << setw(16) << "Octal (Base 8)"
             << setw(14) << "Decimal (10)"
             << setw(9) << "Hex (16)";

    printCentered(CLR_YELLOW + headerSS.str() + CLR_RESET);
    printDivider('-', CLR_GRAY);

    for (size_t i = 0; i < numbers.size(); ++i)
    {
        const auto &num = numbers[i];
        string baseColor = getBaseColor(num.base);

        stringstream coloredRow;
        coloredRow << left
                   << CLR_GRAY << setw(4) << (i + 1)
                   << CLR_WHITE << setw(15) << num.originalStr
                   << baseColor << setw(8) << getBaseCode(num.base)
                   << CLR_CYAN << setw(20) << num.binStr
                   << CLR_YELLOW << setw(16) << num.octStr
                   << CLR_MAGENTA << setw(14) << num.decStr
                   << CLR_GREEN << setw(9) << num.hexStr
                   << CLR_RESET;

        printCentered(coloredRow.str());
    }
    printDivider('=', CLR_CYAN);
    cout << "\n";
}

void printComplementTable(const vector<InputNumber> &numbers)
{
    cout << "\n";
    printDivider('=', CLR_MAGENTA);
    printCentered(CLR_WHITE + "1'S COMPLEMENT  &  2'S COMPLEMENT TABLE" + CLR_RESET);
    printDivider('=', CLR_MAGENTA);
    cout << "\n";

    for (size_t i = 0; i < numbers.size(); ++i)
    {
        const InputNumber &num = numbers[i];
        ComplementResult comp = computeComplements(num);

        string varLabel = string(1, char('a' + i));
        printCentered(CLR_YELLOW + "--- Input #" + to_string(i + 1) + "  [" + varLabel + " = " + num.originalStr + " " + getBaseCode(num.base) + "] ---" + CLR_RESET);
        cout << "\n";

        if (!comp.applicable)
        {
            printCentered(CLR_RED + "  Note: " + comp.note + CLR_RESET);
            cout << "\n";
            continue;
        }

        // Original + padded binary
        printCentered(CLR_GRAY + "  Bit Width  : " + CLR_WHITE + to_string(comp.bitWidth) + " bits" + CLR_RESET);
        printCentered(CLR_GRAY + "  Binary     : " + CLR_CYAN + comp.paddedBin + CLR_RESET);
        cout << "\n";

        // 1's complement row
        printCentered(CLR_GREEN + "  [ 1's Complement ]" + CLR_RESET);
        {
            stringstream ss;
            ss << left
               << CLR_CYAN << setw(26) << ("  BIN: " + comp.onesComp_bin)
               << CLR_YELLOW << setw(18) << ("OCT: " + comp.onesComp_oct)
               << CLR_MAGENTA << setw(16) << ("DEC: " + comp.onesComp_dec)
               << CLR_GREEN << setw(14) << ("HEX: " + comp.onesComp_hex)
               << CLR_RESET;
            printCentered(ss.str());
        }
        cout << "\n";

        // 2's complement row
        printCentered(CLR_CYAN + "  [ 2's Complement ]" + CLR_RESET);
        {
            stringstream ss;
            ss << left
               << CLR_CYAN << setw(26) << ("  BIN: " + comp.twosComp_bin)
               << CLR_YELLOW << setw(18) << ("OCT: " + comp.twosComp_oct)
               << CLR_MAGENTA << setw(16) << ("DEC: " + comp.twosComp_dec)
               << CLR_GREEN << setw(14) << ("HEX: " + comp.twosComp_hex)
               << CLR_RESET;
            printCentered(ss.str());
        }
        cout << "\n";
        printDivider('-', CLR_GRAY);
        cout << "\n";
    }
    printDivider('=', CLR_MAGENTA);
    cout << "\n";
}

void printCompSubResult(const CompSubResult &result,
                        const InputNumber &A,
                        const InputNumber &B)
{
    string methodName = result.useTwos ? "2's Complement" : "1's Complement";
    string compLabel = result.useTwos ? "2's comp(B)" : "1's comp(B)";

    cout << "\n";
    printDivider('=', CLR_MAGENTA);
    printCentered(CLR_WHITE + "SUBTRACTION USING " + methodName + CLR_RESET);
    printCentered(CLR_GRAY + "  A = " + CLR_WHITE + A.originalStr + " [" + getBaseCode(A.base) + "]" +
                  CLR_GRAY + "   B = " + CLR_WHITE + B.originalStr + " [" + getBaseCode(B.base) + "]" + CLR_RESET);
    printDivider('=', CLR_MAGENTA);

    if (!result.isValid)
    {
        cout << "\n";
        printCentered(CLR_RED + "[!] ERROR: " + result.errorMsg + CLR_RESET);
        printDivider('=', CLR_MAGENTA);
        cout << "\n";
        return;
    }

    cout << "\n";
    printCentered(CLR_YELLOW + "Step-by-Step Complement Subtraction ( A - B )" + CLR_RESET);
    cout << "\n";

    // Step 1: Show both operands in binary
    printCentered(CLR_CYAN + "Step 1: Binary representations (" + to_string(result.bitWidth) + "-bit padded)" + CLR_RESET);
    printCentered(CLR_GRAY + "  A = " + CLR_WHITE + A.decStr + CLR_GRAY + " -> BIN: " + CLR_CYAN + result.A_bin + CLR_RESET);
    printCentered(CLR_GRAY + "  B = " + CLR_WHITE + B.decStr + CLR_GRAY + " -> BIN: " + CLR_CYAN + result.B_bin + CLR_RESET);
    cout << "\n";

    // Step 2: Complement of B
    printCentered(CLR_CYAN + "Step 2: Compute " + compLabel + CLR_RESET);
    if (result.useTwos)
    {
        string onesOfB = onesComplementBin(result.B_bin);
        printCentered(CLR_GRAY + "  1's comp(B) = " + CLR_YELLOW + onesOfB + CLR_RESET);
        printCentered(CLR_GRAY + "  Add 1       = " + CLR_GREEN + result.comp_bin + CLR_GRAY + "  <-- 2's complement of B" + CLR_RESET);
    }
    else
    {
        printCentered(CLR_GRAY + "  Flip all bits of B: " + CLR_GREEN + result.comp_bin + CLR_GRAY + "  <-- 1's complement of B" + CLR_RESET);
    }
    cout << "\n";

    // Step 3: Add A + comp(B)
    printCentered(CLR_CYAN + "Step 3: Add  A + " + compLabel + CLR_RESET);
    cout << "\n";
    // visual alignment
    int pad = (TERMINAL_WIDTH - static_cast<int>(result.bitWidth) - 8) / 2;
    string indent(max(0, pad), ' ');
    cout << indent << "    " << CLR_CYAN << result.A_bin << CLR_RESET << "\n";
    cout << indent << "  + " << CLR_GREEN << result.comp_bin << CLR_RESET << "\n";
    cout << indent << "    " << string(result.bitWidth, '-') << "\n";
    cout << indent << "    " << CLR_WHITE << result.sum_bin << CLR_RESET;
    if (result.hasCarry)
        cout << CLR_YELLOW << "  <-- carry-out" << CLR_RESET;
    cout << "\n\n";

    // Step 4: Carry adjustment
    printCentered(CLR_CYAN + "Step 4: Carry Adjustment" + CLR_RESET);
    if (result.useTwos)
    {
        if (result.hasCarry)
            printCentered(CLR_GRAY + "  Carry-out detected -> Discard carry. Result is POSITIVE." + CLR_RESET);
        else
            printCentered(CLR_GRAY + "  No carry-out -> Result is NEGATIVE." + CLR_RESET);
    }
    else
    {
        if (result.hasCarry)
        {
            printCentered(CLR_GRAY + "  Carry-out detected -> End-Around Carry: add carry back to sum." + CLR_RESET);
            printCentered(CLR_GRAY + "  " + result.sum_bin + " + 1 = " + CLR_GREEN + result.adjusted_bin + CLR_RESET + "  (positive)");
        }
        else
        {
            printCentered(CLR_GRAY + "  No carry-out -> Result is NEGATIVE." + CLR_RESET);
            printCentered(CLR_GRAY + "  Take 1's complement of sum to get magnitude: " + CLR_GREEN + result.adjusted_bin + CLR_RESET);
        }
    }
    cout << "\n";

    // Final result
    printDivider('-', CLR_GRAY);
    printCentered(CLR_YELLOW + "RESULT: A - B = " + result.decStr + CLR_RESET);
    if (result.isNegative)
        printCentered(CLR_RED + "  (Negative result)" + CLR_RESET);
    cout << "\n";

    // All-bases result table
    stringstream headerSS;
    headerSS << left
             << setw(26) << "Binary (Base 2)"
             << setw(18) << "Octal (Base 8)"
             << setw(18) << "Decimal (10)"
             << setw(18) << "Hexadecimal (16)";
    printCentered(CLR_YELLOW + "RESULT IN ALL BASES:" + CLR_RESET);
    printCentered(CLR_YELLOW + headerSS.str() + CLR_RESET);
    printDivider('-', CLR_GRAY);

    stringstream rowSS;
    rowSS << left
          << CLR_CYAN << setw(26) << result.binStr
          << CLR_YELLOW << setw(18) << result.octStr
          << CLR_MAGENTA << setw(18) << result.decStr
          << CLR_GREEN << setw(18) << result.hexStr
          << CLR_RESET;
    printCentered(rowSS.str());
    printDivider('=', CLR_MAGENTA);
    cout << "\n";
}

void printArithmeticResult(const ArithmeticResult &result)
{
    cout << "\n";
    printDivider('=', CLR_GREEN);
    printCentered(CLR_WHITE + "ARITHMETIC CALCULATOR RESULT" + CLR_RESET);
    printDivider('=', CLR_GREEN);

    // --- Operation label ---
    printCentered("Operation : " + CLR_YELLOW + result.operationName + " (" + result.operationSymbol + ")" + CLR_RESET);
    cout << "\n";

    // --- Original expression (mixed-base) ---
    printCentered(CLR_GRAY + "  Original (mixed bases)  : " + CLR_WHITE + result.expression + CLR_RESET);

    if (!result.isValid)
    {
        cout << "\n";
        printCentered(CLR_RED + "[!] ERROR: " + result.errorMsg + CLR_RESET);
        printDivider('=', CLR_GREEN);
        cout << "\n";
        return;
    }

    // --- Decimal equivalent expression ---
    printCentered(CLR_GRAY + "  Decimal equivalent      : " + CLR_MAGENTA + result.decimalExpr + CLR_RESET);
    printCentered(CLR_GRAY + "  (All values converted to Decimal before operation)" + CLR_RESET);
    cout << "\n";
    printDivider('-', CLR_GRAY);

    // --- Result table ---
    stringstream headerSS;
    headerSS << left
             << setw(22) << "Binary (Base 2)"
             << setw(18) << "Octal (Base 8)"
             << setw(18) << "Decimal (10)"
             << setw(18) << "Hexadecimal (16)";
    printCentered(CLR_YELLOW + "RESULT IN ALL BASES:" + CLR_RESET);
    printCentered(CLR_YELLOW + headerSS.str() + CLR_RESET);
    printDivider('-', CLR_GRAY);

    stringstream rowSS;
    rowSS << left
          << CLR_CYAN << setw(22) << result.binStr
          << CLR_YELLOW << setw(18) << result.octStr
          << CLR_MAGENTA << setw(18) << result.decStr
          << CLR_GREEN << setw(18) << result.hexStr
          << CLR_RESET;
    printCentered(rowSS.str());
    printDivider('=', CLR_GREEN);
    cout << "\n";
}

void printCustomExprResult(const CustomExprResult &result)
{
    cout << "\n";
    printDivider('=', CLR_GREEN);
    printCentered(CLR_WHITE + "CUSTOM EXPRESSION RESULT" + CLR_RESET);
    printDivider('=', CLR_GREEN);

    // Variable legend
    printCentered(CLR_GRAY + "  Expression : " + CLR_WHITE + result.originalExpr + CLR_RESET);
    cout << "\n";
    printCentered(CLR_GRAY + "  Substituted: " + CLR_WHITE + result.substitutedExpr + CLR_RESET);

    if (!result.isValid)
    {
        cout << "\n";
        printCentered(CLR_RED + "  [!] ERROR: " + result.errorMsg + CLR_RESET);
        printDivider('=', CLR_GREEN);
        cout << "\n";
        return;
    }

    printCentered(CLR_GRAY + "  Decimal eq. : " + CLR_MAGENTA + result.decimalExpr + CLR_RESET);
    printCentered(CLR_GRAY + "  (All values converted to Decimal, then expression evaluated)" + CLR_RESET);
    cout << "\n";
    printDivider('-', CLR_GRAY);

    stringstream headerSS;
    headerSS << left
             << setw(22) << "Binary (Base 2)"
             << setw(18) << "Octal (Base 8)"
             << setw(18) << "Decimal (10)"
             << setw(18) << "Hexadecimal (16)";
    printCentered(CLR_YELLOW + "RESULT IN ALL BASES:" + CLR_RESET);
    printCentered(CLR_YELLOW + headerSS.str() + CLR_RESET);
    printDivider('-', CLR_GRAY);

    stringstream rowSS;
    rowSS << left
          << CLR_CYAN << setw(22) << result.binStr
          << CLR_YELLOW << setw(18) << result.octStr
          << CLR_MAGENTA << setw(18) << result.decStr
          << CLR_GREEN << setw(18) << result.hexStr
          << CLR_RESET;
    printCentered(rowSS.str());
    printDivider('=', CLR_GREEN);
    cout << "\n";
}

void printDetailedSteps(const InputNumber &num, int index)
{
    clearScreen();
    cout << "\n";
    printDivider('-', CLR_YELLOW);
    printCentered(CLR_YELLOW + "MATHEMATICAL STEP-BY-STEP PROOF FOR INPUT #" + to_string(index) + CLR_RESET);
    printCentered("Original Value: " + getBaseColor(num.base) + num.originalStr + CLR_RESET + " in " + getBaseName(num.base));
    printDivider('-', CLR_YELLOW);
    cout << "\n";

    printCentered(CLR_CYAN + "Step 1: Conversion to Decimal (Base 10) using Positional Notation" + CLR_RESET);
    printCentered(CLR_GRAY + "Formula: Value = Sum( Digit * Base^Position )" + CLR_RESET);
    cout << "\n";

    string str = num.originalStr;
    for (char &c : str)
        c = toupper(static_cast<unsigned char>(c));
    size_t dotPos = str.find('.');
    string intPart = (dotPos == string::npos) ? str : str.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : str.substr(dotPos + 1);

    stringstream expSS;
    expSS << "= ";
    int intLen = static_cast<int>(intPart.length());
    bool first = true;
    for (int i = 0; i < intLen; ++i)
    {
        if (!first)
            expSS << " + ";
        int power = intLen - 1 - i;
        expSS << "(" << intPart[i] << " * " << num.base << "^" << power << ")";
        first = false;
    }
    for (size_t j = 0; j < fracPart.length(); ++j)
    {
        expSS << " + (" << fracPart[j] << " * " << num.base << "^-" << (j + 1) << ")";
    }
    expSS << " = " << CLR_MAGENTA << num.decStr << " (Decimal)" << CLR_RESET;
    printCentered(expSS.str());
    cout << "\n";

    printCentered(CLR_CYAN + "Step 2: Conversion from Decimal (" + num.decStr + ") to Target Bases" + CLR_RESET);
    printCentered("-> Binary (Base 2):       " + CLR_CYAN + num.binStr + CLR_RESET + " (Base 2)");
    printCentered("-> Octal (Base 8):        " + CLR_YELLOW + num.octStr + CLR_RESET + " (Base 8)");
    printCentered("-> Decimal (Base 10):     " + CLR_MAGENTA + num.decStr + CLR_RESET + " (Base 10)");
    printCentered("-> Hexadecimal (Base 16): " + CLR_GREEN + num.hexStr + CLR_RESET + " (Base 16)");
    printDivider('-', CLR_YELLOW);
    cout << "\n";
}

void runComplementSubmenu(const vector<InputNumber> &numbers, int count)
{
    bool compKeep = true;
    while (compKeep)
    {
        clearScreen();
        displayHeader();
        printResultsTable(numbers);
        printDivider('-', CLR_MAGENTA);
        printCentered(CLR_WHITE + "COMPLEMENT OPERATIONS" + CLR_RESET);
        printCentered(CLR_GRAY + "1's complement: flip all bits   |   2's complement: flip + add 1" + CLR_RESET);
        printDivider('-', CLR_MAGENTA);
        cout << "\n";

        vector<string> compOptions = {
            "View 1's & 2's Complement Table   -- binary complements for all inputs",
            "Subtract using 1's Complement     -- pick A and B  (shows step-by-step)",
            "Subtract using 2's Complement     -- pick A and B  (shows step-by-step)",
            "View 9's & 10's Complement Table  -- BCD complements for all inputs",
            "BCD Addition                      -- pick A and B  (digit-by-digit)",
            "BCD Subtract using 9's Complement -- pick A and B  (end-around carry)",
            "BCD Subtract using 10's Complement-- pick A and B  (discard carry)",
            "Back to Operation Menu"};
        int compChoice = promptScrollableMenu("SELECT COMPLEMENT OPERATION", compOptions);

        if (compChoice == 7)
        {
            break;
        }

        bool usingCompTable     = (compChoice == 0);
        bool usingOnesCompSub   = (compChoice == 1);
        bool usingTwosCompSub   = (compChoice == 2);
        bool usingBCDCompTable  = (compChoice == 3);
        bool usingBCDAdd        = (compChoice == 4);
        bool usingBCDNinesSub   = (compChoice == 5);
        bool usingBCDTensSub    = (compChoice == 6);
        bool usingCompSub       = usingOnesCompSub  || usingTwosCompSub;
        bool usingBCDSub        = usingBCDNinesSub  || usingBCDTensSub;
        bool usingBCDTwoOp      = usingBCDAdd || usingBCDSub;

        CompSubResult compSubResult;
        BCDAddResult  bcdAddResult;
        BCDSubResult  bcdSubResult;
        int compSubA = 0, compSubB = (count > 1) ? 1 : 0;

        // ── Binary complement subtraction picker ──────────────────────────────
        if (usingCompSub)
        {
            clearScreen();
            displayHeader();
            printResultsTable(numbers);
            cout << "\n";
            printDivider('-', CLR_CYAN);
            string methodLabel = usingTwosCompSub ? "2's" : "1's";
            printCentered(CLR_WHITE + "COMPLEMENT SUBTRACTION  ( " + methodLabel + " Complement )" + CLR_RESET);
            printCentered(CLR_GRAY + "Select the MINUEND (A) and SUBTRAHEND (B) to compute  A - B" + CLR_RESET);
            printCentered(CLR_GRAY + "Note: complement subtraction is a strict two-operand method." + CLR_RESET);
            printDivider('-', CLR_CYAN);
            cout << "\n";

            vector<string> pickerOpts;
            for (int i = 0; i < count; ++i)
            {
                string label = "Input #" + to_string(i + 1) + "  [" + string(1, char('a' + i)) + " = " + numbers[i].originalStr + " " + getBaseCode(numbers[i].base) + "]";
                pickerOpts.push_back(label);
            }

            compSubA = promptScrollableMenu("SELECT  A  (Minuend)", pickerOpts, 0);
            int defaultB = (compSubA == 0) ? 1 : 0;
            compSubB = promptScrollableMenu("SELECT  B  (Subtrahend)", pickerOpts, defaultB);
            compSubResult = complementSubtract(numbers[compSubA], numbers[compSubB], usingTwosCompSub);
        }

        // ── BCD two-operand operations picker ────────────────────────────────
        if (usingBCDTwoOp)
        {
            clearScreen();
            displayHeader();
            printResultsTable(numbers);
            cout << "\n";
            printDivider('-', CLR_CYAN);
            string bcdOpLabel = usingBCDAdd ? "BCD Addition" :
                                (usingBCDNinesSub ? "BCD Subtraction (9's Complement)"
                                                  : "BCD Subtraction (10's Complement)");
            printCentered(CLR_WHITE + bcdOpLabel + CLR_RESET);
            printCentered(CLR_GRAY + "Inputs are converted to decimal first, then each digit is a BCD group." + CLR_RESET);
            printCentered(CLR_GRAY + "Select the two operands  A  and  B  (integers only)." + CLR_RESET);
            printDivider('-', CLR_CYAN);
            cout << "\n";

            vector<string> pickerOpts;
            for (int i = 0; i < count; ++i)
            {
                string label = "Input #" + to_string(i + 1) + "  [" + string(1, char('a' + i)) + " = " + numbers[i].originalStr + " " + getBaseCode(numbers[i].base) + "  ->  DEC: " + numbers[i].decStr + "]";
                pickerOpts.push_back(label);
            }

            compSubA = promptScrollableMenu("SELECT  A", pickerOpts, 0);
            int defaultB = (compSubA == 0) ? 1 : 0;
            compSubB = promptScrollableMenu("SELECT  B", pickerOpts, defaultB);

            if (usingBCDAdd)
                bcdAddResult = bcdAdd(numbers[compSubA], numbers[compSubB]);
            else
                bcdSubResult = bcdSubtract(numbers[compSubA], numbers[compSubB], usingBCDTensSub);
        }

        // ── Show result ───────────────────────────────────────────────────────
        clearScreen();
        displayHeader();
        printResultsTable(numbers);
        if (usingCompTable)
            printComplementTable(numbers);
        else if (usingBCDCompTable)
            printBCDComplementTable(numbers);
        else if (usingCompSub)
            printCompSubResult(compSubResult, numbers[compSubA], numbers[compSubB]);
        else if (usingBCDAdd)
            printBCDAddResult(bcdAddResult);
        else if (usingBCDSub)
            printBCDSubResult(bcdSubResult, numbers[compSubA], numbers[compSubB]);
        pauseConsole();

        // Post-complement options
        int cStepChoice = 0;
        bool cTryAnother = false;
        while (!cTryAnother)
        {
            vector<string> cStepOptions;
            for (int i = 1; i <= count; ++i)
                cStepOptions.push_back("View Math Steps for Input #" + to_string(i) + "  [" + string(1, char('a' + i - 1)) + " = " + numbers[i - 1].originalStr + " " + getBaseCode(numbers[i - 1].base) + "]");
            cStepOptions.push_back("View Current Result Again");
            cStepOptions.push_back("Try Another Complement Operation");
            cStepOptions.push_back("Back to Operation Menu");

            clearScreen();
            displayHeader();
            printResultsTable(numbers);
            if (usingCompTable)
                printComplementTable(numbers);
            else if (usingBCDCompTable)
                printBCDComplementTable(numbers);
            else if (usingCompSub)
                printCompSubResult(compSubResult, numbers[compSubA], numbers[compSubB]);
            else if (usingBCDAdd)
                printBCDAddResult(bcdAddResult);
            else if (usingBCDSub)
                printBCDSubResult(bcdSubResult, numbers[compSubA], numbers[compSubB]);

            cStepChoice = promptScrollableMenu("OPTIONS", cStepOptions, cStepChoice);

            int cViewIdx = count;
            int cAgainIdx = count + 1;
            int cBackIdx = count + 2;

            if (cStepChoice < count)
            {
                printDetailedSteps(numbers[cStepChoice], cStepChoice + 1);
                pauseConsole();
            }
            else if (cStepChoice == cViewIdx)
            {
                clearScreen();
                displayHeader();
                if (usingCompTable)
                    printComplementTable(numbers);
                else if (usingBCDCompTable)
                    printBCDComplementTable(numbers);
                else if (usingCompSub)
                    printCompSubResult(compSubResult, numbers[compSubA], numbers[compSubB]);
                else if (usingBCDAdd)
                    printBCDAddResult(bcdAddResult);
                else if (usingBCDSub)
                    printBCDSubResult(bcdSubResult, numbers[compSubA], numbers[compSubB]);
                pauseConsole();
            }
            else if (cStepChoice == cAgainIdx)
            {
                cTryAnother = true; // back to complement submenu
            }
            else if (cStepChoice == cBackIdx)
            {
                cTryAnother = true;
                compKeep = false; // exit submenu
            }
        }
    } // end complement submenu loop
}

// Prompts the user to type a custom infix expression, validates it, evaluates,
// and returns the CustomExprResult. Displays the full input UI before prompting.
CustomExprResult promptCustomExpression(const vector<InputNumber> &numbers,
                                        const string &varLegend, int count)
{
    clearScreen();
    displayHeader();
    printResultsTable(numbers);
    cout << "\n";
    printDivider('-', CLR_CYAN);
    printCentered(CLR_WHITE + "CUSTOM EXPRESSION INPUT" + CLR_RESET);
    printDivider('-', CLR_CYAN);
    printCentered(CLR_YELLOW + "Variable Map:" + CLR_RESET);
    printCentered(varLegend);
    cout << "\n";
    printCentered(CLR_GRAY + "Operators : + - * /" + CLR_RESET);
    printCentered(CLR_GRAY + "Precedence: (* /) evaluated before (+ -)" + CLR_RESET);
    printCentered(CLR_GRAY + "Parens    : supported   Implicit * : a(b+c) = a*(b+c)" + CLR_RESET);
    printCentered(CLR_GRAY + "Examples  : (a+b)*c-d   a*b+c/d   a(b+c)" + CLR_RESET);
    cout << "\n";

    string exprStr;
    while (true)
    {
        cout << centerText("Enter expression: ", TERMINAL_WIDTH - 25);
        getline(cin, exprStr);

        size_t es = exprStr.find_first_not_of(" \t\r\n");
        size_t ee = exprStr.find_last_not_of(" \t\r\n");
        exprStr = (es == string::npos) ? "" : exprStr.substr(es, ee - es + 1);

        if (exprStr.empty())
        {
            printCentered(CLR_RED + "[!] Expression cannot be empty. Try again." + CLR_RESET);
            continue;
        }

        string testErr;
        tokenizeExpr(exprStr, count, testErr);
        if (!testErr.empty())
        {
            printCentered(CLR_RED + "[!] " + testErr + CLR_RESET);
            continue;
        }
        break;
    }
    return processCustomExpression(exprStr, numbers);
}

void runInteractiveConverter()
{
    clearScreen();
    displayHeader();
    printCentered(CLR_YELLOW + "SPECIFICATION: Accepts at least 3 input numbers." + CLR_RESET);
    cout << "\n";
    int count = getValidatedInt("How many input numbers would you like to enter? (min 3): ", 3, 20);

    vector<InputNumber> numbers;
    vector<string> baseOptions = {
        "Binary (Base 2)",
        "Octal (Base 8)",
        "Decimal (Base 10)",
        "Hexadecimal (Base 16)"};

    for (int i = 1; i <= count; ++i)
    {
        // Scrollable base selection
        int baseChoice = promptScrollableMenu("SELECT BASE FOR INPUT #" + to_string(i), baseOptions);
        int base = (baseChoice == 0) ? 2 : (baseChoice == 1) ? 8
                                       : (baseChoice == 2)   ? 10
                                                             : 16;

        clearScreen();
        displayHeader();
        printDivider('-', CLR_CYAN);
        printCentered(CLR_WHITE + "--- ENTERING VALUE FOR INPUT NUMBER #" + to_string(i) + " ---" + CLR_RESET);
        printCentered("Selected Base: " + getBaseColor(base) + getBaseName(base) + CLR_RESET);
        printDivider('-', CLR_CYAN);
        cout << "\n";

        string inputStr;
        string errorMsg;
        while (true)
        {
            cout << centerText("Enter " + getBaseName(base) + " value: ", TERMINAL_WIDTH - 25);
            getline(cin, inputStr);

            // Trim whitespace
            size_t start = inputStr.find_first_not_of(" \t\r\n");
            size_t end = inputStr.find_last_not_of(" \t\r\n");
            inputStr = (start == string::npos) ? "" : inputStr.substr(start, end - start + 1);

            if (validateInput(inputStr, base, errorMsg))
                break;
            else
                printCentered(CLR_RED + "[!] ERROR: " + errorMsg + " Please re-enter." + CLR_RESET);
        }

        InputNumber num = processConversion(inputStr, base);
        numbers.push_back(num);
    }

    // Build variable legend once: "a = Input 1 (101010 BIN)   b = Input 2 (52 OCT) ..."
    string varLegend = "";
    for (int i = 0; i < count; ++i)
    {
        if (i > 0)
            varLegend += "   ";
        varLegend += CLR_CYAN + string(1, char('a' + i)) + CLR_RESET + CLR_GRAY + " = Input " + to_string(i + 1) + " (" + CLR_WHITE + numbers[i].originalStr + CLR_GRAY + " " + getBaseCode(numbers[i].base) + ")" + CLR_RESET;
    }

    bool keepGoing = true;
    while (keepGoing)
    {
        clearScreen();
        displayHeader();
        printResultsTable(numbers);
        printDivider('-', CLR_CYAN);
        printCentered(CLR_YELLOW + "VARIABLE MAP (for Custom Expression):" + CLR_RESET);
        printCentered(varLegend);
        printDivider('-', CLR_CYAN);
        cout << "\n";

        // ── MAIN OPERATION MENU ──────────────────────────────────────────────
        vector<string> operationOptions = {
            "Addition (+)         -- apply to all inputs",
            "Subtraction (-)      -- apply to all inputs",
            "Multiplication (x)   -- apply to all inputs",
            "Division (/)         -- apply to all inputs",
            "Custom Expression    -- e.g. (a+b)*c-d  (supports precedence & parentheses)",
            "Complement Operations  -->  1's & 2's complement submenu",
            "Return to Main Menu"};
        int operationChoice = promptScrollableMenu("SELECT ARITHMETIC OPERATION", operationOptions);

        if (operationChoice == 6)
        {
            keepGoing = false;
            break;
        }

        // ── COMPLEMENT SUBMENU ───────────────────────────────────────────────
        if (operationChoice == 5)
        {
            runComplementSubmenu(numbers, count);
            continue;
        }

        // ── STANDARD / CUSTOM OPERATIONS ─────────────────────────────────────
        bool usingCustomExpr = (operationChoice == 4);

        ArithmeticResult arithmeticResult;
        CustomExprResult customResult;

        if (!usingCustomExpr)
        {
            arithmeticResult = processArithmetic(numbers, operationChoice);
        }
        else
        {
            customResult = promptCustomExpression(numbers, varLegend, count);
        }

        // --- Show result ---
        clearScreen();
        displayHeader();
        printResultsTable(numbers);
        if (!usingCustomExpr)
            printArithmeticResult(arithmeticResult);
        else
            printCustomExprResult(customResult);
        pauseConsole();

        // --- Post-result options ---
        int stepChoice = 0;
        bool tryAnother = false;
        while (!tryAnother)
        {
            vector<string> stepOptions;
            for (int i = 1; i <= count; ++i)
                stepOptions.push_back("View Math Steps for Input #" + to_string(i) + "  [" + string(1, char('a' + i - 1)) + " = " + numbers[i - 1].originalStr + " " + getBaseCode(numbers[i - 1].base) + "]");
            stepOptions.push_back("View Current Result Again");
            stepOptions.push_back("Try Another Operation       (keep same numbers)");
            stepOptions.push_back("Return to Main Menu");

            clearScreen();
            displayHeader();
            printResultsTable(numbers);
            if (!usingCustomExpr)
                printArithmeticResult(arithmeticResult);
            else
                printCustomExprResult(customResult);

            stepChoice = promptScrollableMenu("OPTIONS", stepOptions, stepChoice);

            int tryAnotherIdx = count + 1;
            int returnIdx = count + 2;

            if (stepChoice < count)
            {
                printDetailedSteps(numbers[stepChoice], stepChoice + 1);
                pauseConsole();
            }
            else if (stepChoice == count)
            {
                clearScreen();
                displayHeader();
                if (!usingCustomExpr)
                    printArithmeticResult(arithmeticResult);
                else
                    printCustomExprResult(customResult);
                pauseConsole();
            }
            else if (stepChoice == tryAnotherIdx)
            {
                tryAnother = true;
            }
            else if (stepChoice == returnIdx)
            {
                keepGoing = false;
                tryAnother = true;
            }
        }
    } // end operation loop

    clearScreen();
}

void runPresetCombinations()
{
    vector<string> presetOptions = {
        "Combination 1: Binary + Octal + Decimal",
        "Combination 2: Binary + Decimal + Hexadecimal",
        "Combination 3: Octal + Decimal + Hexadecimal",
        "Combination 4: Binary + Octal + Hexadecimal",
        "Run ALL Combinations (all 4 operations each)",
        "Return to Main Menu"};

    int presetChoice = promptScrollableMenu("REQUIRED TEST CASE COMBINATIONS", presetOptions);
    if (presetChoice == 5)
    {
        clearScreen();
        return;
    }

    clearScreen();
    displayHeader();

    // Runs all 4 standard ops + complement table + complement subtractions
    auto runAllOps = [](const string &comboTitle,
                        const vector<pair<string, int>> &data)
    {
        // Build InputNumber list once
        vector<InputNumber> numbers;
        for (size_t i = 0; i < data.size(); ++i)
            numbers.push_back(processConversion(data[i].first, data[i].second));

        // Print conversion table once for this combination
        printCentered(CLR_YELLOW + ">>> " + comboTitle + " <<<" + CLR_RESET);
        printResultsTable(numbers);

        // Run all 4 arithmetic operations
        for (int op = 0; op < 4; ++op)
        {
            ArithmeticResult res = processArithmetic(numbers, op);
            printArithmeticResult(res);
        }

        // Complement table
        printComplementTable(numbers);

        // Binary complement subtraction (A - B = Input 1 - Input 2)
        CompSubResult ones = complementSubtract(numbers[0], numbers[1], false);
        printCompSubResult(ones, numbers[0], numbers[1]);

        CompSubResult twos = complementSubtract(numbers[0], numbers[1], true);
        printCompSubResult(twos, numbers[0], numbers[1]);

        // BCD operations (Input 1 + Input 2 and A - B)
        printBCDComplementTable(numbers);

        BCDAddResult bcdA = bcdAdd(numbers[0], numbers[1]);
        printBCDAddResult(bcdA);

        BCDSubResult bcdNines = bcdSubtract(numbers[0], numbers[1], false);
        printBCDSubResult(bcdNines, numbers[0], numbers[1]);

        BCDSubResult bcdTens = bcdSubtract(numbers[0], numbers[1], true);
        printBCDSubResult(bcdTens, numbers[0], numbers[1]);
    };

    // Number combinations as specified in the activity
    typedef vector<pair<string, int>> Combo;
    Combo combo1, combo2, combo3, combo4;

    // BIN + OCT + DEC
    combo1.push_back(make_pair(string("101010"), 2));
    combo1.push_back(make_pair(string("52"), 8));
    combo1.push_back(make_pair(string("42"), 10));

    // BIN + DEC + HEX
    combo2.push_back(make_pair(string("11001100"), 2));
    combo2.push_back(make_pair(string("100"), 10));
    combo2.push_back(make_pair(string("40"), 16));

    // OCT + DEC + HEX
    combo3.push_back(make_pair(string("72"), 8));
    combo3.push_back(make_pair(string("35"), 10));
    combo3.push_back(make_pair(string("1A"), 16));

    // BIN + OCT + HEX
    combo4.push_back(make_pair(string("11110000"), 2));
    combo4.push_back(make_pair(string("72"), 8));
    combo4.push_back(make_pair(string("3C"), 16));

    if (presetChoice == 0 || presetChoice == 4)
        runAllOps("Combination 1: Binary + Octal + Decimal", combo1);
    if (presetChoice == 1 || presetChoice == 4)
        runAllOps("Combination 2: Binary + Decimal + Hexadecimal", combo2);
    if (presetChoice == 2 || presetChoice == 4)
        runAllOps("Combination 3: Octal + Decimal + Hexadecimal", combo3);
    if (presetChoice == 3 || presetChoice == 4)
        runAllOps("Combination 4: Binary + Octal + Hexadecimal", combo4);

    pauseConsole();
    clearScreen();
}