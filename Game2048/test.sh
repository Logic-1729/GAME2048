#!/bin/bash

echo "=== Game2048 Hint() TEST ==="
mkdir -p bin

g++ -std=c++11 -O2 -o bin/benchmark_test -x c++ - src/Game2048_Logic.cpp -I. -Iinclude << 'EOF'
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
#include "include/Game2048_Logic.h"

class TestGame2048 : public Game2048Logic {
public:
    TestGame2048() : Game2048Logic() {
        static bool seeded = false;
        if (!seeded) {
            srand(static_cast<unsigned int>(time(nullptr)));
            seeded = true;
        }
    }
    
    std::pair<unsigned long, bool> RunFullGame() {
        NewGame();
        while (true) {
            if (HasReached2048() || IsGameOver()) break;
            int hintResult = Hint();
            if (hintResult <= 0) break;
        }
        return std::make_pair(m_score, HasReached2048());
    }
};

int main(int argc, char* argv[]) {
    int N = 100000;
    if (argc > 1) {
        N = std::atoi(argv[1]);
    }
    std::vector<std::pair<unsigned long, bool>> results;
    results.reserve(N);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < N; i++) {
        TestGame2048 game;
        results.push_back(game.RunFullGame());
        if ((i + 1) % (N / 10) == 0) {
            std::cout << "COMPLETED " << ((i + 1) / (N / 10))*10 << "% "<< std::endl;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    unsigned long total_score = 0, max_score = 0, min_score = ULONG_MAX;
    int wins = 0;
    for (const auto& result : results) {
        total_score += result.first;
        max_score = std::max(max_score, result.first);
        min_score = std::min(min_score, result.first);
        if (result.second) wins++;
    }
    
    double average_score = static_cast<double>(total_score) / N;
    double win_rate = static_cast<double>(wins) / N * 100.0;
    
    std::ofstream output_file("bin/output.txt");
    output_file << "=== Game2048 Hint() RESULT ===" << std::endl;
    output_file << "GAME TIMES: " << N << std::endl;
    output_file << "TOTAL TIME: " << duration.count() << " ms" << std::endl;
    output_file << "AVERAGE SCORE: " << average_score << std::endl;
    output_file << "HIGHEST SCORE: " << max_score << std::endl;
    output_file << "LOWEST SCORE: " << min_score << std::endl;
    output_file << "WIN RATE: " << win_rate << "%" << std::endl;
    output_file << "WINS: " << wins << "/" << N << std::endl;
    output_file.close();
    
    std::cout << "OUTPUT RESULT TO bin/output.txt" << std::endl;
    return 0;
}
EOF

if [ $? -ne 0 ]; then echo "COMPILE ERROR"; exit 1; fi

./bin/benchmark_test $1
if [ $? -eq 0 ]; then
    grep -E "(AVERAGE SCORE|HIGHEST SCORE|LOWEST SCORE|WIN RATE)" bin/output.txt
else
    echo "FATAL ERROR"
    exit 1
fi
