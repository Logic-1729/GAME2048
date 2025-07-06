// 2048游戏核心逻辑算法定义（纯逻辑版本，不依赖Windows API）
#pragma once

// 标准库头文件（逻辑层只需要这些）
#include <array>        // 固定大小数组容器
#include <vector>       // 动态数组容器
#include <algorithm>    // 算法库（用于 std::max）

// 项目头文件
#include "Game2048_Data.h"        // 需要GameState的完整定义

// 游戏逻辑相关常量定义
namespace LogicConstants {
    constexpr int REDO_NUM = 20;                      // 最大撤销次数，游戏中允许的最大撤销操作次数，防止无限回退
}

// 纯逻辑类声明（不依赖Windows API）
class Game2048Logic {
public:
    // 游戏板相关
    std::array<std::array<unsigned int, 4>, 4> m_gameBoard;
    
    // 分数相关
    unsigned long m_score = 0;
    unsigned long m_highScore = 0;
    
    // 撤销相关
    std::array<GameState, LogicConstants::REDO_NUM> m_redoHistory;
    unsigned int m_redoCount = 0;
    unsigned int m_step = 0;
    
public:
    // 构造函数
    Game2048Logic();
    
    // ========== 核心游戏逻辑 ==========
    
    // 处理移动操作，执行指定方向的移动和合并操作，返回移动是否成功
    bool ProcessMove(int direction);
    
    // 添加新数字到棋盘，在空格中随机添加数字2(90%概率)或4(10%概率)
    void AddNewNumber();
    
    // 新游戏初始化，清空棋盘，重置分数，添加两个初始数字
    void NewGame();
    
    // 智能提示功能，随机尝试有效移动方向（纯逻辑版本）
    // 返回值：0=游戏已结束无法移动, 1=成功移动, -1=无有效移动
    int Hint();
    
    // 根据当前棋盘状态选择移动方向
    int ChooseDirection() const;
    
    // ========== 游戏状态检查 ==========
    
    // 检查游戏是否结束，无空格且无法合并时返回true
    bool IsGameOver() const;
    
    // 检测是否达到2048，棋盘中存在2048或更大的数字时返回true
    bool HasReached2048() const;
    
    // ========== 撤销系统 ==========
    
    // 撤销操作，恢复到上一步状态
    void Redo();
    
    // 保存当前状态到撤销历史
    void SaveRecord();
    
    // ========== 工具函数 ==========
    
    // 检查棋盘是否全为0
    bool IsAllZero(const std::array<std::array<unsigned int, 4>, 4>& board) const;
    
    // 复制棋盘
    void CopyBoard(std::array<std::array<unsigned int, 4>, 4>& dest, 
                   const std::array<std::array<unsigned int, 4>, 4>& src) const;
};
