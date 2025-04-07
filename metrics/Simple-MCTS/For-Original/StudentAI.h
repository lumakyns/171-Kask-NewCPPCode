#ifndef STUDENT_AI_HPP
#define STUDENT_AI_HPP

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <utility>
#include <random>
#include <string>
#include <climits>
#include <random>
#include <chrono>
#include <limits>

#include "AI.h"
#include "Board.h"

class StudentAI : public AI {
private:
    struct SearchTree {
        int wins, games, captureScore;
        std::vector<std::pair<Move, SearchTree*>> options;

        // Constructors
        ~SearchTree();
        SearchTree();
        SearchTree(int winsNum, int gamesNum);

        // Methods
        std::pair<Move*, SearchTree*> selectBestMove(bool opponent);
    };

public:
    Board localBoard;
    SearchTree* MCTree;

    // Constructor
    StudentAI(int col, int row, int p);
    ~StudentAI();

    // Methods
    virtual Move GetMove(Move board);

    // AI Functions.
    Move 				UCT_MCTS();
    std::pair<int, int> MCTS_ExpandRollout(SearchTree* treeToUpdate, Board& currentBoard, int currentPlayer);
};

#endif // STUDENT_AI_HPP