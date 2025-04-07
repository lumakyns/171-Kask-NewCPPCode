#include "StudentAI.h"

// The following part should be completed by students.
// The students can modify anything except the class name and exisiting functions and variables.
StudentAI::StudentAI(int col,int row,int p) : AI(col, row, p) {
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

Move StudentAI::GetMove(Move move) {
    if (move.sequence.empty()) player = 1;
    else board.makeMove(move, player == 1 ? 2 : 1);

    std::vector<std::vector<Move>> moves = board.getAllPossibleMoves(player);

    // Currently playing a random move
    int i = rand() % (moves.size());
    int j = rand() % (moves[i].size());
    board.makeMove(moves[i][j], player);

    return moves[i][j];
}

