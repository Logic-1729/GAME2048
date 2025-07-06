//2048游戏的用户界面实现，包含界面绘制,文字渲染,颜色管理,UI布局初始化等界面相关功能

#include "Game2048.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "version.lib")
#endif

// 字体名称
const char* UIConstants::FONT_NAME = "Arial";

//绘制文本到指定区域

void Game2048::DrawText(HDC hdc, const std::string& text, const RECT& rect, 
                       long fontSize, int fontWeight, COLORREF color, UINT format) const {
    long lfHeight = -MulDiv(fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hf = CreateFontA(lfHeight, 0, 0, 0, fontWeight, 0, 0, 0, 0, 0, 0, 0, 0, UIConstants::FONT_NAME);
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, hf));
    
    SetTextColor(hdc, color);
    RECT tempRect = rect;
    ::DrawTextA(hdc, text.c_str(), -1, &tempRect, format);
    
    SelectObject(hdc, oldFont);
    DeleteObject(hf);
}

void Game2048::FillRect(HDC hdc, const RECT& rect, COLORREF color) const {
    HBRUSH hBrush = CreateSolidBrush(color);
    ::FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}

COLORREF Game2048::GetNumberColor(unsigned int number) const {
    switch (number) {
        case 2: return Colors::CR_2;
        case 4: return Colors::CR_4;
        case 8: return Colors::CR_8;
        case 16: return Colors::CR_16;
        case 32: return Colors::CR_32;
        case 64: return Colors::CR_64;
        case 128: return Colors::CR_128;
        case 256: return Colors::CR_256;
        case 512: return Colors::CR_512;
        case 1024: return Colors::CR_1024;
        case 2048: return Colors::CR_2048;
        default: return Colors::CR_2048;
    }
}

std::string Game2048::NumberToString(unsigned int number) const {
    return std::to_string(number);
}

void Game2048::RefreshRedoButton() {
    std::string redoText = "REDO";
    m_buttonNames[MessageConstants::REDO].name = redoText;
    
    if (m_hwndButtons[MessageConstants::REDO]) {
        EnableWindow(m_hwndButtons[MessageConstants::REDO], m_redoCount > 0);
        InvalidateRect(m_hwndButtons[MessageConstants::REDO], nullptr, TRUE);
    }
}

void Game2048::RefreshMainRect() {
    if (!m_hwnd) return;
    
    HDC hdc = GetDC(m_hwnd);
    SetBkMode(hdc, TRANSPARENT);
    
    HBRUSH hBrush = CreateSolidBrush(Colors::GRAY);
    HFONT oldBrush = static_cast<HFONT>(SelectObject(hdc, hBrush));
    
    HPEN hPen = CreatePen(PS_NULL, 0, 0);
    HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, hPen));
    
    // 画主界面背景
    RoundRect(hdc, m_rectMain.left, m_rectMain.top, m_rectMain.right, m_rectMain.bottom, 
              UIConstants::CORNER_RADIUS, UIConstants::CORNER_RADIUS);
    
    // 画游戏方块
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_gameBoard[i][j] != 0) {
                FillRect(hdc, m_rectCells[i][j], GetNumberColor(m_gameBoard[i][j]));
                
                COLORREF textColor = (m_gameBoard[i][j] <= 8) ? Colors::LESS_EQUAL_8 : Colors::WHITE;
                long fontSize = (m_gameBoard[i][j] < 1024) ? 26 : 18;
                
                DrawText(hdc, NumberToString(m_gameBoard[i][j]), m_rectCells[i][j], 
                        fontSize, 700, textColor, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
        }
    }
    
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
    ReleaseDC(m_hwnd, hdc);
}

