#ifndef CHECKER_H
#define CHECKER_H

#include <map>

#include "Utils.h"
#include "Move.h"
#include "Board.h"

class Board; // Forward declaration
class Checker {
public:
    static const std::map<std::string, std::string> opponent; 

    std::string color;
    int row, col;
    bool isKing;

    // Constructors
    Checker(std::string color, int x, int y);
    Checker(const Checker& oldChecker);

    // Methods - Move related
    std::vector<Move> getPossibleMoves(Board& board);
    void DFSCapture(int pos_x, int pos_y, std::string& selfColor, Board& board, 
        std::vector<Position>& possibleDirections,
        std::vector<std::vector<Position>>& moveList, 
        std::vector<Position>& sequence);

    // Methods - Misc
    void becomeKing();
    void becomeMan();
    std::string toString();
};

#endif // CHECKER_H