/*
 * =====================================================================================
 * Activity No. 1: System Development Activity
 * Title: Number System Converter and Arithmetic Calculator
 * Language: C++ (Standard C++11 compatible with Dev-C++, Code::Blocks, VS, GCC)
 *
 * Specifications:
 * - Interactive scrollable menu navigation with Arrow keys (Up/Down) or W/S + ENTER.
 * - Screen clearing (cls) and vibrant console colors.
 * - ASCII Art Intro Banner on opening.
 * - Centered UI layout and table formatting with ANSI escape handling.
 * - Accepts at least 3 input numbers (user can specify count >= 3).
 * - Independent base selection per input: Binary (2), Octal (8), Decimal (10), Hexadecimal (16).
 * - Full input validation per number system.
 * - Converts each input to Binary, Octal, Decimal, and Hexadecimal.
 * - Displays centered formatted tabular output.
 * - Step-by-step mathematical expansion calculations.
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
#include <chrono>
#include <thread>

#ifdef _WIN32

#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

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
const int TERMINAL_WIDTH = 92;

// --- ANSI Color Codes ---
const string CLR_RESET   = "\033[0m";
const string CLR_RED     = "\033[1;31m";
const string CLR_GREEN   = "\033[1;32m";
const string CLR_YELLOW  = "\033[1;33m";
const string CLR_BLUE    = "\033[1;34m";
const string CLR_MAGENTA = "\033[1;35m";
const string CLR_CYAN    = "\033[1;36m";
const string CLR_WHITE   = "\033[1;37m";
const string CLR_GRAY    = "\033[90m";

// Base Color Palette
string getBaseColor(int base) {
    switch (base) {
        case 2:  return CLR_CYAN;    // Binary: Cyan
        case 8:  return CLR_YELLOW;  // Octal: Yellow/Gold
        case 10: return CLR_MAGENTA; // Decimal: Purple/Magenta
        case 16: return CLR_GREEN;   // Hexadecimal: Green
        default: return CLR_WHITE;
    }
}

// Enable Virtual Terminal Processing for Windows Console
void enableVirtualTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

// Clear screen helper
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}

// Strip ANSI escape sequences to compute visible text length
string stripAnsi(const string& str) {
    string result = "";
    bool insideEscape = false;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\033') {
            insideEscape = true;
        } else if (insideEscape && str[i] == 'm') {
            insideEscape = false;
        } else if (!insideEscape) {
            result += str[i];
        }
    }
    return result;
}

// Centering Helpers
string centerText(const string& text, int width = TERMINAL_WIDTH) {
    int visibleLen = static_cast<int>(stripAnsi(text).length());
    if (visibleLen >= width) return text;
    int leftPad = (width - visibleLen) / 2;
    return string(leftPad, ' ') + text;
}

void printCentered(const string& text, int width = TERMINAL_WIDTH) {
    cout << centerText(text, width) << "\n";
}

void printDivider(char ch = '=', const string& color = CLR_CYAN, int width = TERMINAL_WIDTH) {
    printCentered(color + string(width - 4, ch) + CLR_RESET, width);
}

// --- Scrollable / Keyboard Menu Navigation ---
int promptScrollableMenu(const string& title, const vector<string>& options, int defaultIdx = 0) {
    int selected = defaultIdx;
    int total = static_cast<int>(options.size());

    while (true) {
        clearScreen();
        printDivider('=', CLR_CYAN);
        printCentered(CLR_WHITE + "NUMBER SYSTEM CONVERTER SYSTEM" + CLR_RESET);
        printDivider('=', CLR_CYAN);
        cout << "\n";

        if (!title.empty()) {
            printCentered(CLR_YELLOW + "============== " + title + " ==============" + CLR_RESET);
            cout << "\n";
        }

        for (int i = 0; i < total; ++i) {
            if (i == selected) {
                // Highlighted item with glowing indicator
                string line = " ->  [ " + options[i] + " ] ";
                printCentered(CLR_CYAN + line + CLR_RESET);
            } else {
                // Normal inactive item
                string line = "    " + options[i] + "   ";
                printCentered(CLR_GRAY + line + CLR_RESET);
            }
        }

        cout << "\n";
        printDivider('-', CLR_GRAY);
        printDivider('-', CLR_GRAY);
        cout << "\n";

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            // Extended key code for Arrows
            int arrow = _getch();
            if (arrow == 72) { // UP Arrow
                selected = (selected - 1 + total) % total;
            } else if (arrow == 80) { // DOWN Arrow
                selected = (selected + 1) % total;
            }
        } else if (ch == 'w' || ch == 'W') {
            selected = (selected - 1 + total) % total;
        } else if (ch == 's' || ch == 'S') {
            selected = (selected + 1) % total;
        } else if (ch == 13 || ch == '\n' || ch == ' ') { // Enter or Space
            return selected;
        } else if (ch >= '1' && ch <= '9') {
            int numChoice = ch - '1';
            if (numChoice < total) {
                return numChoice;
            }
        }
    }
}

// --- Function Prototypes ---
void displayAsciiIntro();
void displayHeader();
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
void pauseConsole();
int getValidatedInt(const string& prompt, int minVal, int maxVal);

// =====================================================================================
// ASCII INTRO & HEADER
// =====================================================================================

// Geometrically Accurate Multi-Colored Tetris Animation
void playTetrisAnimation(int totalFrames = 36) {
    const int ROWS = 7;
    const int COLS = 10;

    auto getBlockColor = [](int blockType) -> string {
        switch (blockType) {
            case 1: return CLR_YELLOW;  // O-Piece #1 (Yellow)
            case 2: return CLR_BLUE;    // J-Piece (Blue)
            case 3: return CLR_RED;     // L-Piece (Red)
            case 4: return CLR_MAGENTA; // O-Piece #2 (Magenta)
            case 5: return CLR_CYAN;    // I-Piece (Cyan)
            case 9: return CLR_WHITE;   // Line clear flash (White)
            default: return "";
        }
    };

    for (int frame = 0; frame < totalFrames; ++frame) {
        clearScreen();
        cout << "\n";
        printCentered(CLR_CYAN + "     _   _                 _                  ____                               _             " + CLR_RESET);
        printCentered(CLR_CYAN + "| \\ | |_   _ _ __ ___ | |__   ___ _ __   / ___|___  _ ____   _____ _ __| |_ ___ _ __ " + CLR_RESET);
        printCentered(CLR_CYAN + "|  \\| | | | | '_ ` _ \\| '_ \\ / _ \\ '__| | |   / _ \\| '_ \\ \\ / / _ \\ '__| __/ _ \\ '__|" + CLR_RESET);
        printCentered(CLR_CYAN + "| |\\  | |_| | | | | | | |_) |  __/ |    | |__| (_) | | | \\ V /  __/ |  | ||  __/ |   " + CLR_RESET);
        printCentered(CLR_CYAN + "|_| \\_|\\__,_|_| |_| |_|_.__/ \\___|_|     \\____\\___/|_| |_|\\_/ \\___|_|   \\__\\___|_|   " + CLR_RESET);
        cout << "\n";
        printCentered(CLR_YELLOW + "Activity No. 1" + CLR_RESET);
        printCentered(CLR_CYAN + "Binary (2)" + CLR_GRAY + " | " + CLR_YELLOW + "Octal (8)" + CLR_GRAY + " | " + CLR_MAGENTA + "Decimal (10)" + CLR_GRAY + " | " + CLR_GREEN + "Hexadecimal (16)" + CLR_RESET);
        printDivider('~', CLR_GRAY);
        cout << "\n";

        int board[ROWS][COLS] = {0};

        // --- 1. Yellow O-Piece (2x2) at col 0-1 (Drops frames 0-5 to row 5-6) ---
        int o1Row = min(frame, 5);
        board[o1Row][0] = 1;     board[o1Row][1] = 1;
        board[o1Row + 1][0] = 1; board[o1Row + 1][1] = 1;

        // --- 2. Blue J-Piece at col 2-4 (Drops frames 6-11 to row 5-6) ---
        if (frame >= 6) {
            int jRow = min(frame - 6, 5);
            board[jRow][2] = 2;
            board[jRow + 1][2] = 2; board[jRow + 1][3] = 2; board[jRow + 1][4] = 2;
        }

        // --- 3. Red L-Piece at col 5-7 (Drops frames 12-17 to row 5-6) ---
        if (frame >= 12) {
            int lRow = min(frame - 12, 5);
            board[lRow][7] = 3;
            board[lRow + 1][5] = 3; board[lRow + 1][6] = 3; board[lRow + 1][7] = 3;
        }

        // --- 4. Magenta O-Piece (2x2) at col 8-9 (Drops frames 18-23 to row 5-6) ---
        if (frame >= 18) {
            int o2Row = min(frame - 18, 5);
            board[o2Row][8] = 4;     board[o2Row][9] = 4;
            board[o2Row + 1][8] = 4; board[o2Row + 1][9] = 4;
        }

        // --- 5. Cyan I-Piece (1x4 horizontal) drops into central gap at col 3-6 on row 5 (Frames 24-29) ---
        if (frame >= 24) {
            int iRow = min(frame - 24, 5);
            for (int c = 3; c <= 6; ++c) {
                board[iRow][c] = 5;
            }
        }

        // --- 6. Line Clear Flash (Frames 30-33) ---
        if (frame >= 30 && frame < 34) {
            int flash = (frame % 2 == 0) ? 9 : 5;
            for (int c = 0; c < COLS; ++c) {
                board[5][c] = flash;
                board[6][c] = flash;
            }
        }

        // --- 7. Cleared Lines (Frames 34-35) ---
        if (frame >= 34) {
            for (int c = 0; c < COLS; ++c) {
                board[5][c] = 0;
                board[6][c] = 0;
            }
        }

        // Render Centered Tetris Matrix
        printCentered(CLR_GRAY + "+--------------------+" + CLR_RESET);
        for (int r = 0; r < ROWS; ++r) {
            string rowStr = CLR_GRAY + "| " + CLR_RESET;
            for (int c = 0; c < COLS; ++c) {
                int cell = board[r][c];
                if (cell == 0) {
                    rowStr += "  ";
                } else {
                    rowStr += getBlockColor(cell) + "[]" + CLR_RESET;
                }
            }
            rowStr += CLR_GRAY + " |" + CLR_RESET;
            printCentered(rowStr);
        }
        printCentered(CLR_GRAY + "+--------------------+" + CLR_RESET);

        cout << "\n";
        if (frame >= 30) {
            printCentered(CLR_YELLOW + " DOUBLE TETRIS LINE CLEAR!  " + CLR_RESET);
        } else {
            printCentered(CLR_GRAY + "Loading Number System Converter... (Press any key to skip)" + CLR_RESET);
        }

#ifdef _WIN32
        if (_kbhit()) {
            _getch();
            break;
        }
#endif
        this_thread::sleep_for(chrono::milliseconds(70));
    }
}


void displayAsciiIntro() {
    playTetrisAnimation();
}



void displayHeader() {
    printDivider('=', CLR_CYAN);
    printCentered(CLR_WHITE + "NUMBER SYSTEM CONVERTER SYSTEM" + CLR_RESET);
    printDivider('=', CLR_CYAN);
    cout << "\n";
}

// =====================================================================================
// MAIN ENTRY POINT
// =====================================================================================
int main() {
    enableVirtualTerminal();
    displayAsciiIntro();

    vector<string> mainOptions = {
        "Interactive Number System Converter (Inputs >= 3)",
        "Run Preset Combinations (Test Matrix)",
        "View System Specifications & Requirements",
        "Exit Program"
    };

    int choice = 0;
    do {
        choice = promptScrollableMenu("MAIN MENU", mainOptions, choice);

        switch (choice) {
            case 0:
                runInteractiveConverter();
                break;
            case 1:
                runPresetCombinations();
                break;
            case 2: {
                clearScreen();
                displayHeader();
                printCentered(CLR_YELLOW + "----------------- SYSTEM SPECIFICATIONS -----------------" + CLR_RESET);
                printCentered("Activity: Number System Converter (Activity No. 1)       ");
                printCentered("Supported Bases: Binary(2), Octal(8), Decimal(10), Hex(16)");
                printCentered("Minimum Input Requirement: 3 Numbers (Accepts N >= 3)    ");
                printCentered("Validation: Strict Character-by-Character Radix Checking ");
                printCentered("Precision: Full Integer & Radix Fractional Support       ");
                printCentered("Navigation: Scrollable Menus using Arrow Keys & W/S      ");
                printCentered("Phase 2 Ready: Pre-architected for Mixed-Base Arithmetic ");
                printCentered(CLR_YELLOW + "---------------------------------------------------------" + CLR_RESET);
                cout << "\n";
                pauseConsole();
                clearScreen();
                break;
            }
            case 3:
                clearScreen();
                cout << "\n";
                printDivider('*', CLR_GREEN);
                printCentered(CLR_GREEN + "Thank you for using the Number System Converter. Goodbye!" + CLR_RESET);
                printDivider('*', CLR_GREEN);
                cout << "\n";
                break;
        }
    } while (choice != 3);

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

    if (inputStr == ".") {
        errorMsg = "Input cannot be just a radix point.";
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

void pauseConsole() {
    printCentered(CLR_YELLOW + "Press ENTER to continue..." + CLR_RESET);
    cin.ignore(10000, '\n');
}

int getValidatedInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << centerText(prompt, TERMINAL_WIDTH - 20);
        if (cin >> value) {
            cin.ignore(10000, '\n'); // clear newline
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            printCentered(CLR_RED + "[!] Error: Value must be between " + to_string(minVal) + " and " + to_string(maxVal) + "." + CLR_RESET);
        } else {
            printCentered(CLR_RED + "[!] Error: Invalid numeric input. Please try again." + CLR_RESET);
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

// Centered formatted table of conversion results with color coding
void printResultsTable(const vector<InputNumber>& numbers) {
    cout << "\n";
    printDivider('=', CLR_CYAN);
    printCentered(CLR_WHITE + "CONVERSION RESULTS TABLE" + CLR_RESET);
    printDivider('=', CLR_CYAN);

    // Header row
    stringstream headerSS;
    headerSS << left
             << setw(4)  << "#"
             << setw(15) << "Original Input"
             << setw(8)  << "Base"
             << setw(20) << "Binary (Base 2)"
             << setw(16) << "Octal (Base 8)"
             << setw(14) << "Decimal (10)"
             << setw(9)  << "Hex (16)";
    
    printCentered(CLR_YELLOW + headerSS.str() + CLR_RESET);
    printDivider('-', CLR_GRAY);

    for (size_t i = 0; i < numbers.size(); ++i) {
        const auto& num = numbers[i];
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

// Display step-by-step mathematical expansion
void printDetailedSteps(const InputNumber& num, int index) {
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
    for (char &c : str) c = toupper(static_cast<unsigned char>(c));
    size_t dotPos = str.find('.');
    string intPart = (dotPos == string::npos) ? str : str.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : str.substr(dotPos + 1);

    stringstream expSS;
    expSS << "= ";
    int intLen = static_cast<int>(intPart.length());
    bool first = true;
    for (int i = 0; i < intLen; ++i) {
        if (!first) expSS << " + ";
        int power = intLen - 1 - i;
        expSS << "(" << intPart[i] << " * " << num.base << "^" << power << ")";
        first = false;
    }
    for (size_t j = 0; j < fracPart.length(); ++j) {
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

// Interactive Converter Workflow
void runInteractiveConverter() {
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
        "Hexadecimal (Base 16)"
    };

    for (int i = 1; i <= count; ++i) {
        // Scrollable base selection
        int baseChoice = promptScrollableMenu("SELECT BASE FOR INPUT #" + to_string(i), baseOptions);
        int base = (baseChoice == 0) ? 2 : (baseChoice == 1) ? 8 : (baseChoice == 2) ? 10 : 16;

        clearScreen();
        displayHeader();
        printDivider('-', CLR_CYAN);
        printCentered(CLR_WHITE + "--- ENTERING VALUE FOR INPUT NUMBER #" + to_string(i) + " ---" + CLR_RESET);
        printCentered("Selected Base: " + getBaseColor(base) + getBaseName(base) + CLR_RESET);
        printDivider('-', CLR_CYAN);
        cout << "\n";

        string inputStr;
        string errorMsg;
        while (true) {
            cout << centerText("Enter " + getBaseName(base) + " value: ", TERMINAL_WIDTH - 25);
            getline(cin, inputStr);

            // Trim whitespace
            size_t start = inputStr.find_first_not_of(" \t\r\n");
            size_t end = inputStr.find_last_not_of(" \t\r\n");
            inputStr = (start == string::npos) ? "" : inputStr.substr(start, end - start + 1);

            if (validateInput(inputStr, base, errorMsg)) {
                break;
            } else {
                printCentered(CLR_RED + "[!] ERROR: " + errorMsg + " Please re-enter." + CLR_RESET);
            }
        }

        InputNumber num = processConversion(inputStr, base);
        numbers.push_back(num);
    }

    // Options after conversion
    int stepChoice = 0;
    while (true) {
        vector<string> stepOptions;
        for (int i = 1; i <= count; ++i) {
            stepOptions.push_back("View Math Steps for Input #" + to_string(i) + " (" + numbers[i-1].originalStr + ")");
        }
        stepOptions.push_back("Return to Main Menu");

        clearScreen();
        displayHeader();
        printResultsTable(numbers);

        stepChoice = promptScrollableMenu("CONVERSION OPTIONS", stepOptions, stepChoice);

        if (stepChoice == count) {
            break; // Return to Main Menu
        } else {
            printDetailedSteps(numbers[stepChoice], stepChoice + 1);
            pauseConsole();
        }
    }

    clearScreen();
}

// Run Preset Test Combinations required by Activity No. 1
void runPresetCombinations() {
    vector<string> presetOptions = {
        "Combination 1: Binary + Octal + Decimal",
        "Combination 2: Binary + Decimal + Hexadecimal",
        "Combination 3: Octal + Decimal + Hexadecimal",
        "Combination 4: Binary + Octal + Hexadecimal",
        "Run All 4 Combinations Simultaneously",
        "Return to Main Menu"
    };

    int presetChoice = promptScrollableMenu("REQUIRED TEST CASE COMBINATIONS", presetOptions);
    if (presetChoice == 5) {
        clearScreen();
        return;
    }

    clearScreen();
    displayHeader();

    auto runPreset = [](const string& title, const vector<pair<string, int>>& data) {
        printCentered(CLR_YELLOW + ">>> " + title + " <<<" + CLR_RESET);
        vector<InputNumber> numbers;
        for (const auto& item : data) {
            numbers.push_back(processConversion(item.first, item.second));
        }
        printResultsTable(numbers);
    };

    if (presetChoice == 0 || presetChoice == 4) {
        runPreset("Combination 1: Binary + Octal + Decimal", {
            {"101010", 2},
            {"52", 8},
            {"42", 10}
        });
    }
    if (presetChoice == 1 || presetChoice == 4) {
        runPreset("Combination 2: Binary + Decimal + Hexadecimal", {
            {"11001100", 2},
            {"204", 10},
            {"CC", 16}
        });
    }
    if (presetChoice == 2 || presetChoice == 4) {
        runPreset("Combination 3: Octal + Decimal + Hexadecimal", {
            {"755", 8},
            {"493", 10},
            {"1ED", 16}
        });
    }
    if (presetChoice == 3 || presetChoice == 4) {
        runPreset("Combination 4: Binary + Octal + Hexadecimal", {
            {"11110000", 2},
            {"360", 8},
            {"F0", 16}
        });
    }

    pauseConsole();
    clearScreen();
}