void Game2048::InitializeUI() {
    // 初始化通用控件
    InitCommonControls();
    
    // 窗口大小调整
    RECT clientRect, windowRect;
    GetClientRect(m_hwnd, &clientRect);
    GetWindowRect(m_hwnd, &windowRect);
    
    int dx = clientRect.right - UIConstants::WINDOW_WIDTH;
    int dy = clientRect.bottom - UIConstants::WINDOW_HEIGHT;
    
    SetWindowPos(m_hwnd, nullptr, 
                (m_cxScreen - UIConstants::WINDOW_WIDTH) / 2,
                (m_cyScreen - UIConstants::WINDOW_HEIGHT) / 2,
                (windowRect.right - windowRect.left) - dx,
                (windowRect.bottom - windowRect.top) - dy, 0);
    
    // 设置各个区域的矩形
    m_rectName.left = UIConstants::BORDER;
    m_rectName.top = UIConstants::MARGIN_Y1;
    m_rectName.right = UIConstants::SCORE_X - UIConstants::SCORE_MARGIN_X;
    m_rectName.bottom = m_rectName.top + UIConstants::SCORE_H;
    
    m_rectScore.left = UIConstants::SCORE_X;
    m_rectScore.top = UIConstants::MARGIN_Y1;
    m_rectScore.right = m_rectScore.left + UIConstants::SCORE_W;
    m_rectScore.bottom = m_rectScore.top + UIConstants::SCORE_H;
    
    SetRect(&m_rectScoreTitle, m_rectScore.left, m_rectScore.top, m_rectScore.right,
            static_cast<int>(m_rectScore.top + 0.4 * (m_rectScore.bottom - m_rectScore.top)));
    SetRect(&m_rectScoreNum, m_rectScore.left, 
            static_cast<int>(m_rectScore.top + 0.4 * (m_rectScore.bottom - m_rectScore.top)),
            m_rectScore.right, m_rectScore.bottom);
    
    m_rectBest.right = UIConstants::WINDOW_WIDTH - UIConstants::BORDER;
    m_rectBest.left = m_rectBest.right - UIConstants::SCORE_W;
    m_rectBest.top = UIConstants::MARGIN_Y1;
    m_rectBest.bottom = m_rectBest.top + UIConstants::SCORE_H;
    
    SetRect(&m_rectBestTitle, m_rectBest.left, m_rectBest.top, m_rectBest.right,
            static_cast<int>(m_rectBest.top + 0.4 * (m_rectBest.bottom - m_rectBest.top)));
    SetRect(&m_rectBestNum, m_rectBest.left,
            static_cast<int>(m_rectBest.top + 0.4 * (m_rectBest.bottom - m_rectBest.top)),
            m_rectBest.right, m_rectBest.bottom);
    
    m_rectVCenterText.left = UIConstants::BORDER;
    m_rectVCenterText.right = UIConstants::WINDOW_WIDTH - UIConstants::BORDER;
    m_rectVCenterText.top = m_rectScore.bottom;
    m_rectVCenterText.bottom = m_rectVCenterText.top + UIConstants::VCENTER_H;
    
    m_rectMain.left = UIConstants::BORDER;
    m_rectMain.right = UIConstants::WINDOW_WIDTH - UIConstants::BORDER;
    m_rectMain.top = m_rectVCenterText.bottom;
    m_rectMain.bottom = m_rectMain.top + (m_rectMain.right - m_rectMain.left);
    
    m_iBlock = ((m_rectMain.right - m_rectMain.left) - UIConstants::BLOCK * 4) / 5;
    
    // 设置游戏方块的矩形
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m_rectCells[i][j].left = m_rectMain.left + (j + 1) * m_iBlock + j * UIConstants::BLOCK;
            m_rectCells[i][j].right = m_rectCells[i][j].left + UIConstants::BLOCK;
            m_rectCells[i][j].top = m_rectMain.top + (i + 1) * m_iBlock + i * UIConstants::BLOCK;
            m_rectCells[i][j].bottom = m_rectCells[i][j].top + UIConstants::BLOCK;
        }
    }
    
    m_rectBottomText.left = m_rectMain.left;
    m_rectBottomText.right = m_rectMain.right;
    m_rectBottomText.top = m_rectMain.bottom;
    m_rectBottomText.bottom = m_rectBottomText.top + UIConstants::BOTTOM_TEXT_H;

    // 创建按钮
    m_buttonW = (UIConstants::WINDOW_WIDTH - 2 * UIConstants::BORDER - 
                (MessageConstants::BNUM - 1) * UIConstants::BUTTON_S) / MessageConstants::BNUM;
    
    // 初始化按钮名称
    m_buttonNames[MessageConstants::NEWGAME].name = "NEW";
    m_buttonNames[MessageConstants::HINT].name = "HINT";
    m_buttonNames[MessageConstants::REDO].name = "REDO";
    m_buttonNames[MessageConstants::HIGHSCORE].name = "HISTORY";
    
    for (int i = 0; i < MessageConstants::BNUM; ++i) {
        m_rectButtons[i].top = UIConstants::WINDOW_HEIGHT - UIConstants::BORDER - UIConstants::BUTTON_H;
        m_rectButtons[i].bottom = UIConstants::WINDOW_HEIGHT - UIConstants::BORDER;
        m_rectButtons[i].left = UIConstants::BORDER + i * UIConstants::BUTTON_S + i * m_buttonW;
        m_rectButtons[i].right = m_rectButtons[i].left + m_buttonW;
        
        m_hwndButtons[i] = CreateWindowA(
            "szChildClass",
            m_buttonNames[i].name.c_str(),
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            m_rectButtons[i].left,
            m_rectButtons[i].top,
            m_buttonW,
            UIConstants::BUTTON_H,
            m_hwnd,
            reinterpret_cast<HMENU>(static_cast<UINT_PTR>(i)),
            m_hInst,
            nullptr);
    }
}

