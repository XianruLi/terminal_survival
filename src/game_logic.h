// 函数声明
// 这个文件告诉别人我们有哪些功能可以调用。

#pragma once
#include "game_data.h" // 引入刚才写的数据结构
#include <vector>

// 仅仅声明函数名字和需要的参数，后面加分号结束
void upgradeShelter(Player& p, Shelter& s);
void interactWithCompanions(Player& p, std::vector<Companion>& girls);
void nightPhase(Player& p, Shelter& s);
void saveGame(const Player& p, const Shelter& s, const std::vector<Companion>& girls, int day);
bool loadGame(Player& p, Shelter& s, std::vector<Companion>& girls, int& day);
