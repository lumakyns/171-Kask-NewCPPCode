#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

#include "Move.h"
#include "Checker.h"
#include "Board.h"

/*
* SAVED MOVE
*   Every move consists of a jump, as well as captured pieces, kept inside `SavedMove`
*   This info is kept in case an Undo occurs.
*/
struct SavedMove {
public:
    std::vector<std::vector<int>> savedEnemyList;
    Move moveMade;
    bool becomeKing;
};

class Checker; // Forward declaration
class Board {
public:
    static const std::map<std::string, std::string> opponent;

    std::vector<std::vector<Checker>> board;
    std::vector<SavedMove> savedMoveList;
    
    int col, row, p;
    int tieCount, tieMax;

    // Constructors
    Board();
    Board(int col, int row, int p);

    // Methods - Move related
    bool isValidMove(int chess_row, int chess_col, int target_row, int target_col, std::string turn);
    std::vector<std::vector<Move>> getAllPossibleMoves(std::string color);
    std::vector<std::vector<Move>> getAllPossibleMoves(int player);
    
    void makeMove(Move move, int player);
    void undo();

    // Methods - Checks
    void checkInitialVariable();
    bool isInBoard(int pos_x, int pos_y);
    int isWin(int turn);
    int isWin(std::string turn);

    // Methods - Misc
    void initializeGame();
    void showBoard();
};

#endif //BOARD_H