void Game2048::GetFileVersion(const std::string& format, std::string& result) const {
    // 获取文件版本信息
    char modulePath[MAX_PATH];
    if (GetModuleFileNameA(nullptr, modulePath, MAX_PATH) > 0) {
        DWORD verHandle = 0;
        DWORD verSize = GetFileVersionInfoSizeA(modulePath, &verHandle);
        
        if (verSize != 0) {
            std::vector<BYTE> verData(verSize);
            if (GetFileVersionInfoA(modulePath, verHandle, verSize, verData.data())) {
                VS_FIXEDFILEINFO* fileInfo = nullptr;
                UINT size = 0;
                
                if (VerQueryValueA(verData.data(), "\\", 
                                 reinterpret_cast<LPVOID*>(&fileInfo), &size)) {
                    if (size) {
                        int major = (fileInfo->dwFileVersionMS >> 16) & 0xffff;
                        int minor = (fileInfo->dwFileVersionMS) & 0xffff;
                        int patch = (fileInfo->dwFileVersionLS >> 16) & 0xffff;
                        int build = (fileInfo->dwFileVersionLS) & 0xffff;
                        
                        char buffer[256];
                        sprintf_s(buffer, format.c_str(), major, minor, patch, build);
                        result = buffer;
                        return;
                    }
                }
            }
        }
    }
    
    result = "1.0.0.0";
}

void Game2048::DrawGame(HDC hdc) {
    // 绘制整个游戏界面
    SetBkMode(hdc, TRANSPARENT);
    
    // 绘制游戏标题
    DrawText(hdc, "2048", m_rectName, 36, 700, Colors::TEXT, 
             DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    
    // 绘制分数区域
    FillRect(hdc, m_rectScore, Colors::SCORE_TITLE);
    DrawText(hdc, "SCORE", m_rectScoreTitle, 14, 400, Colors::WHITE, 
             DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    DrawText(hdc, std::to_string(m_score), m_rectScoreNum, 20, 700, Colors::WHITE, 
             DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    
    // 绘制最高分区域
    FillRect(hdc, m_rectBest, Colors::SCORE_TITLE);
    DrawText(hdc, "BEST", m_rectBestTitle, 14, 400, Colors::WHITE, 
             DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    DrawText(hdc, std::to_string(m_highScore), m_rectBestNum, 20, 700, Colors::WHITE, 
             DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    
    // 绘制说明文字
    DrawText(hdc, "Merge numbers to reach 2048!", m_rectVCenterText, 18, 400, Colors::TEXT, 
             DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    
    // 绘制游戏板背景
    FillRect(hdc, m_rectMain, Colors::GRAY);
    
    // 绘制游戏格子
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_gameBoard[i][j] == 0) {
                // 空格子
                FillRect(hdc, m_rectCells[i][j], Colors::BACKGROUND);
            } else {
                // 有数字的格子
                FillRect(hdc, m_rectCells[i][j], GetNumberColor(m_gameBoard[i][j]));
                
                COLORREF textColor = (m_gameBoard[i][j] <= 8) ? Colors::LESS_EQUAL_8 : Colors::WHITE;
                long fontSize = (m_gameBoard[i][j] < 1024) ? 26 : 18;
                
                DrawText(hdc, NumberToString(m_gameBoard[i][j]), m_rectCells[i][j], 
                        fontSize, 700, textColor, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
        }
    }
    
    // 绘制底部说明文字
    DrawText(hdc, "Use arrow keys to move tiles", m_rectBottomText, 12, 400, Colors::TEXT, 
             DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}
