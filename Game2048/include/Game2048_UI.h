//用户界面布局和绘制系统定义，定义UI常量,颜色方案,布局参数和绘制函数声明

#pragma once

#include <windows.h>
#include <string>

//用户界面布局常量，定义窗口尺寸,控件大小,边距等UI布局相关的常量
namespace UIConstants {
    constexpr int WINDOW_WIDTH = 400;                 //游戏窗口宽度(像素)，主窗口的标准宽度，包含游戏区域和边距
    constexpr int WINDOW_HEIGHT = 600;                //游戏窗口高度(像素)，主窗口的标准高度，包含标题,分数区,游戏区,按钮区
    
    // ========== 布局参数 ==========
    
    constexpr int MARGIN_Y1 = 26;                     //顶部边距，窗口顶部到第一个UI元素的距离
    constexpr int SCORE_W = 121;                      //分数框宽度，当前分数和最高分显示框的宽度
    constexpr int SCORE_H = 70;                       //分数框高度，当前分数和最高分显示框的高度
    constexpr int SCORE_X = 130;                      //分数框起始X坐标，分数框在窗口中的水平位置
    constexpr int SCORE_MARGIN = 10;                  //分数框间距，当前分数框和最高分框之间的水平距离
    constexpr int SCORE_MARGIN_X = 6;                 //分数框内的水平边距
    constexpr int VCENTER_H = 68;                     //垂直居中区域高度，用于显示游戏说明文字的区域高度
    constexpr int BORDER = 15;                        //游戏区域边框宽度，游戏4x4网格周围的边框厚度
    constexpr int BLOCK = 80;                         //游戏方块大小，每个游戏格子的宽度和高度
    constexpr int BOTTOM_TEXT_H = 25;                 //底部文字区域高度，窗口底部显示提示文字的区域高度
    constexpr int CORNER_RADIUS = 5;                  //圆角半径，分数框和游戏方块的圆角半径
    constexpr int BUTTON_H = 20;                      //按钮高度，所有控制按钮的统一高度
    constexpr int BUTTON_S = 5;                       //按钮间距，相邻按钮之间的水平距离
    extern const char* FONT_NAME;                     //字体名称，游戏中使用的标准字体名称
}

//游戏配色方案定义，定义游戏中使用的所有颜色常量，包括背景色,文字色,数字方块色
namespace Colors {
    // ========== 基础色彩 ==========
    
    constexpr COLORREF WHITE = RGB(255, 255, 255);    //纯白色，用于高对比度文字和背景
    constexpr COLORREF BACKGROUND = RGB(250, 248, 239); //背景色，游戏窗口的主背景色，温暖的米白色
    constexpr COLORREF GRAY = RGB(187, 173, 160);     //灰色，用于边框,空格子背景等
    constexpr COLORREF TEXT_COLOR = RGB(119, 110, 101); //文字色，普通文字的颜色，深灰色
    constexpr COLORREF TEXT = RGB(115, 106, 98);
    constexpr COLORREF SCORE_TITLE = RGB(245, 235, 226);
    constexpr COLORREF LESS_EQUAL_8 = RGB(119, 110, 101);
    constexpr COLORREF CR_2 = RGB(238, 228, 218);
    constexpr COLORREF CR_4 = RGB(237, 224, 200);
    constexpr COLORREF CR_8 = RGB(242, 177, 121);
    constexpr COLORREF CR_16 = RGB(245, 149, 99);
    constexpr COLORREF CR_32 = RGB(246, 124, 95);
    constexpr COLORREF CR_64 = RGB(246, 94, 59);
    constexpr COLORREF CR_128 = RGB(237, 204, 97);    //数字128的背景色，金黄色，达到128是一个里程碑
    constexpr COLORREF CR_256 = RGB(237, 204, 97);                //数字256的背景色，与128相同的金黄色
    constexpr COLORREF CR_512 = RGB(237, 204, 97);                //数字512的背景色
    constexpr COLORREF CR_1024 = RGB(237, 204, 97);               //数字1024的背景色
    constexpr COLORREF CR_2048 = RGB(237, 194, 46);   //数字2048的背景色，金色，游戏目标数字的特殊颜色
}

// 前向声明，避免头文件循环包含
class Game2048;

//用户界面绘制函数命名空间，包含所有UI相关的绘制和管理函数
namespace Game2048UI {
    
    // ========== 初始化和整体绘制 ==========
    
    //初始化用户界面，执行UI初始化设置：计算各UI元素的位置和大小,创建自绘按钮控件,设置按钮文本内容,初始化界面布局参数
    void InitializeUI(Game2048* game);
    
    //绘制完整游戏界面，执行完整的界面绘制流程：绘制游戏标题"2048",绘制分数显示区域,绘制游戏说明文字,绘制4x4游戏网格,绘制所有数字方块,应用颜色主题和字体
    void DrawGame(Game2048* game, HDC hdc);
    
    //刷新主游戏区域，重绘游戏棋盘区域，通常在数字变化后调用
    void RefreshMainRect(Game2048* game);
    
    // ========== 绘制基础函数 ==========
    
    //绘制文本到指定区域，高级文本绘制函数，支持：自定义字体大小和粗细,任意颜色设置,多种对齐方式,自动字体创建和清理
    void DrawText(HDC hdc, const std::string& text, const RECT& rect, 
                  long fontSize, int fontWeight, COLORREF color, UINT format);
    
    //填充矩形区域，用指定颜色填充矩形区域，用于绘制背景,按钮,方块等
    void FillRect(HDC hdc, const RECT& rect, COLORREF color);
    
    // ========== 辅助工具函数 ==========
    
    //根据数字获取对应的背景颜色，根据2048游戏规则返回数字的背景色：2浅灰色,4米色,8橙色,16深橙色,更大的数字使用渐进的颜色方案
    COLORREF GetNumberColor(unsigned int number);
    
    // 将数字转换为字符串，简单的数字到字符串转换，用于在界面上显示数字
    std::string NumberToString(unsigned int number);
    
    // 获取程序版本信息，从可执行文件的版本资源中提取版本信息，用于在关于对话框中显示
    void GetFileVersion(const std::string& format, std::string& result);
}
