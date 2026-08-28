/*
 * =====================================================================================
 * Activity No. 1: System Development Activity
 * Title: Number System Converter and Arithmetic Calculator
 * Language: C++ (Standard C++11 compatible with Dev-C++, Code::Blocks, VS, GCC)
 * 
 * Specifications:
 * - Accepts at least 3 input numbers (user can specify count >= 3).
 * - Independent base selection per input: Binary (2), Octal (8), Decimal (10), Hexadecimal (16).
 * - Full input validation per number system.
 * - Converts each input to Binary, Octal, Decimal, and Hexadecimal.
 * - Displays clear, formatted tabular output.
 * - Provides step-by-step mathematical expansion calculations.
 * - Prepared for Phase 2 Arithmetic Calculator integration.
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <sstream>

using namespace std;

// --- Data Structures ---
struct InputNumber {
    string originalStr;
    int base;
    double decimalValue;
    string binStr;
    string octStr;
    string decStr;
    string hexStr;
    bool isValid;
};

// --- Constant Definitions ---
const string DIGITS = "0123456789ABCDEF";

// --- Function Prototypes ---
void displayHeader();
void displayMenu();
string getBaseName(int base);
string getBaseCode(int base);
bool validateInput(const string& inputStr, int base, string& errorMsg);
double toDecimal(const string& inputStr, int base);
string fromDecimal(double decVal, int targetBase, int maxFrac = 6);
InputNumber processConversion(const string& inputStr, int base);
void printResultsTable(const vector<InputNumber>& numbers);
void printDetailedSteps(const InputNumber& num, int index);
void runInteractiveConverter();
void runPresetCombinations();
void clearScreen();
void pauseConsole();
int getValidatedInt(const string& prompt, int minVal, int maxVal);

// =====================================================================================
// MAIN ENTRY POINT
// =====================================================================================
int main() {
    int choice = 0;

    do {
        displayHeader();
        cout << "  [1] Interactive Number System Converter (Custom Inputs >= 3)\n";
        cout << "  [2] Run Preset Combinations (Test Matrix from Activity 1)\n";
        cout << "  [3] System Information & Specifications\n";
        cout << "  [4] Exit Program\n";
        cout << "  ------------------------------------------------------------\n";
        choice = getValidatedInt("  Enter choice (1-4): ", 1, 4);

        switch (choice) {
            case 1:
                runInteractiveConverter();
                break;
            case 2:
                runPresetCombinations();
                break;
            case 3: {
                displayHeader();
                cout << "  ================== SYSTEM SPECIFICATIONS ==================\n";
                cout << "  Activity: Activity No. 1 - Number System Converter\n";
                cout << "  Supported Bases:\n";
                cout << "    - Binary (Base 2):       Digits 0-1\n";
                cout << "    - Octal (Base 8):        Digits 0-7\n";
                cout << "    - Decimal (Base 10):     Digits 0-9\n";
                cout << "    - Hexadecimal (Base 16): Digits 0-9, A-F\n";
                cout << "  Minimum Inputs: 3 (supports arbitrary N >= 3)\n";
                cout << "  Fractional Support: Full radix point precision conversion\n";
                cout << "  Phase 2 Ready: Pre-architected for arithmetic operations\n";
                cout << "  ===========================================================\n\n";
                pauseConsole();
                break;
            }
            case 4:
                cout << "\n  Thank you for using the Number System Converter. Goodbye!\n\n";
                break;
        }
    } while (choice != 4);

    return 0;
}

// =====================================================================================
// CONVERSION & VALIDATION LOGIC
// =====================================================================================

string getBaseName(int base) {
    switch (base) {
        case 2:  return "Binary (Base 2)";
        case 8:  return "Octal (Base 8)";
        case 10: return "Decimal (Base 10)";
        case 16: return "Hexadecimal (Base 16)";
        default: return "Unknown Base";
    }
}

string getBaseCode(int base) {
    switch (base) {
        case 2:  return "BIN";
        case 8:  return "OCT";
        case 10: return "DEC";
        case 16: return "HEX";
        default: return "UNK";
    }
}

// Validate input string according to the selected base
bool validateInput(const string& inputStr, int base, string& errorMsg) {
    if (inputStr.empty()) {
        errorMsg = "Input cannot be empty.";
        return false;
    }

    int dotCount = 0;
    for (size_t i = 0; i < inputStr.length(); ++i) {
        char c = inputStr[i];

        if (c == '.') {
            dotCount++;
            if (dotCount > 1) {
                errorMsg = "Multiple radix points (.) are not allowed.";
                return false;
            }
            continue;
        }

        char upperC = toupper(static_cast<unsigned char>(c));
        size_t digitVal = DIGITS.find(upperC);

        if (digitVal == string::npos || digitVal >= static_cast<size_t>(base)) {
            stringstream ss;
            ss << "Invalid character '" << c << "' for " << getBaseName(base) << ".";
            errorMsg = ss.str();
            return false;
        }
    }

    // Single dot check
    if (inputStr == ".") {
        errorMsg = "Input cannot be just a decimal point.";
        return false;
    }

    return true;
}

// Convert Base-N string to Decimal intermediate using Positional Notation
double toDecimal(const string& inputStr, int base) {
    string str = inputStr;
    for (char &c : str) c = toupper(static_cast<unsigned char>(c));

    size_t dotPos = str.find('.');
    string intPart = (dotPos == string::npos) ? str : str.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : str.substr(dotPos + 1);

    if (intPart.empty()) intPart = "0";

    double decValue = 0.0;

    // Integer part: sum(d_i * base^power)
    int intLen = static_cast<int>(intPart.length());
    for (int i = 0; i < intLen; ++i) {
        int digit = static_cast<int>(DIGITS.find(intPart[i]));
        int power = intLen - 1 - i;
        decValue += digit * pow(base, power);
    }

    // Fractional part: sum(d_j * base^(-j))
    for (size_t j = 0; j < fracPart.length(); ++j) {
        int digit = static_cast<int>(DIGITS.find(fracPart[j]));
        int power = -(static_cast<int>(j) + 1);
        decValue += digit * pow(base, power);
    }

    return decValue;
}

// Convert Decimal value to target Base-M string using successive division & multiplication
string fromDecimal(double decVal, int targetBase, int maxFrac) {
    if (decVal == 0.0) return "0";

    long long intPart = static_cast<long long>(floor(decVal));
    double fracPart = decVal - intPart;

    // Integer conversion: Successive division
    string intResult = "";
    if (intPart == 0) {
        intResult = "0";
    } else {
        long long temp = intPart;
        while (temp > 0) {
            int rem = temp % targetBase;
            intResult = DIGITS[rem] + intResult;
            temp /= targetBase;
        }
    }

    // Fractional conversion: Successive multiplication
    string fracResult = "";
    if (fracPart > 1e-9) {
        double tempFrac = fracPart;
        int count = 0;
        while (tempFrac > 1e-9 && count < maxFrac) {
            tempFrac *= targetBase;
            int digit = static_cast<int>(floor(tempFrac));
            fracResult += DIGITS[digit];
            tempFrac -= digit;
            count++;
        }
    }

    if (!fracResult.empty()) {
        return intResult + "." + fracResult;
    }
    return intResult;
}

// Full conversion process for a single input
InputNumber processConversion(const string& inputStr, int base) {
    InputNumber num;
    num.originalStr = inputStr;
    num.base = base;

    string errorMsg;
    num.isValid = validateInput(inputStr, base, errorMsg);

    if (!num.isValid) {
        num.decimalValue = 0.0;
        num.binStr = "INVALID";
        num.octStr = "INVALID";
        num.decStr = "INVALID";
        num.hexStr = "INVALID";
        return num;
    }

    num.decimalValue = toDecimal(inputStr, base);
    num.binStr = fromDecimal(num.decimalValue, 2);
    num.octStr = fromDecimal(num.decimalValue, 8);

    // Clean decimal string representation
    stringstream ss;
    if (floor(num.decimalValue) == num.decimalValue) {
        ss << static_cast<long long>(num.decimalValue);
    } else {
        ss << fixed << setprecision(4) << num.decimalValue;
    }
    num.decStr = ss.str();

    num.hexStr = fromDecimal(num.decimalValue, 16);
    return num;
}

// =====================================================================================
// USER INTERFACE & PRESENTATION
// =====================================================================================

void displayHeader() {
    cout << "\n";
    cout << "  ============================================================\n";
    cout << "               NUMBER SYSTEM CONVERTER SYSTEM                 \n";
    cout << "         Activity No. 1 - System Development Activity         \n";
    cout << "  ============================================================\n";
}

void pauseConsole() {
    cout << "  Press ENTER to continue...";
    cin.ignore(10000, '\n');
}

int getValidatedInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(10000, '\n'); // clear newline
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            cout << "  [!] Error: Value must be between " << minVal << " and " << maxVal << ".\n";
        } else {
            cout << "  [!] Error: Invalid numeric input. Please try again.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

// Formatted table of conversion results
void printResultsTable(const vector<InputNumber>& numbers) {
    cout << "\n";
    cout << "  ========================================================================================\n";
    cout << "                                  CONVERSION RESULTS TABLE                                \n";
    cout << "  ========================================================================================\n";
    cout << "  " << left
         << setw(4)  << "#"
         << setw(16) << "Original Input"
         << setw(8)  << "Base"
         << setw(20) << "Binary (Base 2)"
         << setw(16) << "Octal (Base 8)"
         << setw(14) << "Decimal (10)"
         << setw(12) << "Hex (16)"
         << "\n";
    cout << "  ----------------------------------------------------------------------------------------\n";

    for (size_t i = 0; i < numbers.size(); ++i) {
        const auto& num = numbers[i];
        cout << "  " << left
             << setw(4)  << (i + 1)
             << setw(16) << num.originalStr
             << setw(8)  << getBaseCode(num.base)
             << setw(20) << num.binStr
             << setw(16) << num.octStr
             << setw(14) << num.decStr
             << setw(12) << num.hexStr
             << "\n";
    }
    cout << "  ========================================================================================\n\n";
}

// Display step-by-step mathematical expansion
void printDetailedSteps(const InputNumber& num, int index) {
    cout << "\n  ------------------------------------------------------------\n";
    cout << "  MATHEMATICAL STEP-BY-STEP PROOF FOR INPUT #" << index << "\n";
    cout << "  Input: " << num.originalStr << " in " << getBaseName(num.base) << "\n";
    cout << "  ------------------------------------------------------------\n";

    // Step 1: Base N to Decimal via Positional Notation
    cout << "  Step 1: Conversion to Decimal (Base 10) using Positional Notation\n";
    cout << "  Formula: Value = Sum( Digit * Base^Position )\n\n";

    string str = num.originalStr;
    for (char &c : str) c = toupper(static_cast<unsigned char>(c));
    size_t dotPos = str.find('.');
    string intPart = (dotPos == string::npos) ? str : str.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : str.substr(dotPos + 1);

    cout << "  Expansion:\n  = ";
    int intLen = static_cast<int>(intPart.length());
    bool first = true;
    for (int i = 0; i < intLen; ++i) {
        if (!first) cout << " + ";
        int power = intLen - 1 - i;
        cout << "(" << intPart[i] << " * " << num.base << "^" << power << ")";
        first = false;
    }

    for (size_t j = 0; j < fracPart.length(); ++j) {
        cout << " + (" << fracPart[j] << " * " << num.base << "^-" << (j + 1) << ")";
    }
    cout << "\n  = " << num.decStr << " (Decimal)\n\n";

    // Step 2: Conversions from Decimal to other bases
    cout << "  Step 2: Conversion from Decimal (" << num.decStr << ") to Target Bases\n";
    cout << "    -> Binary (Base 2):       " << num.binStr << " (Base 2)\n";
    cout << "    -> Octal (Base 8):        " << num.octStr << " (Base 8)\n";
    cout << "    -> Decimal (Base 10):     " << num.decStr << " (Base 10)\n";
    cout << "    -> Hexadecimal (Base 16): " << num.hexStr << " (Base 16)\n";
    cout << "  ------------------------------------------------------------\n\n";
}

// Interactive Converter Workflow
void runInteractiveConverter() {
    displayHeader();
    cout << "  SPECIFICATION: Accepts at least 3 input numbers.\n";
    int count = getValidatedInt("  How many input numbers would you like to enter? (min 3): ", 3, 20);

    vector<InputNumber> numbers;

    for (int i = 1; i <= count; ++i) {
        cout << "\n  --- Input Number #" << i << " ---\n";
        cout << "  Select Base:\n";
        cout << "    [1] Binary (Base 2)\n";
        cout << "    [2] Octal (Base 8)\n";
        cout << "    [3] Decimal (Base 10)\n";
        cout << "    [4] Hexadecimal (Base 16)\n";
        
        int baseChoice = getValidatedInt("  Choice for Input #" + to_string(i) + " (1-4): ", 1, 4);
        int base = (baseChoice == 1) ? 2 : (baseChoice == 2) ? 8 : (baseChoice == 3) ? 10 : 16;

        string inputStr;
        string errorMsg;
        while (true) {
            cout << "  Enter " << getBaseName(base) << " value: ";
            getline(cin, inputStr);

            // Trim whitespace
            size_t start = inputStr.find_first_not_of(" \t\r\n");
            size_t end = inputStr.find_last_not_of(" \t\r\n");
            inputStr = (start == string::npos) ? "" : inputStr.substr(start, end - start + 1);

            if (validateInput(inputStr, base, errorMsg)) {
                break;
            } else {
                cout << "  [!] ERROR: " << errorMsg << " Please re-enter.\n";
            }
        }

        InputNumber num = processConversion(inputStr, base);
        numbers.push_back(num);
    }

    // Display formatted results table
    printResultsTable(numbers);

    // Option to view step-by-step solutions
    int stepChoice;
    do {
        cout << "  Options:\n";
        cout << "    [1-" << count << "] View Step-by-Step Math for specific Input Number\n";
        cout << "    [" << (count + 1) << "] Return to Main Menu\n";
        stepChoice = getValidatedInt("  Select option: ", 1, count + 1);

        if (stepChoice >= 1 && stepChoice <= count) {
            printDetailedSteps(numbers[stepChoice - 1], stepChoice);
        }
    } while (stepChoice != (count + 1));
}

// Run Preset Test Combinations required by Activity No. 1
void runPresetCombinations() {
    displayHeader();
    cout << "  ================ REQUIRED TEST CASE COMBINATIONS ================\n";
    cout << "  [1] Combination 1: Binary + Octal + Decimal\n";
    cout << "  [2] Combination 2: Binary + Decimal + Hexadecimal\n";
    cout << "  [3] Combination 3: Octal + Decimal + Hexadecimal\n";
    cout << "  [4] Combination 4: Binary + Octal + Hexadecimal\n";
    cout << "  [5] Run All 4 Combinations Simultaneously\n";
    cout << "  [6] Return to Main Menu\n";
    cout << "  -----------------------------------------------------------------\n";

    int presetChoice = getValidatedInt("  Select preset combination (1-6): ", 1, 6);
    if (presetChoice == 6) return;

    auto runPreset = [](const string& title, const vector<pair<string, int>>& data) {
        cout << "\n  >>> " << title << " <<<\n";
        vector<InputNumber> numbers;
        for (const auto& item : data) {
            numbers.push_back(processConversion(item.first, item.second));
        }
        printResultsTable(numbers);
    };

    if (presetChoice == 1 || presetChoice == 5) {
        runPreset("Combination 1: Binary + Octal + Decimal", {
            {"101010", 2},
            {"52", 8},
            {"42", 10}
        });
    }
    if (presetChoice == 2 || presetChoice == 5) {
        runPreset("Combination 2: Binary + Decimal + Hexadecimal", {
            {"11001100", 2},
            {"204", 10},
            {"CC", 16}
        });
    }
    if (presetChoice == 3 || presetChoice == 5) {
        runPreset("Combination 3: Octal + Decimal + Hexadecimal", {
            {"755", 8},
            {"493", 10},
            {"1ED", 16}
        });
    }
    if (presetChoice == 4 || presetChoice == 5) {
        runPreset("Combination 4: Binary + Octal + Hexadecimal", {
            {"11110000", 2},
            {"360", 8},
            {"F0", 16}
        });
    }

    pauseConsole();
}
