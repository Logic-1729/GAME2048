//2048游戏的Windows消息处理实现，包含主窗口和子窗口（按钮）的消息处理逻辑，处理绘制,键盘输入,鼠标操作等

#include "Game2048.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>

//主窗口消息处理函数，处理游戏主窗口的各种Windows消息，包括创建,绘制,键盘输入,定时器等
LRESULT Game2048::HandleMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:{
            m_hwnd = hwnd;
            InitializeUI();
            LoadGame();
            return 0;
        }
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            SetBkMode(hdc, TRANSPARENT);
            
            HBRUSH hBrush = CreateSolidBrush(Colors::GRAY);
            HFONT oldBrush = static_cast<HFONT>(SelectObject(hdc, hBrush));
            
            HPEN hPen = CreatePen(PS_NULL, 0, 0);
            HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, hPen));
            
            // 绘制标题
            DrawText(hdc, "2048", m_rectName, 34, 0, Colors::TEXT, 
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            
            // 绘制分数框
            RoundRect(hdc, m_rectScore.left, m_rectScore.top, m_rectScore.right, m_rectScore.bottom,
                     UIConstants::CORNER_RADIUS, UIConstants::CORNER_RADIUS);
            DrawText(hdc, "SCORE", m_rectScoreTitle, 12, 700, Colors::SCORE_TITLE,
                    DT_CENTER | DT_SINGLELINE | DT_BOTTOM);
            DrawText(hdc, NumberToString(m_score), m_rectScoreNum, 24, 0, Colors::WHITE,
                    DT_CENTER | DT_SINGLELINE | DT_TOP);
            
            // 绘制最高分框
            RoundRect(hdc, m_rectBest.left, m_rectBest.top, m_rectBest.right, m_rectBest.bottom,
                     UIConstants::CORNER_RADIUS, UIConstants::CORNER_RADIUS);
            DrawText(hdc, "BEST", m_rectBestTitle, 12, 700, Colors::SCORE_TITLE,
                    DT_CENTER | DT_SINGLELINE | DT_BOTTOM);
            DrawText(hdc, NumberToString(m_highScore), m_rectBestNum, 24, 0, Colors::WHITE,
                    DT_CENTER | DT_SINGLELINE | DT_TOP);
            
            // 绘制说明文字
            DrawText(hdc, "Unite the numbers to reach 2048!", m_rectVCenterText, 13, 0, Colors::TEXT,
                    DT_LEFT | DT_SINGLELINE | DT_VCENTER);
            //DrawText(hdc, "Use arrow keys to move and merge tiles!", m_rectBottomText, 13, 0, Colors::TEXT,
             //       DT_LEFT | DT_SINGLELINE | DT_VCENTER);
            
            // 绘制主游戏区域
            RoundRect(hdc, m_rectMain.left, m_rectMain.top, m_rectMain.right, m_rectMain.bottom,
                     UIConstants::CORNER_RADIUS, UIConstants::CORNER_RADIUS);
            
            // 绘制游戏方块
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
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_KEYDOWN: {
            bool moved = false;
            
            switch (wParam) {
                case VK_UP:
                case 'W':
                case 'w':
                    moved = ProcessMove(0);
                    break;
                case VK_DOWN:
                case 'S':
                case 's':
                    moved = ProcessMove(1);
                    break;
                case VK_LEFT:
                case 'A':
                case 'a':
                    moved = ProcessMove(2);
                    break;
                case VK_RIGHT:
                case 'D':
                case 'd':
                    moved = ProcessMove(3);
                    break;
                case VK_RETURN:
                case VK_SPACE: {
                    int randomDirection = rand() % 4;  // 生成0-3的随机方向
                    moved = ProcessMove(randomDirection);
                    break;
                }
                case 'Z':
                case 'z':
                    // UNDO功能
                    if (m_redoCount > 0) {
                        Redo();
                        RefreshRedoButton();
                        InvalidateRect(hwnd, nullptr, TRUE);
                    }
                    return 0;
                case 'Q':
                case 'q':
                    // 退出功能
                    MessageBeep(0);
                    if (IDOK == MessageBoxA(hwnd, "EXIT ENSURE?\n IT WILL BE SAVED", "EXIT", 
                                          MB_OKCANCEL | MB_ICONQUESTION)) {
                        SaveGame();
                        PostQuitMessage(0);
                    }
                    return 0;
                case 'F':
                case 'f':
                    // HINT功能（智能随机移动）
                    HintWithUI();
                    InvalidateRect(hwnd, nullptr, TRUE);
                    return 0;
                case 'N':
                case 'n':
                    // NEW GAME功能
                    if (InHighScore()) {
                        JudgeRefreshHighScore();
                    }
                    NewGame();
                    SaveRecord();
                    RefreshRedoButton();
                    SaveGame();
                    InvalidateRect(hwnd, nullptr, TRUE);
                    return 0;
                case 'H':
                case 'h':
                    // HISTORY功能
                    {
                        MessageBeep(0);
                        std::string scoreboard = "--- TOP 10 SCORES ---\n\n RANK\t  SCORE";
                        
                        bool hasAnyScores = false;
                        for (int i = 0; i < DataConstants::HIGHSCORE_COUNT; ++i) {
                            if (m_highScores[i].score != 0) {
                                hasAnyScores = true;
                                std::stringstream ss;
                                ss << "\n  " << std::setw(2) << (i + 1) << "\t  " 
                                   << std::setw(6) << m_highScores[i].score << "pts";
                                scoreboard += ss.str();
                            }
                        }
                        
                        if (!hasAnyScores) {
                            scoreboard += "\n\n  No scores recorded yet!\n  Play some games to see rankings here.";
                        }
                        
                        MessageBoxA(hwnd, scoreboard.c_str(), "HISTORY RANKINGS", 0);
                    }
                    return 0;
            }
            
            if (moved) {
                AddNewNumber();
                RefreshMainRect();
                SaveRecord();
                
                m_highScore = std::max(m_score, m_highScore);
                
                RefreshRedoButton(); // 更新UNDO按钮状态
                InvalidateRect(hwnd, &m_rectScore, TRUE);
                InvalidateRect(hwnd, &m_rectBest, TRUE);
                
                if (IsGameOver()) {
                    HandleGameEndAndUpdateScores();
                }
            } else if (IsGameOver()) {
                // 死局时显示游戏结束信息并更新分数
                HandleGameEndAndUpdateScores();
            }
            return 0;
        }
        
        case WM_MOUSEMOVE:
            m_onMouse = -1;
            for (int i = 0; i < MessageConstants::BNUM; ++i) {
                InvalidateRect(m_hwndButtons[i], nullptr, FALSE);
            }
            return 0;
            
        case WM_CLOSE: {
            MessageBeep(0);
            if (IDOK == MessageBoxA(hwnd, "EXIT ENSURE?\n IT WILL BE SAVED", "EXIT", 
                                  MB_OKCANCEL | MB_ICONQUESTION)) {
                SaveGame();
                break; // 继续到DefWindowProc
            } else {
                return 0; // 忽略关闭消息
            }
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_QUIT:
            return 0;
    }
    
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT Game2048::HandleChildMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT: {
            HDC hdc;
            PAINTSTRUCT ps;
            RECT rect;
            HDC hDCMem;
            HBITMAP hBitmap;
            
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            
            hDCMem = CreateCompatibleDC(hdc);
            hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
            SelectObject(hDCMem, hBitmap);
            
            int buttonId = GetDlgCtrlID(hwnd);
            COLORREF bgColor = (m_onMouse == buttonId) ? Colors::CR_32 : Colors::GRAY;
            FillRect(hDCMem, rect, bgColor);
            
            SetBkMode(hDCMem, TRANSPARENT);
            DrawText(hDCMem, m_buttonNames[buttonId].name, rect, 10, 700, Colors::WHITE,
                    DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            
            BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDCMem, 0, 0, SRCCOPY);
            DeleteObject(hBitmap);
            DeleteDC(hDCMem);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_LBUTTONDOWN: {
            int buttonId = GetDlgCtrlID(hwnd);
            
            switch (buttonId) {
                case MessageConstants::NEWGAME:
                    // NEW按钮：直接开始新游戏，不显示确认对话框
                    if (InHighScore()) {
                        JudgeRefreshHighScore();
                    }
                    NewGame();
                    SaveRecord();
                    RefreshRedoButton();
                    SaveGame();
                    InvalidateRect(GetParent(hwnd), nullptr, TRUE);
                    break;
                    
                case MessageConstants::HINT:
                    // HINT功能：智能随机移动直到找到有效操作
                    HintWithUI();
                    InvalidateRect(GetParent(hwnd), nullptr, TRUE);
                    break;
                    
                case MessageConstants::REDO:
                    if (m_redoCount > 0) {
                        Redo();
                        RefreshRedoButton(); // 更新UNDO按钮状态
                        InvalidateRect(GetParent(hwnd), nullptr, TRUE);
                    }
                    break;
                    
                case MessageConstants::HIGHSCORE: {
                    MessageBeep(0);
                    std::string scoreboard = "--- TOP 10 SCORES ---\n\n RANK\t  SCORE";
                    
                    // 检查是否有任何分数记录
                    bool hasAnyScores = false;
                    for (int i = 0; i < DataConstants::HIGHSCORE_COUNT; ++i) {
                        if (m_highScores[i].score != 0) {
                            hasAnyScores = true;
                            std::stringstream ss;
                            ss << "\n  " << std::setw(2) << (i + 1) << "\t  " 
                               << std::setw(6) << m_highScores[i].score << "pts";
                            scoreboard += ss.str();
                        }
                    }
                    
                    // 如果没有任何分数记录，显示提示
                    if (!hasAnyScores) {
                        scoreboard += "\n\n  No scores recorded yet!\n  Play some games to see rankings here.";
                    }
                    
                    MessageBoxA(GetParent(hwnd), scoreboard.c_str(), "HISTORY RANKINGS", 0);
                    break;
                }
            }
            return 0;
        }
        
        case WM_MOUSEMOVE:
            m_onMouse = GetDlgCtrlID(hwnd);
            for (int i = 0; i < MessageConstants::BNUM; ++i) {
                InvalidateRect(m_hwndButtons[i], nullptr, FALSE);
            }
            return 0;
    }
    
    return DefWindowProc(hwnd, message, wParam, lParam);
}

