// main.cpp — Entry point only.
// All logic lives in src/ and include/.

#include <iostream>
#include <string>
#include <vector>

#include "include/globals.h"
#include "include/prototypes.h"

using namespace std;

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
            printCentered("Activity: Number System Converter (Activity No. 1)         ");
            printCentered("Supported Bases: Binary(2), Octal(8), Decimal(10), Hex(16)  ");
            printCentered("Minimum Input Requirement: 3 Numbers (Accepts N >= 3)      ");
            printCentered("Validation: Strict Character-by-Character Radix Checking   ");
            printCentered("Precision: Full Integer & Radix Fractional Support         ");
            printCentered("Arithmetic: Addition, Subtraction, Multiplication, Division ");
            printCentered("Complements: 1's & 2's (binary), 9's & 10's (BCD)          ");
            printCentered("BCD Ops: BCD Addition, 9's & 10's Complement Subtraction   ");
            printCentered("Navigation: Scrollable Menus using Arrow Keys & W/S        ");
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
