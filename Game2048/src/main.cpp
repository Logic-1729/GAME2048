//2048游戏程序主入口，Windows应用程序启动点，负责游戏初始化,窗口创建和消息循环

#include "Game2048.h"

//Windows应用程序主入口点，程序启动流程：创建全局游戏对象实例,初始化游戏系统和UI,获取系统显示参数,创建并居中显示主窗口,进入Windows消息循环,程序退出时自动清理资源
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
    
    // ========== 游戏对象创建 ==========
    
    // 创建全局游戏对象实例，使用智能指针管理游戏对象，确保自动释放资源
    g_pGame = std::make_unique<Game2048>();
    
    //初始化游戏系统，执行游戏初始化：注册窗口类,设置游戏参数,加载保存的游戏数据,初始化随机数生成器
    if (!g_pGame->Initialize(hInstance)) {
        return 0;
    }
    
    // ========== 系统参数获取 ==========
    
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);      //获取屏幕宽度，用于计算窗口居中位置
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);      //获取屏幕高度，用于计算窗口居中位置
    int cyCaption = GetSystemMetrics(SM_CYCAPTION);    //获取标题栏高度，用于计算窗口总高度
    int cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME); //获取窗口边框宽度，用于计算窗口总宽度
    int cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME); //获取窗口边框高度，用于计算窗口总高度
    
    // ========== 窗口创建 ==========
    
    char szAppTitle[MAX_PATH] = "2048";                //应用程序标题，显示在窗口标题栏的文字
    
    // 创建主应用程序窗口，窗口参数：类名"2048",标题栏,系统菜单,最小化按钮,屏幕居中位置
    HWND hwnd = CreateWindowA(
        "2048",                                              // 窗口类名
        szAppTitle,                                          // 窗口标题
        WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,  // 窗口样式
        (cxScreen - UIConstants::WINDOW_WIDTH) / 2,          // X坐标(居中)
        (cyScreen - UIConstants::WINDOW_HEIGHT) / 2,         // Y坐标(居中)
        UIConstants::WINDOW_WIDTH + 2 * cxSizeFrame,        // 窗口宽度(客户区+边框)
        UIConstants::WINDOW_HEIGHT + cyCaption + 2 * cySizeFrame,  // 窗口高度(客户区+标题栏+边框)
        nullptr,                                             // 父窗口句柄
        nullptr,                                             // 菜单句柄
        hInstance,                                          // 应用程序实例句柄
        nullptr                                             // 创建参数
    );
    
    //检查窗口创建是否成功，如果创建失败，显示错误消息并退出程序
    if (!hwnd) {
        MessageBoxA(nullptr, "Failed to create window.", "Error", MB_ICONERROR);
        return 0;
    }
    
    // ========== 窗口显示 ==========
    
    ShowWindow(hwnd, iCmdShow);                      //显示窗口，按照传入的显示方式参数显示窗口
    UpdateWindow(hwnd);                              //更新窗口，立即重绘窗口，确保界面正确显示
    
    // ========== 消息循环 ==========
    
    MSG msg;                                         //Windows消息结构体，用于接收和处理Windows系统消息
    
    //主消息循环，持续处理Windows消息直到收到WM_QUIT：GetMessage从消息队列获取消息,TranslateMessage翻译键盘消息,DispatchMessage分发消息到窗口过程函数
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);                      //翻译虚拟键消息为字符消息
        DispatchMessage(&msg);                       //将消息发送到对应的窗口过程函数
    }
    
    return static_cast<int>(msg.wParam);             //返回程序退出代码，通常是PostQuitMessage函数传递的退出代码
}
