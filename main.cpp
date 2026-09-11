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
int _getch()
{
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

struct InputNumber
{
    string originalStr;
    int base;
    double decimalValue;
    string binStr;
    string octStr;
    string decStr;
    string hexStr;
    bool isValid;
};

struct ArithmeticResult
{
    string operationName;
    string operationSymbol;
    string expression;       
    string decimalExpr;     
    bool isValid;
    string errorMsg;
    double decimalValue;
    string binStr;
    string octStr;
    string decStr;
    string hexStr;
};

struct CustomExprResult
{
    bool isValid;
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

const string DIGITS = "0123456789ABCDEF";
const int TERMINAL_WIDTH = 92;

const string CLR_RESET = "\033[0m";
const string CLR_RED = "\033[1;31m";
const string CLR_GREEN = "\033[1;32m";
const string CLR_YELLOW = "\033[1;33m";
const string CLR_BLUE = "\033[1;34m";
const string CLR_MAGENTA = "\033[1;35m";
const string CLR_CYAN = "\033[1;36m";
const string CLR_WHITE = "\033[1;37m";
const string CLR_GRAY = "\033[90m";

string getBaseColor(int base)
{
    switch (base)
    {
    case 2:
        return CLR_CYAN; 
    case 8:
        return CLR_YELLOW; 
    case 10:
        return CLR_MAGENTA; 
    case 16:
        return CLR_GREEN; 
    default:
        return CLR_WHITE;
    }
}


void enableVirtualTerminal()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= 0x0004; 
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}

string stripAnsi(const string &str)
{
    string result = "";
    bool insideEscape = false;
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '\033')
        {
            insideEscape = true;
        }
        else if (insideEscape && str[i] == 'm')
        {
            insideEscape = false;
        }
        else if (!insideEscape)
        {
            result += str[i];
        }
    }
    return result;
}

string centerText(const string &text, int width = TERMINAL_WIDTH)
{
    int visibleLen = static_cast<int>(stripAnsi(text).length());
    if (visibleLen >= width)
        return text;
    int leftPad = (width - visibleLen) / 2;
    return string(leftPad, ' ') + text;
}

void printCentered(const string &text, int width = TERMINAL_WIDTH)
{
    cout << centerText(text, width) << "\n";
}

void printDivider(char ch = '=', const string &color = CLR_CYAN, int width = TERMINAL_WIDTH)
{
    printCentered(color + string(width - 4, ch) + CLR_RESET, width);
}

