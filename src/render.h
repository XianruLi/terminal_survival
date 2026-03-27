#pragma once
#include <string>

// --- ANSI 魔法指令定义 (工程里的特殊寄存器地址) ---
#define ANSI_CLEAR "\033[H\033[2J"  // 清除整个屏幕并将光标归位 (0,0)
#define ANSI_GOTO(y, x) "\033[" << y << ";" << x << "H" // 将光标移到指定坐标
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"

// 为了兼容 MinGW 跨平台，我们需要手动实现清屏函数
void clearScreen();
void drawHLine(int y, int x, int length);
void drawVLine(int x, int y, int length);
void drawBox(int y, int x, int width, int height);

// 这个函数负责根据你的朋友苏晴的数据，在屏幕上的固定位置画出来
void drawCompanionInfo(int y, int x, const std::string& name, int trust);
