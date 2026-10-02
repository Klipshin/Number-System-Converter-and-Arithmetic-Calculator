#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
using namespace std;

// Hex digit lookup table
const string DIGITS = "0123456789ABCDEF";

// Console width (columns)
const int TERMINAL_WIDTH = 92;

// ANSI color escape codes
const string CLR_RESET   = "\033[0m";
const string CLR_RED     = "\033[1;31m";
const string CLR_GREEN   = "\033[1;32m";
const string CLR_YELLOW  = "\033[1;33m";
const string CLR_BLUE    = "\033[1;34m";
const string CLR_MAGENTA = "\033[1;35m";
const string CLR_CYAN    = "\033[1;36m";
const string CLR_WHITE   = "\033[1;37m";
const string CLR_GRAY    = "\033[90m";

#endif // GLOBALS_H
