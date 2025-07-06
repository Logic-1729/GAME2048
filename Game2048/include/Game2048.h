#pragma once

// 标准库头文件
#include <iostream>    
#include <vector>       // 动态数组容器
#include <array>        // 固定大小数组容器
#include <string>      
#include <fstream>      
#include <memory>       // 智能指针
#include <algorithm>    // 算法库
#include <sstream>      // 字符串流

// Windows API头文件
#include <windows.h>    // Windows基础API
#include <commctrl.h>   // Windows通用控件
#include <tchar.h>      // 字符类型宏

// 项目头文件
#include "ControlStyle.h"         // UI控件样式定义
#include "Game2048_Data.h"        // 游戏数据结构定义
#include "Game2048_Logic.h"       // 游戏逻辑算法
#include "Game2048_Messages.h"    // 消息和常量定义
#include "Game2048_UI.h"          // UI相关常量和函数

//全局游戏实例指针,用于在Windows消息回调函数中访问游戏实例
extern std::unique_ptr<Game2048> g_pGame;

//2048游戏主类，继承纯逻辑类并添加UI功能
class Game2048 : public Game2048Logic {
private:
    HWND m_hwnd;                                            //游戏主窗口的Windows句柄，用于窗口操作和消息处理
    HWND m_hwndButtons[MessageConstants::BNUM];             //存储所有按钮（新游戏,撤销,提示,关于,高分）的窗口句柄
    HINSTANCE m_hInst;                                      //Windows应用程序实例，用于创建窗口和资源管理
    
    
    // UI相关的历史数据和高分记录（使用不同的数据结构）
    std::vector<HighScore> m_highScores;                    //存储历史最高分记录，用于显示高分榜
    
    
    // UI相关成员变量
    int m_onMouse;                                          //记录当前鼠标悬停在哪个按钮上，-1表示无悬停
    std::array<RECT, MessageConstants::BNUM> m_rectButtons; //存储所有按钮的矩形区域坐标，用于绘制和点击检测
    std::array<std::array<RECT, 4>, 4> m_rectCells;         //4x4的矩形数组，存储每个游戏格子的绘制区域坐标
    

    RECT m_rectName, m_rectScore, m_rectBest;               //分别存储名称,分数,最高分等UI元素的绘制区域
    RECT m_rectScoreTitle, m_rectScoreNum;                  // 分数标题和数字区域
    RECT m_rectBestTitle, m_rectBestNum;                    // 最高分标题和数字区域
    RECT m_rectMain, m_rectVCenterText, m_rectBottomText;   // 主要区域和文本区域
    
    // ========== 系统参数 ==========
    
    // 屏幕和窗口尺寸参数，包含屏幕宽高,标题栏高度,边框宽度等系统参数
    int m_cxScreen, m_cyScreen, m_cyCaption, m_cxSizeFrame, m_cySizeFrame;
    
    // UI布局参数：m_iBlock游戏格子的大小，m_buttonW按钮的宽度
    int m_iBlock, m_buttonW;
    
    // ========== 字符串和随机数 ==========
    
    // 文件路径，存储游戏数据文件的完整路径
    std::string m_filePath;
    
    // 临时字符串缓冲区，用于字符串转换和临时存储，避免频繁分配内存
    mutable std::string m_tempBuffer;
    
    // ========== 按钮配置 ==========
    
    // 按钮名称结构体，存储按钮显示的文本名称
    struct ButtonName {
        std::string name;  // 按钮显示的文本
    };
    
    // 按钮名称数组，存储所有按钮的显示文本
    std::array<ButtonName, MessageConstants::BNUM> m_buttonNames;

public:
    // ========== 构造和析构 ==========
    
    // 构造函数，初始化游戏对象，设置默认值
    Game2048();
    
    // 析构函数，清理资源，保存游戏数据
    ~Game2048();
    
    // ========== 初始化和清理 ==========
    
    // 初始化游戏，创建游戏窗口，初始化UI控件，加载游戏数据
    bool Initialize(HINSTANCE hInstance);
    
    // 清理资源，释放窗口资源，保存游戏状态
    void Cleanup();
    
    // ========== 核心游戏逻辑 ==========
    
    // 开始新游戏，重置游戏到初始状态：清空4x4游戏棋盘,重置分数为0,清空撤销历史记录,生成初始数字
    void NewGame();
    
    // 随机游戏模式，清空当前棋盘,随机生成2-4个初始数字在随机位置,设置随机模式标志
    void RandomGame();
    
    // 撤销操作，回退到上一个游戏状态：检查历史记录,恢复棋盘布局和分数,更新计数器
    void Redo();
    
    // 检查游戏是否结束，判断条件：棋盘完全填满且无法进行任何有效的合并操作
    bool IsGameOver() const;
    
