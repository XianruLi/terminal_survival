// 函数实现
// 把之前写的所有业务逻辑（函数内部的具体代码）全搬到这里。
#include "game_logic.h"
#include <iostream>
#include <fstream> // 用于读写文件，即存档
#include <cstdlib>

using namespace std;

void saveGame(const Player& p, const Shelter& s, const vector<Companion>& girls, int day) {
    ofstream outFile("savegame.txt"); 
    if (outFile.is_open()) {
        outFile << day << "\n";
        outFile << p.stamina << " " << p.hunger << "\n";
        outFile << p.inv.wood << " " << p.inv.stone << " " << p.inv.food << " " << p.inv.axe << "\n";
        outFile << s.level << "\n";
        for (size_t i = 0; i < girls.size(); i++) {
            outFile << girls[i].trust << " " << girls[i].questAmount << "\n";
        }
        outFile.close();
        cout << "\n>>> [系统] 游戏进度已成功保存到 savegame.txt！\n";
    }
}

bool loadGame(Player& p, Shelter& s, vector<Companion>& girls, int& day) {
    ifstream inFile("savegame.txt"); 
    if (inFile.is_open()) {
        inFile >> day;
        inFile >> p.stamina >> p.hunger;
        inFile >> p.inv.wood >> p.inv.stone >> p.inv.food >> p.inv.axe;
        inFile >> s.level;
        for (size_t i = 0; i < girls.size(); i++) {
            inFile >> girls[i].trust >> girls[i].questAmount;
        }
        inFile.close();
        return true; 
    }
    return false; 
}

void upgradeShelter(Player& p, Shelter& s) {
    cout << "\n--- 营地建设 ---\n当前状态: " << s.getName() << "\n";
    if (s.level == 0) {
        cout << "下一级: [简易木棚] (需要 5 木头)\n是否升级？(1:是 0:否): ";
        int choice; cin >> choice;
        if (choice == 1 && p.inv.wood >= 5) { p.inv.wood -= 5; s.level = 1; cout << "\n>>> 搭建成功！\n"; }
    } else if (s.level == 1) {
        cout << "下一级: [坚固木屋] (需要 10 木头, 5 石头)\n是否升级？(1:是 0:否): ";
        int choice; cin >> choice;
        if (choice == 1 && p.inv.wood >= 10 && p.inv.stone >= 5) { p.inv.wood -= 10; p.inv.stone -= 5; s.level = 2; cout << "\n>>> 搭建成功！\n"; }
    }
}

void interactWithCompanions(Player& p, vector<Companion>& girls) {
    cout << "\n--- 营地同伴 ---\n";
    for (size_t i = 0; i < girls.size(); i++) {
        cout << i + 1 << ". " << girls[i].name << " - 信任度: " << girls[i].trust << " (需要: " << girls[i].questAmount << " " << girls[i].questType << ")\n";
    }
    cout << "你想找谁搭话？(输入编号, 0返回): ";
    int choice; cin >> choice;
    if (choice > 0 && choice <= static_cast<int>(girls.size())) {
        Companion& target = girls[choice - 1];
        bool questComplete = false;
        if (target.questType == "food" && p.inv.food >= target.questAmount) { p.inv.food -= target.questAmount; questComplete = true; }
        else if (target.questType == "wood" && p.inv.wood >= target.questAmount) { p.inv.wood -= target.questAmount; questComplete = true; }
        else if (target.questType == "stone" && p.inv.stone >= target.questAmount) { p.inv.stone -= target.questAmount; questComplete = true; }

        if (questComplete) {
            target.trust += 20; target.questAmount += 2;
            cout << "\n>>> " << target.name << " 很高兴！信任度提升！\n";
        } else {
            cout << "\n>>> " << target.name << " 还在等你凑齐物资。\n";
        }
    }
}

void nightPhase(Player& p, Shelter& s) {
    cout << "\n夕阳西下...\n"; p.hunger -= 15; 
    if (rand() % 100 < 40) { 
        cout << "\n【夜间危机】野兽袭击！\n";
        if (s.level == 0) { p.stamina -= 40; cout << "你在沙滩上受了重伤。\n"; }
        else if (s.level == 1) { p.stamina -= 15; cout << "木棚挡住了一部分攻击。\n"; }
        else if (s.level == 2) { cout << "坚固的木屋保护了你们，毫发无损！\n"; }
    }
    p.stamina = min(100, p.stamina + 30 + (s.level * 10)); 
}
