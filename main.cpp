// main.cpp - Entry point for the mathematical function calculator
// Supports: Ackermann, Fibonacci, Euler's totient, Catalan numbers,
// and Buchholz Ordinal Collapsing Function (OCF)

#include <cstdio>
#include <windows.h>
#include "Function.h"

// Common colors
#define RESET_COLOR   "\033[0m"      // Reset color
#define COLOR_GREEN   "\033[32m"       // Standard bright green
// Custom colors
#define COLOR_DEEP_GREEN   "\033[38;2;10;180;10m"   // Deep green

// Program entry point
// Configures console for UTF-8 output and ANSI color support,
// then launches the interactive function menu
signed main() {
    // Set console code pages to UTF-8 for proper character display
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Enable VT100 escape sequence processing for terminal colors
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        GetConsoleMode(hOut, &mode);
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    printf(COLOR_DEEP_GREEN ">>> PROGRAM STARTED <<<" COLOR_GREEN "\n");
    
    // Create Function instance and run interactive menu
    Function func;
    func.choose(func);

    return 0;
}
