#include "Checker.h"

/* 
* CHECKER
*   `Checkers` contain an a piece's position and state.
*/
const std::map<std::string, std::string> Checker::opponent = {{"W","B"},{"B","W"}};

Checker::Checker(std::string color_, int row_, int col_):
    color{(color_[0] <= 90) ? color_ : std::string(1, (color_[0] - 32))}, 
    row{row_}, 
    col{col_}, 
    isKing{false}
{}

Checker::Checker(const Checker& oldChecker):
    color{oldChecker.color},
    row{oldChecker.row}, 
    col{oldChecker.col}, 
    isKing{oldChecker.isKing}
{}

void Checker::becomeKing() {
    isKing = true;
};

void Checker::becomeMan() {
    isKing = false;
};

std::string Checker::toString() {
    if (isKing) return color;
    else        return (color == ".") ? color : std::string(1, (color[0] + 32));
}

std::vector<Move> Checker::getPossibleMoves(Board& board) {
    if (color == ".") return {};

    // Helper object
    Direction direction;

    std::vector<Move> result;
    std::vector<Position> possibleDirections = direction[color];

    // Allows a king checker to move in all directions
    if (isKing) {
        for (const auto& dir : direction[opponent.at(color)]) {
            possibleDirections.push_back(dir);
        }
    }

    // Option 1: Find all capture moves
    std::string selfColor = ".";
    std::swap(selfColor, board.board[row][col].color);

    // Run a DFS to find all consecutive captures
    std::vector<std::vector<Position>> moveList;
    std::vector<Position> sequence;
    DFSCapture(row, col, selfColor, board, possibleDirections, moveList, sequence);

    std::swap(selfColor, board.board[row][col].color);

    if (!moveList.empty()) {
        // Add all capture moves to the list.
        for (auto& jump : moveList) {
            jump.insert(jump.begin(), Position(row, col)); // Add the initial position at the beggining.
            result.emplace_back(Move(jump));
        }

        return result;
    }

    // Option 2: If no capture moves were found, return regular moves
    for (const auto& dir : possibleDirections) {
        int pos_x = row + dir[0];
        int pos_y = col + dir[1];

        if (board.isInBoard(pos_x, pos_y)) {
            if (board.board[pos_x][pos_y].color == ".") {
                // Add this position as a move, with a single jump
                result.emplace_back(
                    Move(std::vector<Position>{Position(row, col), Position(pos_x, pos_y)})
                );
            }
        }
    }

    return result;
}

void Checker::DFSCapture(int pos_x, int pos_y, std::string& selfColor, Board& board, 
                         std::vector<Position>& possibleDirections,
                         std::vector<std::vector<Position>>& moveList, std::vector<Position>& sequence)
{
    bool foundCapture = false;

    // For every direction the checker can go in...
    for (auto& dir : possibleDirections) {
        int jump_x = pos_x + dir[0];
        int jump_y = pos_y + dir[1];
        int capt_x = jump_x + dir[0];
        int capt_y = jump_y + dir[1];

        // Check if there's a piece to be captured.
        if (board.isInBoard(jump_x, jump_y) 
            && board.board[jump_x][jump_y].color == opponent.at(selfColor)
            && board.isInBoard(capt_x, capt_y)
            && board.board[capt_x][capt_y].color == ".")
        {
            foundCapture = true;

            std::string pieceBackup = ".";
            std::swap(pieceBackup, board.board[jump_x][jump_y].color);
            sequence.push_back(Position{capt_x, capt_y});

            DFSCapture(capt_x, capt_y, selfColor, board, possibleDirections, moveList, sequence);

            sequence.pop_back();
            std::swap(pieceBackup, board.board[jump_x][jump_y].color);
        }
    }

    // If we can't find any more moves to make, add this sequence of moves to our final list.
    if (!foundCapture && !sequence.empty()) {
        moveList.push_back(sequence);
        return;
    }
}