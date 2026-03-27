#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "game_data.h"
#include "game_logic.h"
#include "render.h" // 引入新的显示引擎

// 如果是编译为 Windows 程序，就引入 Windows 的专属词典
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

int main() {
    srand(time(0)); 
    Player p; Shelter shelter;
    vector<Companion> companions;
    companions.push_back({"苏晴", "温柔贤惠", "food", 3, 31}); // 初始信任高点
    companions.push_back({"林月", "活泼好动", "wood", 5, 20});

    int day = 1;
    string action;
    string lastLog = "你从坠机的沙滩上醒来，一片茫然。"; // [新增] 用于存储上一条事件的日志

    // 在游戏启动前开启 Windows 的 ANSI 支持（如果需要交叉编译给朋友）
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif

    // 主菜单暂时保持 CLI 风格，一旦进入游戏循环就转为 TUI
    cout << "======================================\n";
    cout << "  《终端求生：Metroid-like Roguelike版》\n";
    cout << "======================================\n";
    cout << "1. 开始新游戏\n";
    int startChoice; cin >> startChoice;

    while (true) {
        // --- 第一步：渲染全屏画面 ---
        clearScreen(); // 清屏
        
        // 1. 画主场景大框 (左边)
        drawBox(1, 1, 40, 15);
        std::cout << ANSI_GOTO(2, 2) << "--- 荒岛场景 (Metroid风格) ---";
        std::cout << ANSI_GOTO(8, 18) << ANSI_COLOR_RED << "@" << ANSI_COLOR_RESET; // 先画个主角符号占坑
        
        // 2. 画状态栏大框 (右边)
        drawBox(1, 42, 38, 15);
        std::cout << ANSI_GOTO(2, 43) << "[第 " << day << " 天 白天]";
        std::cout << ANSI_GOTO(4, 43) << ANSI_COLOR_RED << "体力: " << p.stamina << ANSI_COLOR_RESET;
        std::cout << ANSI_GOTO(5, 43) << ANSI_COLOR_BLUE << "饱腹: " << p.hunger << ANSI_COLOR_RESET;
        std::cout << ANSI_GOTO(7, 43) << "[背包] 木: " << p.inv.wood << " 石: " << p.inv.stone << " 食: " << p.inv.food;
        
        // 3. 画同伴区域 (右边下半部分)
        std::cout << ANSI_GOTO(10, 43) << "--- 同伴 ---";
        drawCompanionInfo(11, 43, companions[0].name, companions[0].trust);
        drawCompanionInfo(12, 43, companions[1].name, companions[1].trust);

        // 4. 画日志框 (最下方)
        drawBox(16, 1, 80, 5);
        std::cout << ANSI_GOTO(17, 2) << "[日志] " << lastLog;

        // 5. 画指令栏 (底部)
        std::cout << ANSI_GOTO(21, 1) << "行动: [explore]探索 [talk]交谈 [night]夜晚 > ";

        // --- 第二步：等待输入 (这里暂时保留 std::cin) ---
        cin >> action;

        // --- 第三步：逻辑更新 ---
        if (action == "explore") {
            if (p.stamina >= 20) {
                p.stamina -= 20; p.hunger -= 10;
                p.inv.wood += rand() % 2; p.inv.food += 1;
                lastLog = "你在附近探索，发现了一些食物和木头。"; // 更新日志，而不是打印它
            } else {
                lastLog = "[系统] 你太累了，无法探索！";
            }
        } 
        else if (action == "night") {
            lastLog = "夕阳西下，你们躲入营地，度过了惊心动魄的一晚。";
            day++; 
        }
        else if (action == "quit") {
            break;
        }
        
        // 死亡判定
        if (p.stamina <= 0) {
            clearScreen();
            cout << ANSI_GOTO(10, 30) << ANSI_COLOR_RED << "GAME OVER" << ANSI_COLOR_RESET;
            break;
        }
    }

    return 0;
}