    // 添加新数字到棋盘，在空格中随机添加数字2(90%概率)或4(10%概率)
    void AddNewNumber();
    
    // 处理移动操作，执行指定方向的移动和合并操作，返回移动是否成功
    bool ProcessMove(int direction);
    
    // 智能提示功能，随机尝试有效移动方向
    // 返回值：0=游戏已结束无法移动, 1=成功移动, -1=无有效移动
    int Hint();
    
    // 根据当前棋盘状态选择移动方向
    int ChooseDirection() const;
    
    // 检测是否达到2048，棋盘中存在2048或更大的数字时返回true
    bool HasReached2048() const;
    
    // ========== 数据管理 ==========
    
    // 保存游戏状态到文件，将棋盘,分数,撤销历史等完整游戏状态持久化到2048.SAV文件
    void SaveGame();
    
    // 从文件加载游戏状态，从存档文件恢复游戏，如果文件损坏或不存在则启动新游戏
    void LoadGame();
    
    // 保存记录数据，保存高分榜等数据到文件
    void SaveRecord();
    
    // 检查棋盘是否为空，棋盘全为0时返回true
    bool IsAllZero(const std::array<std::array<unsigned int, 4>, 4>& board) const;
    
    // 复制棋盘数据，将源棋盘的数据完全复制到目标棋盘
    void CopyBoard(std::array<std::array<unsigned int, 4>, 4>& dest, 
                   const std::array<std::array<unsigned int, 4>, 4>& src) const;
    
    // ========== 高分管理系统 ==========
    
    // 检查当前分数是否达到高分榜标准，比较当前分数与高分榜最后一名的分数
    bool InHighScore() const;
    
    // 更新高分榜记录，将当前分数插入到高分榜合适位置并保存到文件
    void UpdateHighScore(const std::string& name);
    
    // 判断是否需要刷新高分显示，检查当前分数是否超过了历史最高分
    bool JudgeRefreshHighScore();
    
    // ========== 用户界面和绘制系统 ==========
    
    // 初始化用户界面，设置UI布局和控件：计算UI元素位置,创建按钮,初始化字体和颜色
    void InitializeUI();
    
    // 绘制整个游戏界面，包括标题,分数,网格,数字方块和按钮
    void DrawGame(HDC hdc);
    
    // 绘制文本到指定区域，使用ASCII字符集的底层文本绘制实现
    void DrawText(HDC hdc, const std::string& text, const RECT& rect, 
                  long fontSize, int fontWeight, COLORREF color, UINT format) const;
    
    // 填充矩形区域，用指定颜色填充矩形，用于绘制背景和按钮
    void FillRect(HDC hdc, const RECT& rect, COLORREF color) const;
    
    // 刷新撤销按钮状态，根据撤销次数更新按钮的可用性和外观
    void RefreshRedoButton();
    
    // 刷新主游戏区域，重绘游戏棋盘区域，通常在数字变化后调用
    void RefreshMainRect();
    
    // ========== 辅助工具函数 ==========
    
    // 根据数字获取对应的颜色，不同数字有不同的视觉区分(2=浅灰,4=深灰,8=橙色等)
    COLORREF GetNumberColor(unsigned int number) const;
    
    // 将数字转换为字符串，简单的数字到字符串转换，用于界面显示
    std::string NumberToString(unsigned int number) const;
    
    // 获取程序版本信息，从可执行文件中提取版本信息并格式化显示
    void GetFileVersion(const std::string& format, std::string& result) const;
    
    // 询问是否开始新游戏，显示确认对话框，用户可以选择确认或取消
    bool AskStartNewGame();
    
    // 处理游戏结束逻辑，统一处理游戏结束时的逻辑：检查2048,高分榜,显示对话框
    void HandleGameOver(HWND parentHwnd);
    
    // 游戏结束时的新处理函数，先显示MessageBox然后更新高分榜，界面保持不动
    void HandleGameEndAndUpdateScores();
    
    // UI层的Hint包装函数，处理所有Windows API相关的操作
    void HintWithUI();

    // ========== Windows消息处理系统 ==========
    
    // 主窗口消息处理函数，处理重绘,键盘输入,定时器,鼠标移动,窗口关闭等消息
    LRESULT HandleMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    // 子窗口(按钮)消息处理函数，处理按钮点击,重绘,鼠标悬停等消息
    LRESULT HandleChildMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    // ========== 公共访问器 ==========
    
    // 获取主窗口句柄，用于外部代码需要访问主窗口时使用
    HWND GetMainWindow() const { return m_hwnd; }
    
    // 获取应用程序实例句柄，用于创建子窗口或访问应用程序资源
    HINSTANCE GetInstance() const { return m_hInst; }
};
