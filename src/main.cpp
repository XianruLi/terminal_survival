#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "game_data.h"
#include "game_logic.h"

using namespace std;

int main() {
    srand(time(0)); 
    Player p; Shelter shelter;
    vector<Companion> companions;
    companions.push_back({"苏晴", "温柔贤惠", "food", 3, 10});
    companions.push_back({"林月", "活泼好动", "wood", 5, 5});
    companions.push_back({"楚冰", "高冷警戒", "stone", 2, 0});

    bool isPlaying = true;
    int day = 1;
    string action;

    cout << "======================================\n";
    cout << "  欢迎来到《终端求生：荒岛坠机》\n";
    cout << "======================================\n";
    
    cout << "1. 开始新游戏\n2. 读取本地存档\n请选择: ";
    int startChoice; cin >> startChoice;
    
    if (startChoice == 2 && loadGame(p, shelter, companions, day)) {
        cout << ">>> 读取成功！欢迎回到荒岛，现在是第 " << day << " 天。\n";
    }

    while (isPlaying) {
        cout << "\n======================================";
        cout << "\n[第 " << day << " 天 白天] | 体力: " << p.stamina << " | 饱腹: " << p.hunger << "\n";
        cout << "[背包] 木头: " << p.inv.wood << " | 石头: " << p.inv.stone << " | 食物: " << p.inv.food << "\n";
        cout << "[营地] " << shelter.getName() << "\n";
        cout << "--------------------------------------\n";
        cout << "行动: [explore]探索  [build]建造  [talk]交谈  [eat]进食  [night]夜晚  [save]存档\n";
        cout << "你想做什么？> ";

        cin >> action;

        if (action == "explore") {
            if (p.stamina >= 20) {
                p.stamina -= 20; p.hunger -= 10;
                p.inv.wood += rand() % 4; p.inv.stone += rand() % 3; p.inv.food += rand() % 2 + 1;
                cout << "\n你搜索了一番，物资增加了！\n";
            }
        } 
        else if (action == "build") { upgradeShelter(p, shelter); }
        else if (action == "talk") { interactWithCompanions(p, companions); }
        else if (action == "eat") {
            if (p.inv.food > 0) { p.inv.food -= 1; p.stamina = min(100, p.stamina + 30); p.hunger = min(100, p.hunger + 40); }
        }
        else if (action == "save") { saveGame(p, shelter, companions, day); }
        else if (action == "night") { nightPhase(p, shelter); day++; }
        else if (action == "quit") { isPlaying = false; }

        if (p.hunger <= 0 || p.stamina <= 0) {
            cout << "\n你倒在了荒岛上，再也没有醒来。游戏结束。\n";
            isPlaying = false;
        }
    }
    return 0;
}
