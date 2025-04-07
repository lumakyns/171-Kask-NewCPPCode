#include "Board.h"

/*
* BOARD
*   `Board` is the current state of the game board
*   The board also contains previously played moves
*/
const std::map<std::string, std::string> Board::opponent  = {{"W","B"},{"B","W"}};

Board::Board(): 
    col{0}, 
    row{0}, 
    p{0}
{}

Board::Board(int col, int row, int p): 
    col{col}, 
    row{row}, 
    p{p}, 
    tieCount{0}, 
    tieMax{40}
{
    // Make an empty board
    board = std::vector<std::vector<Checker>>(row, std::vector<Checker>(col, Checker(".", 0, 0)));
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            board[i][j].row = i;
            board[i][j].col = j;
        }
    }
}

void Board::showBoard() {
    // Top part of board
    std::cerr << std::setw(2) << " ";
    for (int j = 0; j < col; ++j) {
        std::cerr << std::setw(3) << j;
    }
    std::cerr << "\n";

    // Pieces
    for (int i = 0; i < row; ++i) {
        std::cerr << i << "|";
        for (int j = 0; j < col; ++j) {
            std::cerr << std::setw(3) << board[i][j].toString();
        }
        std::cerr << "\n";
    }

    // Bottom part of board
    for (int j = 0; j < col; ++j) {
        std::cerr << "----";
    }
    std::cerr << "\n";
}

void Board::initializeGame() {
    checkInitialVariable();

    // Place alternating pieces
    for (int i = p; i >= 0; i--) {
        for (int j = (p - i - 1) % 2; j >= 0 && j < col; j += 2) {

            // White pieces
            int i_white = row - p + i;
            board[i_white][j] = Checker("W", i_white, j);

            // Black Pieces
            if (((row % 2) + (p % 2)) % 2 == 1) {
                // Even row: Shift pieces to the left
                if (i % 2 == 1) {
                    if (j - 1 >= 0) {
                        board[i][j-1] = Checker("B", i, j-1);
                    }

                    if ((j == col - 2) && (col % 2 == 0)) {
                        board[i][col - 1] = Checker("B", i, col - 1);
                    }
                } 
                
                // Odd row: Shift pieces to the right
                else {
                    if (j + 1 <= col -1) {
                        board[i][j+1] = Checker("B", i, j+1);
                    }

                    if ((j == col - 1 || j == col - 2) && (p % 2 == 0)) {
                        board[i][0] = Checker("B", i, 0);
                    }
                }
            } 
            
            else {
                board[i][j] = Checker("B", i, j);
            }
        }
    }
}

void Board::checkInitialVariable() {
    // Q > 0
    if (row - 2 * p <= 0) {
        std::cerr << "Q <= 0\n";
        throw InvalidParameterError();
    }

    // M = 2P + Q
    if (row != 2 * p + (row - 2 * p)) {
        std::cerr << "M != 2P + Q\n";
        throw InvalidParameterError();
    }

    // N*P is even
    if (col * p % 2 != 0) {
        std::cerr << "N*P is odd\n";
        throw InvalidParameterError();
    }
}

bool Board::isValidMove(int chess_row, int chess_col, int target_row, int target_col, std::string turn) {
    // Out of bounds
    if (!isInBoard(target_row, target_col)) return false;
    
    // Moving an invalid / non-existent piece
    if (board[target_row][target_col].color != "." ||
        board[chess_row][chess_col].color != turn)
    {
        return false;
    }

    // Invalid move
    int diff_col = target_col - chess_col;
    int diff_row = target_row - chess_row;
    if (abs(diff_col) != abs(diff_row)) return false;
    
    // Check other cases
    Checker chess_being_moved = board[chess_row][chess_col];
    if (diff_row == 1 && diff_col == 1) {
        return chess_being_moved.isKing || chess_being_moved.color == "B";
    }

    else if (diff_row == 1 && diff_col == -1) {
        return chess_being_moved.isKing || chess_being_moved.color == "B";
    }

    else if (diff_row == -1 && diff_col == 1) {
        return chess_being_moved.isKing || chess_being_moved.color == "W";
    }

    else if (diff_row == -1 && diff_col == -1) {
        return chess_being_moved.isKing || chess_being_moved.color == "W";
    }
        
    else if (diff_row == 2 && diff_col == 2) {
        return (chess_being_moved.isKing || chess_being_moved.color == "B") && 
                board[chess_row + 1][chess_col + 1].color != turn           && 
                board[chess_row + 1][chess_col + 1].color != ".";
    }

    else if (diff_row == 2 && diff_col == -2) {
        return (chess_being_moved.isKing || chess_being_moved.color == "B") && 
                board[chess_row + 1][chess_col - 1].color != turn           && 
                board[chess_row + 1][chess_col - 1].color != ".";
    }

    else if (diff_row == -2 && diff_col == 2) {
        return (chess_being_moved.isKing || chess_being_moved.color == "W") && 
                board[chess_row - 1][chess_col + 1].color != turn           && 
                board[chess_row - 1][chess_col + 1].color != ".";
    }
    
    else if (diff_row == -2 && diff_col == -2) {
        return (chess_being_moved.isKing || chess_being_moved.color == "W") && 
                board[chess_row - 1][chess_col - 1].color != turn           && 
                board[chess_row - 1][chess_col - 1].color != ".";
    }
    
    else return false;
}