// --- Scrollable / Keyboard Menu Navigation ---
int promptScrollableMenu(const string &title, const vector<string> &options, int defaultIdx = 0)
{
    int selected = defaultIdx;
    int total = static_cast<int>(options.size());

    while (true)
    {
        clearScreen();
        printDivider('=', CLR_CYAN);
        printCentered(CLR_WHITE + "NUMBER SYSTEM CONVERTER SYSTEM" + CLR_RESET);
        printDivider('=', CLR_CYAN);
        cout << "\n";

        if (!title.empty())
        {
            printCentered(CLR_YELLOW + "============== " + title + " ==============" + CLR_RESET);
            cout << "\n";
        }

        for (int i = 0; i < total; ++i)
        {
            if (i == selected)
            {
                // Highlighted item with glowing indicator
                string line = " ->  [ " + options[i] + " ] ";
                printCentered(CLR_CYAN + line + CLR_RESET);
            }
            else
            {
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
        if (ch == 0 || ch == 224)
        {
            // Extended key code for Arrows
            int arrow = _getch();
            if (arrow == 72)
            { // UP Arrow
                selected = (selected - 1 + total) % total;
            }
            else if (arrow == 80)
            { // DOWN Arrow
                selected = (selected + 1) % total;
            }
        }
        else if (ch == 'w' || ch == 'W')
        {
            selected = (selected - 1 + total) % total;
        }
        else if (ch == 's' || ch == 'S')
        {
            selected = (selected + 1) % total;
        }
        else if (ch == 13 || ch == '\n' || ch == ' ')
        { // Enter or Space
            return selected;
        }
        else if (ch >= '1' && ch <= '9')
        {
            int numChoice = ch - '1';
            if (numChoice < total)
            {
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
bool validateInput(const string &inputStr, int base, string &errorMsg);
double toDecimal(const string &inputStr, int base);
string fromDecimal(double decVal, int targetBase, int maxFrac = 6);
string formatDecimalValue(double value);
InputNumber processConversion(const string &inputStr, int base);
ArithmeticResult processArithmetic(const vector<InputNumber> &numbers, int operationChoice);
void printArithmeticResult(const ArithmeticResult &result);
CustomExprResult processCustomExpression(const string &exprStr, const vector<InputNumber> &numbers);
void printCustomExprResult(const CustomExprResult &result);
void printResultsTable(const vector<InputNumber> &numbers);
void printDetailedSteps(const InputNumber &num, int index);
void runInteractiveConverter();
void runPresetCombinations();
void pauseConsole();
int getValidatedInt(const string &prompt, int minVal, int maxVal);

// =====================================================================================
// ASCII INTRO & HEADER
// =====================================================================================

void playTetrisAnimation(int totalFrames = 36)
{
    const int ROWS = 7;
    const int COLS = 10;

    auto getBlockColor = [](int blockType) -> string
    {
        switch (blockType)
        {
        case 1:
            return CLR_YELLOW; // O-Piece #1 (Yellow)
        case 2:
            return CLR_BLUE; // J-Piece (Blue)
        case 3:
            return CLR_RED; // L-Piece (Red)
        case 4:
            return CLR_MAGENTA; // O-Piece #2 (Magenta)
        case 5:
            return CLR_CYAN; // I-Piece (Cyan)
        case 9:
            return CLR_WHITE; // Line clear flash (White)
        default:
            return "";
        }
    };

    for (int frame = 0; frame < totalFrames; ++frame)
    {
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
        board[o1Row][0] = 1;
        board[o1Row][1] = 1;
        board[o1Row + 1][0] = 1;
        board[o1Row + 1][1] = 1;

        // --- 2. Blue J-Piece at col 2-4 (Drops frames 6-11 to row 5-6) ---
        if (frame >= 6)
        {
            int jRow = min(frame - 6, 5);
            board[jRow][2] = 2;
            board[jRow + 1][2] = 2;
            board[jRow + 1][3] = 2;
            board[jRow + 1][4] = 2;
        }

        // --- 3. Red L-Piece at col 5-7 (Drops frames 12-17 to row 5-6) ---
        if (frame >= 12)
        {
            int lRow = min(frame - 12, 5);
            board[lRow][7] = 3;
            board[lRow + 1][5] = 3;
            board[lRow + 1][6] = 3;
            board[lRow + 1][7] = 3;
        }

        // --- 4. Magenta O-Piece (2x2) at col 8-9 (Drops frames 18-23 to row 5-6) ---
        if (frame >= 18)
        {
            int o2Row = min(frame - 18, 5);
            board[o2Row][8] = 4;
            board[o2Row][9] = 4;
            board[o2Row + 1][8] = 4;
            board[o2Row + 1][9] = 4;
        }

        // --- 5. Cyan I-Piece (1x4 horizontal) drops into central gap at col 3-6 on row 5 (Frames 24-29) ---
        if (frame >= 24)
        {
            int iRow = min(frame - 24, 5);
            for (int c = 3; c <= 6; ++c)
            {
                board[iRow][c] = 5;
            }
        }

        // --- 6. Line Clear Flash (Frames 30-33) ---
        if (frame >= 30 && frame < 34)
        {
            int flash = (frame % 2 == 0) ? 9 : 5;
            for (int c = 0; c < COLS; ++c)
            {
                board[5][c] = flash;
                board[6][c] = flash;
            }
        }

        // --- 7. Cleared Lines (Frames 34-35) ---
        if (frame >= 34)
        {
            for (int c = 0; c < COLS; ++c)
            {
                board[5][c] = 0;
                board[6][c] = 0;
            }
        }

        // Render Centered Tetris Matrix
        printCentered(CLR_GRAY + "+--------------------+" + CLR_RESET);
        for (int r = 0; r < ROWS; ++r)
        {
            string rowStr = CLR_GRAY + "| " + CLR_RESET;
            for (int c = 0; c < COLS; ++c)
            {
                int cell = board[r][c];
                if (cell == 0)
                {
                    rowStr += "  ";
                }
                else
                {
                    rowStr += getBlockColor(cell) + "[]" + CLR_RESET;
                }
            }
            rowStr += CLR_GRAY + " |" + CLR_RESET;
            printCentered(rowStr);
        }
        printCentered(CLR_GRAY + "+--------------------+" + CLR_RESET);

        cout << "\n";
        if (frame >= 30)
        {
            printCentered(CLR_YELLOW + " DOUBLE TETRIS LINE CLEAR!  " + CLR_RESET);
        }
        else
        {
            printCentered(CLR_GRAY + "Loading Number System Converter... (Press any key to skip)" + CLR_RESET);
        }

#ifdef _WIN32
        if (_kbhit())
        {
            _getch();
            break;
        }
#endif
        this_thread::sleep_for(chrono::milliseconds(70));
    }
}

void displayAsciiIntro()
{
    playTetrisAnimation();
}

void displayHeader()
{
    printDivider('=', CLR_CYAN);
    printCentered(CLR_WHITE + "NUMBER SYSTEM CONVERTER SYSTEM" + CLR_RESET);
    printDivider('=', CLR_CYAN);
    cout << "\n";
}

int main()
{
    enableVirtualTerminal();
    displayAsciiIntro();

    vector<string> mainOptions = {
        "Interactive Number System Converter (Inputs >= 3)",
        "Run Preset Combinations (Test Matrix)",
        "View System Specifications & Requirements",
        "Exit Program"};

    int choice = 0;
    do
    {
        choice = promptScrollableMenu("MAIN MENU", mainOptions, choice);

        switch (choice)
        {
        case 0:
            runInteractiveConverter();
            break;
        case 1:
            runPresetCombinations();
            break;
        case 2:
        {
            clearScreen();
            displayHeader();
            printCentered(CLR_YELLOW + "----------------- SYSTEM SPECIFICATIONS -----------------" + CLR_RESET);
            printCentered("Activity: Number System Converter (Activity No. 1)       ");
            printCentered("Supported Bases: Binary(2), Octal(8), Decimal(10), Hex(16)");
            printCentered("Minimum Input Requirement: 3 Numbers (Accepts N >= 3)    ");
            printCentered("Validation: Strict Character-by-Character Radix Checking ");
            printCentered("Precision: Full Integer & Radix Fractional Support       ");
            printCentered("Arithmetic: Addition, Subtraction, Multiplication, Division");
            printCentered("Navigation: Scrollable Menus using Arrow Keys & W/S      ");
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

string getBaseName(int base)
{
    switch (base)
    {
    case 2:
        return "Binary (Base 2)";
    case 8:
        return "Octal (Base 8)";
    case 10:
        return "Decimal (Base 10)";
    case 16:
        return "Hexadecimal (Base 16)";
    default:
        return "Unknown Base";
    }
}

string getBaseCode(int base)
{
    switch (base)
    {
    case 2:
        return "BIN";
    case 8:
        return "OCT";
    case 10:
        return "DEC";
    case 16:
        return "HEX";
    default:
        return "UNK";
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

        char upperC = toupper(static_cast<unsigned char>(c));
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

// Convert Base-N string to Decimal intermediate using Positional Notation
double toDecimal(const string &inputStr, int base)
{
    string str = inputStr;
    for (char &c : str)
        c = toupper(static_cast<unsigned char>(c));

    size_t dotPos = str.find('.');
    string intPart = (dotPos == string::npos) ? str : str.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : str.substr(dotPos + 1);

    if (intPart.empty())
        intPart = "0";

    double decValue = 0.0;

    // Integer part: sum(d_i * base^power)
    int intLen = static_cast<int>(intPart.length());
    for (int i = 0; i < intLen; ++i)
    {
        int digit = static_cast<int>(DIGITS.find(intPart[i]));
        int power = intLen - 1 - i;
        decValue += digit * pow(base, power);
    }

    // Fractional part: sum(d_j * base^(-j))
    for (size_t j = 0; j < fracPart.length(); ++j)
    {
        int digit = static_cast<int>(DIGITS.find(fracPart[j]));
        int power = -(static_cast<int>(j) + 1);
        decValue += digit * pow(base, power);
    }

    return decValue;
}

// Convert Decimal value to target Base-M string using successive division & multiplication
string fromDecimal(double decVal, int targetBase, int maxFrac)
{
    if (decVal == 0.0)
        return "0";

    bool isNegative = decVal < 0;
    double absVal = fabs(decVal);
    long long intPart = static_cast<long long>(floor(absVal));
    double fracPart = absVal - intPart;

    // Integer conversion: Successive division
    string intResult = "";
    if (intPart == 0)
    {
        intResult = "0";
    }
    else
    {
        long long temp = intPart;
        while (temp > 0)
        {
            int rem = static_cast<int>(temp % targetBase);
            intResult = DIGITS[rem] + intResult;
            temp /= targetBase;
        }
    }

    // Fractional conversion: Successive multiplication
    string fracResult = "";
    if (fracPart > 1e-9)
    {
        double tempFrac = fracPart;
        int count = 0;
        while (tempFrac > 1e-9 && count < maxFrac)
        {
            tempFrac *= targetBase;
            int digit = static_cast<int>(floor(tempFrac + 1e-12));
            if (digit >= targetBase)
                digit = targetBase - 1;
            fracResult += DIGITS[digit];
            tempFrac -= digit;
            count++;
        }
    }

    if (isNegative)
        intResult = "-" + intResult;

    if (!fracResult.empty())
    {
        return intResult + "." + fracResult;
    }
    return intResult;
}

string formatDecimalValue(double value)
{
    if (fabs(value) < 1e-9)
        value = 0.0;

    stringstream ss;
    if (fabs(value - round(value)) < 1e-9)
    {
        ss << static_cast<long long>(round(value));
    }
    else
    {
        ss << fixed << setprecision(6) << value;
        string result = ss.str();
        while (!result.empty() && result.back() == '0')
            result.pop_back();
        if (!result.empty() && result.back() == '.')
            result.pop_back();
        return result;
    }
    return ss.str();
}

// Full conversion process for a single input
InputNumber processConversion(const string &inputStr, int base)
{
    InputNumber num;
    num.originalStr = inputStr;
    num.base = base;

    string errorMsg;
    num.isValid = validateInput(inputStr, base, errorMsg);

    if (!num.isValid)
    {
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

    num.decStr = formatDecimalValue(num.decimalValue);
    num.hexStr = fromDecimal(num.decimalValue, 16);
    return num;
}

ArithmeticResult processArithmetic(const vector<InputNumber> &numbers, int operationChoice)
{
    ArithmeticResult result;
    result.isValid = true;
    result.errorMsg = "";

    switch (operationChoice)
    {
    case 0:
        result.operationName = "Addition";
        result.operationSymbol = "+";
        result.decimalValue = 0.0;
        for (const auto &num : numbers)
            result.decimalValue += num.decimalValue;
        break;
    case 1:
        result.operationName = "Subtraction";
        result.operationSymbol = "-";
        result.decimalValue = numbers[0].decimalValue;
        for (size_t i = 1; i < numbers.size(); ++i)
            result.decimalValue -= numbers[i].decimalValue;
        break;
    case 2:
        result.operationName = "Multiplication";
        result.operationSymbol = "x";
        result.decimalValue = 1.0;
        for (const auto &num : numbers)
            result.decimalValue *= num.decimalValue;
        break;
    case 3:
        result.operationName = "Division";
        result.operationSymbol = "/";
        result.decimalValue = numbers[0].decimalValue;
        for (size_t i = 1; i < numbers.size(); ++i)
        {
            if (fabs(numbers[i].decimalValue) < 1e-12)
            {
                result.isValid = false;
                result.errorMsg = "Division by zero is not allowed.";
                break;
            }
            result.decimalValue /= numbers[i].decimalValue;
        }
        break;
    default:
        result.isValid = false;
        result.errorMsg = "Unknown arithmetic operation.";
        break;
    }

    // --- Build original-value expression (e.g. "101010 [BIN] + 52 [OCT] + 42 [DEC]") ---
    stringstream expSS;
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        if (i > 0)
            expSS << " " << result.operationSymbol << " ";
        expSS << numbers[i].originalStr << " [" << getBaseCode(numbers[i].base) << "]";
    }
    result.expression = expSS.str();

    // --- Build decimal-equivalent expression (e.g. "42 + 42 + 42") ---
    stringstream decExpSS;
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        if (i > 0)
            decExpSS << " " << result.operationSymbol << " ";
        decExpSS << numbers[i].decStr;
    }
    if (result.isValid)
        decExpSS << " = " << result.decStr;
    result.decimalExpr = decExpSS.str();

    if (result.isValid)
    {
        result.binStr = fromDecimal(result.decimalValue, 2);
        result.octStr = fromDecimal(result.decimalValue, 8);
        result.decStr = formatDecimalValue(result.decimalValue);
        result.hexStr = fromDecimal(result.decimalValue, 16);
    }
    else
    {
        result.binStr = "UNDEFINED";
        result.octStr = "UNDEFINED";
        result.decStr = "UNDEFINED";
        result.hexStr = "UNDEFINED";
    }

    return result;
}

// =====================================================================================
// EXPRESSION EVALUATOR  (Shunting-Yard + RPN stack machine)
// =====================================================================================

enum TokenKind { TOK_VAR, TOK_OP, TOK_LPAREN, TOK_RPAREN, TOK_END, TOK_INVALID };

struct Token
{
    TokenKind kind;   // token category
    char      op;     // operator character (for TOK_OP, TOK_LPAREN, TOK_RPAREN)
    int       varIdx; // 0-based index into numbers[] (for TOK_VAR)
    string    raw;    // original text, used for error messages
};

// Factory so we don't rely on aggregate brace-init (GCC 4.9 compat)
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

// Lexer: convert expression string into a list of tokens.
// Variables a-z map to input numbers 0-25 (must be within numVars).
vector<Token> tokenizeExpr(const string &expr, int numVars, string &errorMsg)
{
    vector<Token> tokens;
    errorMsg = "";

    for (size_t i = 0; i < expr.length(); ++i)
    {
        char c = expr[i];

        // Skip whitespace
        if (isspace(static_cast<unsigned char>(c))) continue;

        if (c == '(')
        {
            tokens.push_back(makeToken(TOK_LPAREN, '(', -1, "("));
        }
        else if (c == ')')
        {
            tokens.push_back(makeToken(TOK_RPAREN, ')', -1, ")"));
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            tokens.push_back(makeToken(TOK_OP, c, -1, string(1, c)));
        }
        else if (isalpha(static_cast<unsigned char>(c)))
        {
            int idx = tolower(static_cast<unsigned char>(c)) - 'a';
            if (idx >= numVars)
            {
                errorMsg = string("Variable '") + c + "' is out of range. "
                         + "Only " + to_string(numVars) + " input(s) defined "
                         + "(a" + (numVars > 1 ? string(" to ") + char('a' + numVars - 1) : "") + ").";
                return vector<Token>();
            }
            tokens.push_back(makeToken(TOK_VAR, 0, idx, string(1, c)));
        }
        else
        {
            errorMsg = string("Invalid character '") + c + "' in expression. "
                       + "Use letters a-" + char('a' + numVars - 1) + " and operators + - * / ( ).";
            return vector<Token>();
        }
    }

    tokens.push_back(makeToken(TOK_END, 0, -1, ""));

    vector<Token> expanded;
    for (size_t j = 0; j < tokens.size(); ++j)
    {
        expanded.push_back(tokens[j]);
        if (j + 1 < tokens.size())
        {
            TokenKind cur  = tokens[j].kind;
            TokenKind next = tokens[j + 1].kind;
            bool needMul = (cur == TOK_VAR    && next == TOK_LPAREN)
                        || (cur == TOK_RPAREN  && next == TOK_VAR)
                        || (cur == TOK_RPAREN  && next == TOK_LPAREN)
                        || (cur == TOK_VAR     && next == TOK_VAR);
            if (needMul)
                expanded.push_back(makeToken(TOK_OP, '*', -1, "*"));
        }
    }
    return expanded;
}

// Shunting-Yard: convert infix token list to postfix (RPN).
vector<Token> infixToPostfix(const vector<Token> &tokens, string &errorMsg)
{
    vector<Token> output;
    vector<Token> opStack;
    errorMsg = "";

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const Token &tok = tokens[i];
        if (tok.kind == TOK_END) break;

        if (tok.kind == TOK_VAR)
        {
            output.push_back(tok);
        }
        else if (tok.kind == TOK_OP)
        {
            // Pop operators with higher or equal precedence (left-associative)
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
        {
            opStack.push_back(tok);
        }
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

    // Drain remaining operators
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

// RPN evaluator: compute the result of a postfix token list.
struct RpnResult
{
    bool   ok;
    string msg;
    double val;
};

RpnResult makeRpnResult(bool ok, const string &msg, double val)
{
    RpnResult r;
    r.ok  = ok;
    r.msg = msg;
    r.val = val;
    return r;
}

RpnResult evalPostfix(const vector<Token> &postfix, const vector<InputNumber> &numbers)
{
    vector<double> stk;

    for (size_t i = 0; i < postfix.size(); ++i)
    {
        const Token &tok = postfix[i];

        if (tok.kind == TOK_VAR)
        {
            stk.push_back(numbers[tok.varIdx].decimalValue);
        }
        else if (tok.kind == TOK_OP)
        {
            if (stk.size() < 2)
                return makeRpnResult(false, string("Not enough operands for operator '") + tok.op + "'.", 0.0);

            double b = stk.back(); stk.pop_back();
            double a = stk.back(); stk.pop_back();

            if (tok.op == '+')
                stk.push_back(a + b);
            else if (tok.op == '-')
                stk.push_back(a - b);
            else if (tok.op == '*')
                stk.push_back(a * b);
            else if (tok.op == '/')
            {
                if (fabs(b) < 1e-12)
                    return makeRpnResult(false, "Division by zero: divisor evaluates to 0.", 0.0);
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

// High-level: tokenize, convert, evaluate, format results.
CustomExprResult processCustomExpression(const string &exprStr, const vector<InputNumber> &numbers)
{
    CustomExprResult res;
    res.isValid      = false;
    res.originalExpr = exprStr;
    res.decimalValue = 0.0;
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
    { res.errorMsg = "Expression is empty after parsing."; return res; }

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

    // Build substituted and decimal-equivalent expressions character-by-character
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
        else if (c == '+')
        { subSS << " + "; decSS << " + "; }
        else if (c == '-')
        { subSS << " - "; decSS << " - "; }
        else if (c == '*')
        { subSS << " x "; decSS << " x "; }
        else if (c == '/')
        { subSS << " / "; decSS << " / "; }
        else
        { subSS << c; decSS << c; }
    }
    decSS << " = " << res.decStr;
    res.substitutedExpr = subSS.str();
    res.decimalExpr     = decSS.str();

    return res;
}


void pauseConsole()
{
    printCentered(CLR_YELLOW + "Press ENTER to continue..." + CLR_RESET);
    cin.ignore(10000, '\n');
}

int getValidatedInt(const string &prompt, int minVal, int maxVal)
{
    int value;
    while (true)
    {
        cout << centerText(prompt, TERMINAL_WIDTH - 20);
        if (cin >> value)
        {
            cin.ignore(10000, '\n'); // clear newline
            if (value >= minVal && value <= maxVal)
            {
                return value;
            }
            printCentered(CLR_RED + "[!] Error: Value must be between " + to_string(minVal) + " and " + to_string(maxVal) + "." + CLR_RESET);
        }
        else
        {
            printCentered(CLR_RED + "[!] Error: Invalid numeric input. Please try again." + CLR_RESET);
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
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
          << CLR_CYAN    << setw(22) << result.binStr
          << CLR_YELLOW  << setw(18) << result.octStr
          << CLR_MAGENTA << setw(18) << result.decStr
          << CLR_GREEN   << setw(18) << result.hexStr
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
          << CLR_CYAN    << setw(22) << result.binStr
          << CLR_YELLOW  << setw(18) << result.octStr
          << CLR_MAGENTA << setw(18) << result.decStr
          << CLR_GREEN   << setw(18) << result.hexStr
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
            size_t end   = inputStr.find_last_not_of(" \t\r\n");
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
        if (i > 0) varLegend += "   ";
        varLegend += CLR_CYAN + string(1, char('a' + i)) + CLR_RESET
                  + CLR_GRAY + " = Input " + to_string(i + 1) + " ("
                  + CLR_WHITE + numbers[i].originalStr
                  + CLR_GRAY  + " " + getBaseCode(numbers[i].base) + ")" + CLR_RESET;
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

        vector<string> operationOptions = {
            "Addition (+)        -- apply to all inputs",
            "Subtraction (-)     -- apply to all inputs",
            "Multiplication (x)  -- apply to all inputs",
            "Division (/)        -- apply to all inputs",
            "Custom Expression   -- e.g. (a+b)*c-d  (supports precedence & parentheses)",
            "Return to Main Menu"};
        int operationChoice = promptScrollableMenu("SELECT ARITHMETIC OPERATION", operationOptions);

        if (operationChoice == 5) { keepGoing = false; break; }

        // --- Branch: simple operation vs. custom expression ---
        bool usingCustomExpr = (operationChoice == 4);
        ArithmeticResult arithmeticResult;
        CustomExprResult customResult;

        if (!usingCustomExpr)
        {
            arithmeticResult = processArithmetic(numbers, operationChoice);
        }
        else
        {
            // Show legend + expression prompt
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

                // Trim
                size_t es = exprStr.find_first_not_of(" \t\r\n");
                size_t ee = exprStr.find_last_not_of(" \t\r\n");
                exprStr = (es == string::npos) ? "" : exprStr.substr(es, ee - es + 1);

                if (exprStr.empty())
                { printCentered(CLR_RED + "[!] Expression cannot be empty. Try again." + CLR_RESET); continue; }

                // Pre-validate
                string testErr;
                tokenizeExpr(exprStr, count, testErr);
                if (!testErr.empty())
                { printCentered(CLR_RED + "[!] " + testErr + CLR_RESET); continue; }
                break;
            }

            customResult = processCustomExpression(exprStr, numbers);
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
                stepOptions.push_back("View Math Steps for Input #" + to_string(i)
                                      + "  [" + string(1, char('a' + i - 1)) + " = "
                                      + numbers[i-1].originalStr + " "
                                      + getBaseCode(numbers[i-1].base) + "]");
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
            int returnIdx     = count + 2;

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
                keepGoing  = false;
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

    // Runs all 4 operations (+, -, *, /) on a given set of numbers
    auto runAllOps = [](const string &comboTitle,
                        const vector<pair<string, int> > &data)
    {
        // Build InputNumber list once
        vector<InputNumber> numbers;
        for (size_t i = 0; i < data.size(); ++i)
            numbers.push_back(processConversion(data[i].first, data[i].second));

        // Print conversion table once for this combination
        printCentered(CLR_YELLOW + ">>> " + comboTitle + " <<<" + CLR_RESET);
        printResultsTable(numbers);

        // Run all 4 operations
        for (int op = 0; op < 4; ++op)
        {
            ArithmeticResult res = processArithmetic(numbers, op);
            printArithmeticResult(res);
        }
    };

    // Number combinations as specified in the activity
    typedef vector<pair<string, int> > Combo;
    Combo combo1, combo2, combo3, combo4;

    // BIN + OCT + DEC
    combo1.push_back(make_pair(string("101010"),   2));
    combo1.push_back(make_pair(string("52"),       8));
    combo1.push_back(make_pair(string("42"),      10));

    // BIN + DEC + HEX
    combo2.push_back(make_pair(string("11001100"), 2));
    combo2.push_back(make_pair(string("100"),     10));
    combo2.push_back(make_pair(string("40"),      16));

    // OCT + DEC + HEX
    combo3.push_back(make_pair(string("72"),       8));
    combo3.push_back(make_pair(string("35"),      10));
    combo3.push_back(make_pair(string("1A"),      16));

    // BIN + OCT + HEX
    combo4.push_back(make_pair(string("11110000"), 2));
    combo4.push_back(make_pair(string("72"),       8));
    combo4.push_back(make_pair(string("3C"),      16));

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
