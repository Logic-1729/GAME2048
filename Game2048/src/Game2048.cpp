//2048游戏主类实现，实现Game2048类的核心功能：初始化、清理、窗口过程等

#include "Game2048.h"
#include <cstdlib>  // for srand()
#include <ctime>    // for time()

//全局游戏对象指针，全局智能指针，用于在Windows回调函数中访问游戏实例，因为Windows回调函数是C风格的，无法直接传递C++对象
std::unique_ptr<Game2048> g_pGame;

//Game2048类构造函数，初始化所有成员变量为默认值：窗口句柄设为nullptr、游戏数据重置为初始状态、随机数生成器用真随机种子初始化、为容器分配内存空间
Game2048::Game2048() 
    : Game2048Logic()                    //调用基类构造函数
    , m_hwnd(nullptr)                    //主窗口句柄
    , m_hInst(nullptr)                   //应用程序实例句柄
    , m_onMouse(-1)                      // 鼠标悬停状态
{
    // 初始化rand()随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // m_gameBoard的初始化在基类构造函数中完成
    
    //初始化高分榜，分配指定数量的高分记录空间
    m_highScores.resize(DataConstants::HIGHSCORE_COUNT);
    
    // m_redoHistory现在是基类的std::array，不需要resize
    
    //初始化按钮控件句柄数组，将所有按钮句柄设为nullptr，后续在InitializeUI中创建
    for (int i = 0; i < MessageConstants::BNUM; ++i) {
        m_hwndButtons[i] = nullptr;
    }
}

//Game2048类析构函数，自动清理资源，保存游戏状态
Game2048::~Game2048() {
    Cleanup();
}

//初始化游戏系统，执行完整的游戏初始化流程：保存实例句柄、设置游戏数据文件路径、获取系统显示参数、注册主窗口类、注册子窗口(按钮)类
bool Game2048::Initialize(HINSTANCE hInstance) {
    m_hInst = hInstance;                 //保存应用程序实例句柄，后续创建窗口和资源时需要使用
    
    // ========== 设置数据文件路径 ==========
    
    //获取程序执行文件路径
    char modulePath[MAX_PATH];
    if (GetModuleFileNameA(nullptr, modulePath, MAX_PATH) > 0) {
        m_filePath = modulePath;
        
        //提取目录路径,去掉文件名，保留目录路径
        size_t pos = m_filePath.find_last_of('\\');
        if (pos != std::string::npos) {
            m_filePath = m_filePath.substr(0, pos + 1);
        }
        
        //设置存档文件名,使用固定的文件名"2048.SAV"
        m_filePath += "2048.SAV";
    }
    
    //获取系统显示相关参数
    m_cxScreen = GetSystemMetrics(SM_CXSCREEN);       // 屏幕宽度
    m_cyScreen = GetSystemMetrics(SM_CYSCREEN);       // 屏幕高度
    m_cyCaption = GetSystemMetrics(SM_CYCAPTION);     // 标题栏高度
    m_cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME); // 边框宽度
    m_cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME); // 边框高度
    
    
    //主窗口类结构体,定义主游戏窗口的属性和行为
    WNDCLASSA wndclass = {};
    wndclass.style = CS_HREDRAW | CS_VREDRAW;           // 窗口样式：水平和垂直重绘
    wndclass.lpfnWndProc = WndProc;                     // 窗口过程函数
    wndclass.cbClsExtra = 0;                            // 类额外字节数
    wndclass.cbWndExtra = 0;                            // 窗口额外字节数
    wndclass.hInstance = hInstance;                     // 应用程序实例
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // 窗口图标
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);  // 鼠标光标
    wndclass.hbrBackground = CreateSolidBrush(Colors::BACKGROUND); // 背景画刷
    wndclass.lpszMenuName = nullptr;                    // 菜单名称
    wndclass.lpszClassName = "2048";                    // 窗口类名
    
    //注册主窗口类,向系统注册窗口类，失败时显示错误消息
    if (!RegisterClassA(&wndclass)) {
        MessageBoxA(nullptr, "Failed to register window class.", "2048", MB_ICONERROR);
        return false;
    }
    
    //修改窗口类属性用于子窗口
    wndclass.lpfnWndProc = ChildWndProc;               // 子窗口过程函数
    wndclass.cbWndExtra = sizeof(long);                // 存储按钮ID的额外空间
    wndclass.hIcon = nullptr;                          // 子窗口不需要图标
    wndclass.lpszClassName = "szChildClass";           // 子窗口类名
    
    //注册子窗口类,用于创建自绘按钮控件
    if (!RegisterClassA(&wndclass)) {
        MessageBoxA(nullptr, "Failed to register child window class.", "2048", MB_ICONERROR);
        return false;
    }
    
    return true;
}

//清理游戏资源
void Game2048::Cleanup() {
    if (m_hwnd) {
        SaveGame();
    }
}

//主窗口全局窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (g_pGame) {
        return g_pGame->HandleMessage(hwnd, message, wParam, lParam);
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

//子窗口(按钮)全局窗口过程函数
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (g_pGame) {
        return g_pGame->HandleChildMessage(hwnd, message, wParam, lParam);
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// ========== 转发函数实现（保持接口兼容性） ==========

// 核心游戏逻辑转发函数
bool Game2048::ProcessMove(int direction) {
    return Game2048Logic::ProcessMove(direction);
}

void Game2048::AddNewNumber() {
    Game2048Logic::AddNewNumber();
}

void Game2048::NewGame() {
    Game2048Logic::NewGame();
}

int Game2048::Hint() {
    return Game2048Logic::Hint();
}

int Game2048::ChooseDirection() const {
    return Game2048Logic::ChooseDirection();
}

// 游戏状态检查转发函数
bool Game2048::IsGameOver() const {
    return Game2048Logic::IsGameOver();
}

bool Game2048::HasReached2048() const {
    return Game2048Logic::HasReached2048();
}

// 撤销系统转发函数
void Game2048::Redo() {
    Game2048Logic::Redo();
}

void Game2048::SaveRecord() {
    Game2048Logic::SaveRecord();
}

// 工具函数转发
bool Game2048::IsAllZero(const std::array<std::array<unsigned int, 4>, 4>& board) const {
    return Game2048Logic::IsAllZero(board);
}

void Game2048::CopyBoard(std::array<std::array<unsigned int, 4>, 4>& dest, 
                        const std::array<std::array<unsigned int, 4>, 4>& src) const {
    Game2048Logic::CopyBoard(dest, src);
}
