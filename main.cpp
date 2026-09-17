#include <cstdio>
#include <windows.h>
#include "Function.h"

//常用颜色
#define RESET_COLOR   "\033[0m"      // 重置颜色
#define COLOR_GREEN   "\033[32m"       // 标准亮绿色
//自定义颜色
#define COLOR_DEEP_GREEN   "\033[38;2;10;180;10m"   // 深绿色

signed main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        GetConsoleMode(hOut, &mode);
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    printf(COLOR_DEEP_GREEN ">>> PROGRAM STARTED <<<" COLOR_GREEN "\n");
    
    Function func;
    func.choose(func);

    return 0;
}
