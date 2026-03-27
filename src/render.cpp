#include "render.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

void clearScreen() {
#ifdef _WIN32
    // Windows 下可能需要特殊的 ANSI 启用，简单起见我们这里用系统命令
    system("cls");
#else
    // Linux 下直接发送魔法指令清屏，效率极高
    std::cout << ANSI_CLEAR;
#endif
}

void drawHLine(int y, int x, int length) {
    std::cout << ANSI_GOTO(y, x);
    for (int i = 0; i < length; i++) std::cout << "─"; // 使用制表符
}

void drawVLine(int x, int y, int height) {
    for (int i = 0; i < height; i++) {
        std::cout << ANSI_GOTO(y + i, x) << "│";
    }
}

void drawBox(int y, int x, int width, int height) {
    drawHLine(y, x + 1, width - 2);
    drawHLine(y + height - 1, x + 1, width - 2);
    drawVLine(x, y + 1, height - 2);
    drawVLine(x + width - 1, y + 1, height - 2);
    
    // 画角
    std::cout << ANSI_GOTO(y, x) << "┌";
    std::cout << ANSI_GOTO(y, x + width - 1) << "┐";
    std::cout << ANSI_GOTO(y + height - 1, x) << "└";
    std::cout << ANSI_GOTO(y + height - 1, x + width - 1) << "┘";
}

void drawCompanionInfo(int y, int x, const std::string& name, int trust) {
    std::cout << ANSI_GOTO(y, x) << name << " (" << trust << ")";
    // 根据信任度改变颜色
    if (trust > 30) {
        std::cout << " [" << ANSI_COLOR_GREEN << "信任" << ANSI_COLOR_RESET << "]";
    } else {
        std::cout << " [" << ANSI_COLOR_BLUE << "警戒" << ANSI_COLOR_RESET << "]";
    }
}
