#include "Game2048_Logic.h"
#include "Game2048_Data.h"        // 需要GameState的完整定义
#include <cstdlib>      // for rand() and srand()
#include <ctime>        // for time()


Game2048Logic::Game2048Logic() {
    //TODO:Initiallize the game board and other relevant variables
}

bool Game2048Logic::ProcessMove(int direction) {
    //TODO:use 0123 to represent up, down, left, right respectively,just do the process under each direction
    // return true if the move is successful, false if the move cannot be made
}


void Game2048Logic::AddNewNumber() {
    // TODO:Add a new number to a random empty position on the board,which 90% chance is 2 and 10% chance is 4
}

bool Game2048Logic::IsGameOver() const {
    //TODO:Check if there are no valid moves left
}

bool Game2048Logic::HasReached2048() const {
    //TODO:Check if the player has reached the 2048 tile
}

void Game2048Logic::NewGame() {
    //TODO:reset the game state, and do the initialization for a new game
}

void Game2048Logic::Redo() {
    //to redo the last move, which is the first step in the redo history
    //Note: you couldn't call this function if the redo history is empty and the redo count is 0
}

void Game2048Logic::SaveRecord() {
    // save the current game state to the history for redo functionality
}

bool Game2048Logic::IsAllZero(const std::array<std::array<unsigned int, 4>, 4>& board) const {
    // Check if all elements in the board are zero
}

void Game2048Logic::CopyBoard(std::array<std::array<unsigned int, 4>, 4>& dest, 
                        const std::array<std::array<unsigned int, 4>, 4>& src) const {
    // Copy the contents of src board to dest board
}

int Game2048Logic::ChooseDirection() const {
    // choose a random direction for the AI to move,you do not need to change it before extension
    // return a number between 0 and 3, representing up, down, left, right respectively
    return rand() % 4;
}

int Game2048Logic::Hint() {
    // to do the hint for the player, which would be a smart move, though it is a random move now
    // you do not need to change it before extension
    // return 1 if the move is successful, -1 if the game is over, 0 if no valid move
    if (IsGameOver()) return 0; 
    bool moved = false;
    std::vector<int> triedDirections; 
    while (!moved && triedDirections.size() < 4) {
        int direction = ChooseDirection(); 
        bool alreadyTried = false;
        for (int tried : triedDirections) {
            if (tried == direction) {
                alreadyTried = true;
                break;
            }
        }
        if (alreadyTried) continue; 
        triedDirections.push_back(direction);
        moved = ProcessMove(direction);
    }
    if (moved) {
        AddNewNumber();
        SaveRecord();
        m_highScore = std::max(m_score, m_highScore);
        return 1; 
    } else {
        return -1; 
    }
}