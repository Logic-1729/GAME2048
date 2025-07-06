//2048游戏的数据管理实现，包含高分榜管理,游戏状态保存/加载,分数判断等数据相关功能

#include "Game2048.h"
#include <algorithm>

//判断当前分数是否可以进入高分榜，检查高分榜是否已满，以及当前分数是否达到进入高分榜的条件
bool Game2048::InHighScore() const {
    // 记录历史分数排名，但设置一个最低阈值避免记录太低的分数
    // 如果高分榜还没满，或者当前分数大于最低记录，就可以进入
    if (m_highScores[DataConstants::HIGHSCORE_COUNT - 1].score == 0) {
        // 高分榜还没满，只要分数达到阈值就记录
        return m_score >= DataConstants::HIGHSCORE_THRESHOLD;
    } else {
        // 高分榜已满，当前分数要大于最低记录才能进入
        return m_score > m_highScores[DataConstants::HIGHSCORE_COUNT - 1].score;
    }
}

void Game2048::UpdateHighScore(const std::string& name) {
    // 检查是否已经存在相同分数的记录（避免重复记录）
    for (int i = 0; i < DataConstants::HIGHSCORE_COUNT; ++i) {
        if (m_highScores[i].score == m_score) {
            // 已经存在相同分数的记录，不重复添加
            return;
        }
    }
    
    // 找到合适的插入位置
    int insertPosition = DataConstants::HIGHSCORE_COUNT; // 默认插入到末尾
    for (int i = 0; i < DataConstants::HIGHSCORE_COUNT; ++i) {
        if (m_score > m_highScores[i].score || m_highScores[i].score == 0) {
            insertPosition = i;
            break;
        }
    }
    
    if (insertPosition < DataConstants::HIGHSCORE_COUNT) {
        // 向后移动所有较低的分数（为新分数腾出空间）
        for (int j = DataConstants::HIGHSCORE_COUNT - 1; j > insertPosition; --j) {
            m_highScores[j] = m_highScores[j - 1];
        }
        
        // 插入新的分数记录
        m_highScores[insertPosition].score = m_score;
        m_highScores[insertPosition].name = name;
        
        // 更新显示的最高分（取高分榜第一名的分数）
        if (m_highScores[0].score > 0) {
            m_highScore = m_highScores[0].score;
        }
    }
}

void Game2048::SaveGame() {
    std::ofstream file(m_filePath, std::ios::binary);
    if (file.is_open()) {
        // 保存高分榜
        for (const auto& score : m_highScores) {
            file.write(reinterpret_cast<const char*>(&score.score), sizeof(score.score));
            size_t nameLength = score.name.length();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
            if (nameLength > 0) {
                file.write(reinterpret_cast<const char*>(score.name.c_str()), nameLength * sizeof(char));
            }
        }
        
        // 保存游戏状态
        file.write(reinterpret_cast<const char*>(m_gameBoard.data()), 
                  sizeof(m_gameBoard));
        file.write(reinterpret_cast<const char*>(&m_step), sizeof(m_step));
        file.write(reinterpret_cast<const char*>(&m_score), sizeof(m_score));
        file.write(reinterpret_cast<const char*>(&m_highScore), sizeof(m_highScore));
        
        // 保存撤销历史
        for (const auto& state : m_redoHistory) {
            file.write(reinterpret_cast<const char*>(&state.score), sizeof(state.score));
            file.write(reinterpret_cast<const char*>(state.num.data()), sizeof(state.num));
        }
        
        file.write(reinterpret_cast<const char*>(&m_redoCount), sizeof(m_redoCount));
        
        file.close();
    }
}

void Game2048::LoadGame() {
    std::ifstream file(m_filePath, std::ios::binary);
    if (file.is_open()) {
        try {
            // 加载高分榜
            for (auto& score : m_highScores) {
                file.read(reinterpret_cast<char*>(&score.score), sizeof(score.score));
                size_t nameLength;
                file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                if (nameLength > 0 && nameLength < 1000) { // 防止异常数据
                    score.name.resize(nameLength);
                    file.read(reinterpret_cast<char*>(&score.name[0]), nameLength * sizeof(char));
                }
            }
            
            // 加载游戏状态
            file.read(reinterpret_cast<char*>(m_gameBoard.data()), sizeof(m_gameBoard));
            file.read(reinterpret_cast<char*>(&m_step), sizeof(m_step));
            file.read(reinterpret_cast<char*>(&m_score), sizeof(m_score));
            file.read(reinterpret_cast<char*>(&m_highScore), sizeof(m_highScore));
            
            // 加载撤销历史
            for (auto& state : m_redoHistory) {
                file.read(reinterpret_cast<char*>(&state.score), sizeof(state.score));
                file.read(reinterpret_cast<char*>(state.num.data()), sizeof(state.num));
            }
            
            file.read(reinterpret_cast<char*>(&m_redoCount), sizeof(m_redoCount));
            
            file.close();
            RefreshRedoButton();
        } catch (...) {
            file.close();
        }
    }
    
    // 如果没有保存的游戏或加载失败，开始新游戏
    if (m_step == 0) {
        NewGame();
        SaveRecord();
        RefreshRedoButton();
        SaveGame();
    }
}