bool Game2048::JudgeRefreshHighScore() {
    // 直接更新高分，不需要玩家姓名
    UpdateHighScore("Player");
    SaveGame(); // 保存更新的高分榜到文件
    return true;
}

void Game2048::HandleGameEndAndUpdateScores() {
    if (!IsGameOver()) return;
    // 1. 先显示MessageBox（显示游戏结束信息）
    std::string buffer, title;
    if (HasReached2048()) {
        buffer = "CONGRATULATIONS!\nYou reached 2048!\n\nFinal Score: " + std::to_string(m_score);
        title = "YOU WIN!";
    } else {
        buffer = "YOU LOSE!\nTRY AGAIN!\n\nFinal Score: " + std::to_string(m_score);
        title = "GAME OVER!";
    }
    
    MessageBeep(0);
    // 强制显示MessageBox，确保它总是出现
    MessageBoxA(GetActiveWindow(), buffer.c_str(), title.c_str(), MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL | MB_TOPMOST);
    
    // 2. 然后更新高分榜（如果当前分数符合条件）
    if (InHighScore()) JudgeRefreshHighScore();
    
    // 3. 界面保持不动，游戏状态保持结束状态
    // 用户可以点击NEW开始新游戏，或UNDO撤回，或查看HISTORY
}

bool Game2048::AskStartNewGame() {
    std::string buffer, title;
    
    if (IsGameOver()) {
        if (HasReached2048()) {
            buffer = "CONGRATULATIONS!\nYou reached 2048!\n\nNEW GAME AGAIN?";
            title = "YOU WIN!";
        } else {
            buffer = "YOU LOSE!\nTRY AGAIN!\n\nNEW GAME?";
            title = "GAME OVER!";
        }
    } else {
        buffer = "NEW GAME AGAIN?";
        title = "NEW GAME";
    }
    
    MessageBeep(0);
    // 使用NULL作为父窗口，确保MessageBox总是显示
    int result = MessageBoxA(NULL, buffer.c_str(), title.c_str(), MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
    return (IDYES == result);
}

// UI层的Hint包装函数，处理所有Windows API相关的操作
void Game2048::HintWithUI() {
    int hintResult = Hint(); // 调用纯逻辑版本的Hint
    
    if (hintResult == 0) {
        MessageBeep(MB_ICONWARNING); // 游戏已结束
    } else if (hintResult == 1) {
        RefreshRedoButton(); // 成功移动，更新按钮状态
        
        // 检查移动后游戏是否结束
        if (IsGameOver()) {
            HandleGameEndAndUpdateScores();
        }
    } else {
        MessageBeep(MB_ICONWARNING); // 无有效移动
    }
}