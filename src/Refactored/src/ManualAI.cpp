#include "ManualAI.h"

ManualAI::ManualAI(int col, int row, int p) : AI(col, row, p) {
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

Move ManualAI::GetMove(Move move) {
    if (move.sequence.empty()) player = 1;  
    else board.makeMove(move, (player == 1) ? 2 : 1);
    
    std::vector<std::vector<Move>> moves = board.getAllPossibleMoves(player);

    // Print moves for selection
    for (int i = 0; i < moves.size(); i++) {
        std::cout << i << " : [";
        for (int j = 0; j < moves[i].size(); j++) {
            std::cout << j << " : " << moves[i][j].toString() << ", ";
        }
        std::cout << "]\n";
    }

    std::cout << "Waiting input {int} {int}: ";
    int n = -1, m = -1;

    do {
        std::cin >> n;
        std::cin >> m;
        if ((n < 0 || n >= moves.size()) || (m < 0 || m >= moves[n].size())) {
            std::cout << "Invalid move" << "\n";
            std::cout << "Waiting input {int} {int}: ";
        }
            
    } while ((n < 0 || n >= moves.size()) || (m < 0 || m >= moves[n].size()));

    board.makeMove(moves[n][m], player);
    return moves[n][m];
}

