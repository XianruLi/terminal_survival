#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream> // [新增] 用于读写文件

using namespace std;

struct Inventory {
    int wood = 0;
    int stone = 0;
    int food = 0;
    int axe = 0;
};

struct Player {
    string name = "你";
    int stamina = 100;
    int hunger = 100;
    Inventory inv;
};

struct Shelter {
    int level = 0; 
    string getName() {
        if (level == 0) return "沙滩露营 (防御:极弱)";
        if (level == 1) return "简易木棚 (防御:中等)";
        return "坚固木屋 (防御:极强)";
    }
};

struct Companion {
    string name;
    string personality;
    string questType;  
    int questAmount;   
    int trust = 0;     
};

// ---------------------------------------------------------
// [新增] 保存与读取模块
// ---------------------------------------------------------

// 保存游戏 (把数据写进文件)
void saveGame(const Player& p, const Shelter& s, const vector<Companion>& girls, int day) {
    // 创建一个名为 savegame.txt 的输出流 (传送带向外运)
    ofstream outFile("savegame.txt"); 
    
    if (outFile.is_open()) {
        // 按照固定的顺序，把数据写进文本文件
        outFile << day << "\n";
        outFile << p.stamina << " " << p.hunger << "\n";
        outFile << p.inv.wood << " " << p.inv.stone << " " << p.inv.food << " " << p.inv.axe << "\n";
        outFile << s.level << "\n";
        
        // 循环保存所有女主的数据
        for (int i = 0; i < girls.size(); i++) {
            outFile << girls[i].trust << " " << girls[i].questAmount << "\n";
        }
        
        outFile.close(); // 用完记得关闭文件
        cout << "\n>>> [系统] 游戏进度已成功保存到 savegame.txt！\n";
    } else {
        cout << "\n[错误] 无法创建保存文件！\n";
    }
}

// 读取游戏 (从文件把数据读进内存)
// 注意：所有的参数都加了 & (引用)，因为我们要把读到的数据塞回真正的变量里
bool loadGame(Player& p, Shelter& s, vector<Companion>& girls, int& day) {
    // 创建一个输入流 (传送带向里运)
    ifstream inFile("savegame.txt"); 
    
    if (inFile.is_open()) {
        // 必须严格按照刚才保存的顺序读取！
        inFile >> day;
        inFile >> p.stamina >> p.hunger;
        inFile >> p.inv.wood >> p.inv.stone >> p.inv.food >> p.inv.axe;
        inFile >> s.level;
        
        for (int i = 0; i < girls.size(); i++) {
            inFile >> girls[i].trust >> girls[i].questAmount;
        }
        
        inFile.close();
        return true; // 读取成功
    }
    return false; // 如果文件不存在，返回失败
}

// ---------------------------------------------------------
// 之前的模块 (保持不变，省略了部分打印文字使代码紧凑)
// ---------------------------------------------------------
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
    for (int i = 0; i < girls.size(); i++) {
        cout << i + 1 << ". " << girls[i].name << " - 信任度: " << girls[i].trust << " (需要: " << girls[i].questAmount << " " << girls[i].questType << ")\n";
    }
    cout << "你想找谁搭话？(输入编号, 0返回): ";
    int choice; cin >> choice;
    if (choice > 0 && choice <= girls.size()) {
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

// ---------------------------------------------------------
// 主循环
// ---------------------------------------------------------
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
    
    // [新增] 游戏启动时的读档询问
    cout << "1. 开始新游戏\n";
    cout << "2. 读取本地存档\n";
    cout << "请选择: ";
    int startChoice; cin >> startChoice;
    
    if (startChoice == 2) {
        if (loadGame(p, shelter, companions, day)) {
            cout << ">>> 读取成功！欢迎回到荒岛，现在是第 " << day << " 天。\n";
        } else {
            cout << ">>> 没有找到存档文件，将开始新游戏...\n";
        }
    }

    while (isPlaying) {
        cout << "\n======================================";
        cout << "\n[第 " << day << " 天 白天] | 体力: " << p.stamina << " | 饱腹: " << p.hunger << "\n";
        cout << "[背包] 木头: " << p.inv.wood << " | 石头: " << p.inv.stone << " | 食物: " << p.inv.food << "\n";
        cout << "[营地] " << shelter.getName() << "\n";
        cout << "--------------------------------------\n";
        // 加入了 save 指令
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
        else if (action == "save") {
            // 调用保存功能
            saveGame(p, shelter, companions, day);
        }
        else if (action == "night") { nightPhase(p, shelter); day++; }
        else if (action == "quit") { isPlaying = false; }

        if (p.hunger <= 0 || p.stamina <= 0) {
            cout << "\n你倒在了荒岛上，再也没有醒来。游戏结束。\n";
            isPlaying = false;
        }
    }

    return 0;
}
