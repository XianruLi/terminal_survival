// 文件包含结构体：玩家、背包、女主

#pragma once // 魔法指令：防止这个文件被重复加载导致报错
#include <string>
#include <vector>

// 为了规范，我们在头文件里用 std::string，而不是直接 using namespace std
struct Inventory {
    int wood = 0;
    int stone = 0;
    int food = 0;
    int axe = 0;
};

struct Player {
    std::string name = "你";
    int stamina = 100;
    int hunger = 100;
    Inventory inv;
};

struct Shelter {
    int level = 0;
    std::string getName() {
        if (level == 0) return "沙滩露营 (防御:极弱)";
        if (level == 1) return "简易木棚 (防御:中等)";
        return "坚固木屋 (防御:极强)";
    }
};

struct Companion {
    std::string name;
    std::string personality;
    std::string questType;
    int questAmount;
    int trust = 0;
};
