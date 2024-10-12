#include "GameBoard.h"
#include <cmath>
#include <algorithm>

// Evaluation function for the board state
double evaluate(const GameBoard& board) {
    double score = 0;
    for (const auto& row : board.board) {
        for (int value : row) {
            score += value;
        }
    }
    return score;
}

// Expectimax recursive function
double expectimax(GameBoard& board, int depth, bool isMax) {
    if (board.isGameOver() || depth == 0) {
        return evaluate(board);
    }

    if (isMax) {
        // Maximize player's score
        double maxValue = -INFINITY;
        for (int dir = 0; dir < 4; ++dir) {
            GameBoard newBoard = board.simulateMove(dir);
            if (!newBoard.equals(board)) {
                maxValue = std::max(maxValue, expectimax(newBoard, depth - 1, false));
            }
        }
        return maxValue;
    } else {
        // Expectation over chance nodes (tile spawns)
        double totalValue = 0;
        auto emptyCells = board.getEmptyCells();
        int emptyCount = emptyCells.size();

        for (const auto& cell : emptyCells) {
            GameBoard boardWith2 = board;
            boardWith2.spawnTile(cell.first, cell.second, 2);
            totalValue += 0.9 * expectimax(boardWith2, depth - 1, true) / emptyCount;

            GameBoard boardWith4 = board;
            boardWith4.spawnTile(cell.first, cell.second, 4);
            totalValue += 0.1 * expectimax(boardWith4, depth - 1, true) / emptyCount;
        }
        return totalValue;
    }
}

// Process AI's move
void processAI(GameBoard& board) {
    double bestScore = -INFINITY;
    int bestMove = 0;

    for (int dir = 0; dir < 4; ++dir) {
        GameBoard newBoard = board.simulateMove(dir);
        if (!newBoard.equals(board)) {
            double score = expectimax(newBoard, 3, false);  // Adjust depth as needed
            if (score > bestScore) {
                bestScore = score;
                bestMove = dir;
            }
        }
    }

    board.move(bestMove);
}
