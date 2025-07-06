#pragma once

#include <vector>
#include <array>
#include <string>
#include <fstream>

//高分记录结构，存储单个高分记录，包含玩家姓名和分数
struct HighScore {
    std::string name;      //玩家姓名
    unsigned long score;   //得分
    
    //默认构造函数
    HighScore() : score(0) {}
    
    //带参数构造函数
    HighScore(const std::string& n, unsigned long s) : name(n), score(s) {}
};

//游戏状态结构，存储游戏的完整状态，包括分数和游戏板状态
struct GameState {
    unsigned long score;                               //当前分数
    std::array<std::array<unsigned int, 4>, 4> num;  //4x4游戏板状态
    
    //默认构造函数，初始化分数为0，游戏板为空
    GameState() : score(0) {
        for (auto& row : num) {
            row.fill(0);
        }
    }
};

//数据管理相关常量，定义高分榜大小,分数阈值等数据相关的常量
namespace DataConstants {
    constexpr int HIGHSCORE_COUNT = 10;               //高分榜记录数量
    constexpr int HIGHSCORE_THRESHOLD = 0;            //高分阈值，降低阈值更容易记录历史分数
    constexpr int MAX_NAME_LENGTH = 64;               //玩家姓名最大长度
    constexpr int LONG_STRING_LENGTH = 256;           //长字符串最大长度
}

// 数据管理类的声明部分
class Game2048;

// 数据管理相关方法声明
namespace Game2048Data {
    // 游戏数据保存和加载
    void SaveGame(Game2048* game);                    //保存当前游戏状态到文件
    void LoadGame(Game2048* game);                    //从文件加载游戏状态
    void SaveRecord(Game2048* game);                  //保存游戏记录到文件
    
    // 高分管理
    bool InHighScore(const Game2048* game);           //检查当前分数是否达到高分榜标准
    void UpdateHighScore(Game2048* game, const std::string& name); //更新高分榜记录
    bool JudgeRefreshHighScore(Game2048* game);       //判断是否需要刷新高分显示
    
    // 辅助函数
    bool IsAllZero(const std::array<std::array<unsigned int, 4>, 4>& board); //检查棋盘是否为空
    void CopyBoard(std::array<std::array<unsigned int, 4>, 4>& dest,         //复制棋盘数据
                   const std::array<std::array<unsigned int, 4>, 4>& src);
}
