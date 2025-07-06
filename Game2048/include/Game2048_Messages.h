//Windows消息处理和UI控件定义，定义按钮ID,消息处理函数,对话框处理等用户界面相关的声明

#pragma once

#include <windows.h>

//消息处理和按钮相关常量，定义UI控件的标识符和数量常量
namespace MessageConstants {
    //按钮标识符枚举，定义游戏中各个按钮的唯一标识符，用于消息处理中识别按钮
    enum ButtonID {
        NEWGAME = 0,     //新游戏按钮ID
        HINT = 1,        //提示按钮ID
        REDO = 2,        //撤销按钮ID
        HIGHSCORE = 3    //高分榜按钮ID
    };
    
    constexpr int BNUM = 4;                           //按钮总数常量，定义游戏中按钮的总数量，用于循环和数组大小
}

// 前向声明，避免头文件循环包含
class Game2048;

//消息处理函数命名空间，包含所有Windows消息处理相关的函数声明
namespace Game2048Messages {
    
    // ========== 窗口消息处理 ==========
    
    //主窗口消息处理函数，处理主窗口的所有Windows消息：WM_CREATE窗口创建时的初始化,WM_PAINT重绘游戏界面
     //          - WM_KEYDOWN: 键盘输入处理(方向键,快捷键)
     //          - WM_TIMER: 定时器消息(动画效果)
     //        - WM_MOUSEMOVE: 鼠标移动(按钮高亮效果)
    //         - WM_CLOSE: 窗口关闭确认
    LRESULT HandleMessage(Game2048* game, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    //子窗口(按钮)消息处理函数，处理自绘按钮的消息：WM_PAINT绘制按钮外观,WM_LBUTTONDOWN按钮点击处理,WM_MOUSEMOVE鼠标悬停效果
    LRESULT HandleChildMessage(Game2048* game, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    // ========== 对话框处理 ==========
    
    //关于对话框消息处理函数，处理游戏关于信息的对话框，显示版本信息,作者信息等
    INT_PTR HandleAboutDialog(Game2048* game, HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    
    //姓名输入对话框消息处理函数，处理高分记录时的姓名输入对话框，让玩家输入姓名以保存到高分榜
    INT_PTR HandleNameDialog(Game2048* game, HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    
    // ========== UI控件管理 ==========
    
    //刷新撤销按钮状态，根据当前撤销次数更新撤销按钮的可用性：有可撤销操作时按钮可用,无可撤销操作时按钮置灰,更新按钮文字显示剩余撤销次数
    void RefreshRedoButton(Game2048* game);
    
    //询问是否开始新游戏，显示确认对话框询问玩家是否开始新游戏：当前游戏进行中时显示警告,提供确认和取消选项,处理游戏结束时的特殊情况
    bool AskStartNewGame(Game2048* game);
}

// ========== 全局窗口过程函数 ==========

//主窗口全局窗口过程函数，Windows系统调用的标准窗口过程函数，将消息转发给Game2048类的消息处理函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//子窗口(按钮)全局窗口过程函数，处理自绘按钮的Windows消息，将消息转发给Game2048类的子窗口消息处理函数
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