std::vector<std::vector<Move>> Board::getAllPossibleMoves(std::string color) {
    // Find all pieces' moves
    bool foundCapture = false;

    std::vector<std::vector<Move>> movesPerPiece;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (board[i][j].color != color) continue;
    
            movesPerPiece.emplace_back(board[i][j].getPossibleMoves(*this));

            // No moves obtained
            if (movesPerPiece.back().empty()) {
                movesPerPiece.pop_back();
                continue;
            }

            bool newCapture = movesPerPiece.back().front().isCapture();

            // If the newest added move(s) is a capture, begin only adding captures to the list
            if (newCapture && !foundCapture) {
                movesPerPiece.erase(movesPerPiece.begin(), --movesPerPiece.end());
                foundCapture = true;
            }

            else if (!newCapture && foundCapture) {
                movesPerPiece.pop_back();
            }
        } 
    }

    return movesPerPiece;
}

std::vector<std::vector<Move>> Board::getAllPossibleMoves(int player) {
    return getAllPossibleMoves((player == 1 ? "B" : "W"));
}

bool Board::isInBoard(int pos_x, int pos_y) {
    return (0 <= pos_x && pos_x < row && 
            0 <= pos_y && pos_y < col);
}

void Board::makeMove(Move move, int player) {
    // Create a new saved move
    SavedMove newSavedMove;
    newSavedMove.moveMade = move;
    
    // Decide player 
    std::string turn = "";
    if (player == 1)      turn = "B";
    else if (player == 2) turn = "W";
    else throw InvalidMoveError();
    
    std::vector<std::vector<int>> captureLogList;

    bool isStartCheckerKing = board[move.sequence[0][0]][move.sequence[0][1]].isKing;
    bool ifCapture = false;
    tieCount += 1;

    // Check all jumps are consistent
    for (int i = 0; i < move.sequence.size() - 1; ++i) {
        Position start  = move.sequence[i];
        Position target = move.sequence[i+1];

        // If jump is invalid, throw error
        if (!isValidMove(start[0], start[1], target[0], target[1], turn) && 
            !(ifCapture && (std::abs(start[0] - target[0]) == 1)))
        {
            throw InvalidMoveError();
        }

        // Play out single jump
        board[start[0] ][start[1] ].color = ".";
        board[target[0]][target[1]].color = turn;
        board[target[0]][target[1]].isKing = board[start[0]][start[1]].isKing;
        board[start[0] ][start[1] ].becomeMan();

        // If this move was a capture
        if (std::abs(start[0] - target[0]) == 2) {
            ifCapture = true;
            tieCount = 0;

            // The piece that was captured
            Position capturedPosition{start[0] + ((target[0]-start[0]) / 2), start[1] + ((target[1]-start[1]) / 2)};

            // NOTE: captureLog records some statistics about the single captured piece
            std::vector<int> captureLog;

            // Record capture position
            captureLog.push_back(capturedPosition[0]); // row
            captureLog.push_back(capturedPosition[1]); // col

            bool capturedIsBlack = board[capturedPosition[0]][capturedPosition[1]].color == "B";
            bool capturedIsKing  = board[capturedPosition[0]][capturedPosition[1]].isKing;

            // Push back color 1 = "B" 2 = "W"
            if (capturedIsBlack) captureLog.push_back(1);
            else                 captureLog.push_back(2);
            
            // the checker is regular or king, 0 = regular, 1 = king
            if (capturedIsKing) captureLog.push_back(1);
            else                captureLog.push_back(0);
                
            captureLogList.push_back(captureLog);
            
            // Update board
            board[capturedPosition[0]][capturedPosition[1]] = Checker(".", capturedPosition[0], capturedPosition[1]);
        }

        // Promote piece if needed
        if ((turn == "B" && target[0] == row - 1) ||
            (turn == "W" && target[0] == 0      )) 
        {
            newSavedMove.becomeKing = !isStartCheckerKing;

            board[target[0]][target[1]].becomeKing();
            if (!isStartCheckerKing) break;
        }

        else newSavedMove.becomeKing = false;
    }

    newSavedMove.savedEnemyList = captureLogList;
    savedMoveList.push_back(newSavedMove);
}

int Board::isWin(int turn) {
    // Tie limit reached.
    if (tieCount >= tieMax) return -1;

    // If a player has no moves, they lose
    if (turn == 1 && getAllPossibleMoves(1).empty()) return 2;
    if (turn == 2 && getAllPossibleMoves(2).empty()) return 1;

    // If either player has no pieces, they lose
    bool W = true, B = true;
        
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (board[i][j].color == ".") continue;
            if (board[i][j].color == "W") W = false;
            else                          B = false;

            if (!W && !B) return 0;
        }
    }        
    
    if (W)      return 1;
    else if (B) return 2;
    else        return 0;
}

int Board::isWin(std::string turn_s) {
    return isWin((turn_s == "W" ? 2 : 1));
}

void Board::undo() {
    // If board isn't empty
    if (!savedMoveList.empty()) {
        SavedMove lastMove = savedMoveList.back();

        Position from = lastMove.moveMade.sequence.front();
        Position to   = lastMove.moveMade.sequence.back();
        
        // Undo piece move
        board[from[0]][from[1]].color = board[to[0]][to[1]].color;

        // Undo becomeKing if needed
        if (lastMove.becomeKing) board[from[0]][from[1]].isKing = false;
        else                     board[from[0]][from[1]].isKing = board[to[0]][to[1]].isKing;

        if (to != from) {
            board[to[0]][to[1]].color = ".";
            board[to[0]][to[1]].isKing = false;
        }

        // Undo chain capture
        for (const auto& captureLog : lastMove.savedEnemyList) {
            int x = captureLog[0];
            int y = captureLog[1];
            int c = captureLog[2];
            int k = captureLog[3];
            
            board[x][y].color  = (c == 1 ? "B"   : "W");
            board[x][y].isKing = (k == 0 ? false : true);
        }

        // Remove move from list
        --tieCount;
        savedMoveList.pop_back();
    }
}
