// ui.cpp — Terminal helpers, animated intro, scrollable menu, pause & input

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
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

#include "../include/globals.h"
#include "../include/prototypes.h"

using namespace std;

// ---------------------------------------------------------------------------
// Color helper
// ---------------------------------------------------------------------------
string getBaseColor(int base)
{
    switch (base)
    {
    case 2:  return CLR_CYAN;
    case 8:  return CLR_YELLOW;
    case 10: return CLR_MAGENTA;
    case 16: return CLR_GREEN;
    default: return CLR_WHITE;
    }
}

// ---------------------------------------------------------------------------
// Terminal utilities
// ---------------------------------------------------------------------------
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
            insideEscape = true;
        else if (insideEscape && str[i] == 'm')
            insideEscape = false;
        else if (!insideEscape)
            result += str[i];
    }
    return result;
}

string centerText(const string &text, int width)
{
    int visibleLen = static_cast<int>(stripAnsi(text).length());
    if (visibleLen >= width)
        return text;
    int leftPad = (width - visibleLen) / 2;
    return string(leftPad, ' ') + text;
}

void printCentered(const string &text, int width)
{
    cout << centerText(text, width) << "\n";
}

void printDivider(char ch, const string &color, int width)
{
    printCentered(color + string(width - 4, ch) + CLR_RESET, width);
}

// ---------------------------------------------------------------------------
// Scrollable keyboard menu
// ---------------------------------------------------------------------------
int promptScrollableMenu(const string &title,
                         const vector<string> &options,
                         int defaultIdx)
{
    int selected = defaultIdx;
    int total    = static_cast<int>(options.size());

    while (true)
    {
        clearScreen();
        printDivider('=', CLR_CYAN);
        printCentered(CLR_WHITE + "NUMBER SYSTEM CONVERTER SYSTEM" + CLR_RESET);
        printDivider('=', CLR_CYAN);
        cout << "\n";

        if (!title.empty())
        {
            printCentered(CLR_YELLOW + "============== " + title +
                          " ==============" + CLR_RESET);
            cout << "\n";
        }

        for (int i = 0; i < total; ++i)
        {
            if (i == selected)
                printCentered(CLR_CYAN + " ->  [ " + options[i] + " ] " + CLR_RESET);
            else
                printCentered(CLR_GRAY + "    " + options[i] + "   " + CLR_RESET);
        }

        cout << "\n";
        printDivider('-', CLR_GRAY);
        printDivider('-', CLR_GRAY);
        cout << "\n";

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            int arrow = _getch();
            if (arrow == 72)       selected = (selected - 1 + total) % total;
            else if (arrow == 80)  selected = (selected + 1) % total;
        }
        else if (ch == 'w' || ch == 'W')
            selected = (selected - 1 + total) % total;
        else if (ch == 's' || ch == 'S')
            selected = (selected + 1) % total;
        else if (ch == 13 || ch == '\n' || ch == ' ')
            return selected;
        else if (ch >= '1' && ch <= '9')
        {
            int numChoice = ch - '1';
            if (numChoice < total)
                return numChoice;
        }
    }
}

// ---------------------------------------------------------------------------
// Pause & validated integer input
// ---------------------------------------------------------------------------
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
            cin.ignore(10000, '\n');
            if (value >= minVal && value <= maxVal)
                return value;
            printCentered(CLR_RED + "[!] Error: Value must be between " +
                          to_string(minVal) + " and " + to_string(maxVal) +
                          "." + CLR_RESET);
        }
        else
        {
            printCentered(CLR_RED + "[!] Error: Invalid numeric input. Please try again." +
                          CLR_RESET);
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

// ---------------------------------------------------------------------------
// Tetris animation & header
// ---------------------------------------------------------------------------
void playTetrisAnimation(int totalFrames)
{
    const int ROWS = 7;
    const int COLS = 10;

    auto getBlockColor = [](int blockType) -> string
    {
        switch (blockType)
        {
        case 1: return CLR_YELLOW;
        case 2: return CLR_BLUE;
        case 3: return CLR_RED;
        case 4: return CLR_MAGENTA;
        case 5: return CLR_CYAN;
        case 9: return CLR_WHITE;
        default: return "";
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

        int board[ROWS][COLS] = {};

        // Yellow O-Piece
        int o1Row = min(frame, 5);
        board[o1Row][0] = 1; board[o1Row][1] = 1;
        board[o1Row+1][0] = 1; board[o1Row+1][1] = 1;

        // Blue J-Piece
        if (frame >= 6)
        {
            int jRow = min(frame - 6, 5);
            board[jRow][2] = 2;
            board[jRow+1][2] = 2; board[jRow+1][3] = 2; board[jRow+1][4] = 2;
        }

        // Red L-Piece
        if (frame >= 12)
        {
            int lRow = min(frame - 12, 5);
            board[lRow][7] = 3;
            board[lRow+1][5] = 3; board[lRow+1][6] = 3; board[lRow+1][7] = 3;
        }

        // Magenta O-Piece
        if (frame >= 18)
        {
            int o2Row = min(frame - 18, 5);
            board[o2Row][8] = 4; board[o2Row][9] = 4;
            board[o2Row+1][8] = 4; board[o2Row+1][9] = 4;
        }

        // Cyan I-Piece
        if (frame >= 24)
        {
            int iRow = min(frame - 24, 5);
            for (int c = 3; c <= 6; ++c) board[iRow][c] = 5;
        }

        // Line clear flash
        if (frame >= 30 && frame < 34)
        {
            int flash = (frame % 2 == 0) ? 9 : 5;
            for (int c = 0; c < COLS; ++c) { board[5][c] = flash; board[6][c] = flash; }
        }

        // Cleared lines
        if (frame >= 34)
            for (int c = 0; c < COLS; ++c) { board[5][c] = 0; board[6][c] = 0; }

        // Render board
        printCentered(CLR_GRAY + "+--------------------+" + CLR_RESET);
        for (int r = 0; r < ROWS; ++r)
        {
            string rowStr = CLR_GRAY + "| " + CLR_RESET;
            for (int c = 0; c < COLS; ++c)
            {
                int cell = board[r][c];
                rowStr += (cell == 0) ? "  " : (getBlockColor(cell) + "[]" + CLR_RESET);
            }
            rowStr += CLR_GRAY + " |" + CLR_RESET;
            printCentered(rowStr);
        }
        printCentered(CLR_GRAY + "+--------------------+" + CLR_RESET);

        cout << "\n";
        if (frame >= 30)
            printCentered(CLR_YELLOW + " DOUBLE TETRIS LINE CLEAR!  " + CLR_RESET);
        else
            printCentered(CLR_GRAY + "Loading Number System Converter... (Press any key to skip)" + CLR_RESET);

#ifdef _WIN32
        if (_kbhit()) { _getch(); break; }
#endif
        this_thread::sleep_for(chrono::milliseconds(70));
    }
}

void displayAsciiIntro()
{
    playTetrisAnimation(36);
}

void displayHeader()
{
    printDivider('=', CLR_CYAN);
    printCentered(CLR_WHITE + "NUMBER SYSTEM CONVERTER SYSTEM" + CLR_RESET);
    printDivider('=', CLR_CYAN);
    cout << "\n";
}
