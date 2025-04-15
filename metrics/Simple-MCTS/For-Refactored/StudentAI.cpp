#include "StudentAI.h"

// Constants
constexpr int ITERATIONS = 1000;           
constexpr double C = 1.41421356237;
constexpr bool DEBUGGING = true;

// Helpers
inline void DEBUG(std::string str, bool newline = true) {
    if (!DEBUGGING) return;

    std::cout << str << (newline ? "\n" : "");
}

inline int RANDOMNUM(int a, int b) {
    static std::random_device rd; 
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(a, b);
    return distrib(gen);
}

inline int switchPlayer(int playerNum) {
    return (playerNum == 1) ? 2 : 1;
}

// Given methods
StudentAI::~StudentAI() {
    delete MCTree;
}

StudentAI::StudentAI(int col, int row, int p) : AI(col, row, p) {
    localBoard = Board(::Board(col,row,p));
    localBoard.initializeGame();

    MCTree = new SearchTree(0, 0);

    player = 2;
}

Move StudentAI::GetMove(Move move) {
    if (move.sequence.empty()) player = 1;
    else {
        // Step 1. Update board
        localBoard.makeMove(move, player == 1 ? 2 : 1);

        // Step 2. Move the root of the searchTree
        bool movedRootSuccessfully = false;

        SearchTree* oldMCTree = MCTree;
        for (auto& movePair : oldMCTree->options) {
            if (movePair.first == move) {
                MCTree = movePair.second;
                movedRootSuccessfully = true;

                movePair.second = nullptr;
                break;
            }
        }

        // If the new root wasn't found, start from 0.
        if (!movedRootSuccessfully) {
            MCTree = new SearchTree(0, 0);
        }

        delete oldMCTree;
    }

    // To start board
    // ./main.exe {col} {row} {p} m {start_player (0 or 1)}
    //    Where...s
    //      {col}: Number of columns
    //      {row}: Number of rows
    //      {p}  : Number of rows filled with pieces
    //      {m/s}: Manual / Self (Self to run AI against itself)
    //      {start_player}: 0 for opponent, 1 for ourselves

    // Call AI function & get time
    Move finalMove = UCT_MCTS();

    return finalMove;
}

// Student Methods

Move StudentAI::UCT_MCTS() {

    // Step 1: Strengthen MCTS tree.
    for (int x = 0; x < ITERATIONS; ++x) {
        // Benchmarking
        auto start = std::chrono::high_resolution_clock::now();
        
        DEBUG("Iteration " + std::to_string(x + 1) + "\r", false);
        Board currentBoard  = localBoard;
        int currentPlayer = player;

        // NOTE: nextMove consists of a Move object, and the next part of the tree belonging to that Move.
        std::pair<Move*, SearchTree*> nextMove = MCTree->selectBestMove(false);
        std::vector<SearchTree*> traversed;
        
        // Step 1a: Find a leaf node
        while (nextMove.first != nullptr) {
            traversed.push_back(nextMove.second);
            currentBoard.makeMove(*(nextMove.first), currentPlayer);
            
            currentPlayer = switchPlayer(currentPlayer);
            nextMove = nextMove.second->selectBestMove(currentPlayer != player);
        }

        // Step 1b: Rollout & Backpropagate
        int leafWins  = 0;
        int leafGames = 0;
        std::pair<int, int> result = MCTS_ExpandRollout(nextMove.second, currentBoard, currentPlayer);
        leafWins += result.first;
        leafGames += result.second;

        for (SearchTree* selectedSearchTree : traversed) {
            selectedSearchTree->wins  += leafWins;
            selectedSearchTree->games += leafGames;
        }
    }
    DEBUG("");

    // Step 2a: Pick the best move.
    std::pair<Move*, SearchTree*> finalPair = MCTree->selectBestMove(false);
    Move finalMove = *(finalPair.first); 

    // Step 2b: Update the search tree
    SearchTree* oldMCTree = MCTree;
    MCTree = finalPair.second;

    for (auto& movePair : oldMCTree->options) {
        if (movePair.second == MCTree) {
            movePair.second = nullptr;
            break;
        }
    }

    delete oldMCTree;

    // Step 2c: Lock in move.
    localBoard.makeMove(finalMove, player);

    return finalMove;
}

// When at a leaf state, MCTS_ExpandRollout will expand the node, rollout all possible moves and return the scores.
std::pair<int, int> StudentAI::MCTS_ExpandRollout(SearchTree* treeToUpdate, Board& currentBoard, int currentPlayer) {
    if (currentBoard.isWin(currentPlayer) == player) return {1, 1};
    if (!treeToUpdate->options.empty()) {
        DEBUG("Error in ExpandRollout: Tree isn't empty.");
    }

    int wins  = 0;
    int games = 0;

    // Step 1: Make blank moves
    std::vector<std::vector<Move>> leafMoves = currentBoard.getAllPossibleMoves(currentPlayer);
    for (std::vector<Move>& chipMoves : leafMoves) {
        for (Move& action : chipMoves) {
            treeToUpdate->options.push_back({action, new SearchTree(0, 0)});
        }
    }

    // Step 2a: Set up new board for random play
    Board childBoard(currentBoard);
    int childPlayer = currentPlayer;

    // Step 2b: Random play
    int rolloutResult = childBoard.isWin(childPlayer);

    while (rolloutResult == 0) {
        std::vector<std::vector<Move>> randomOptions = childBoard.getAllPossibleMoves(childPlayer);

        int i = RANDOMNUM(0, randomOptions.size()-1);
        int j = RANDOMNUM(0, randomOptions[i].size()-1);

        childBoard.makeMove(randomOptions[i][j], childPlayer);
        childPlayer = switchPlayer(childPlayer);
        
        rolloutResult = childBoard.isWin(childPlayer);
    }

    // Step 2c: Results of random play
    bool weWon = (rolloutResult == player); 

    if (weWon) ++wins;
    ++games;

    treeToUpdate->wins  = wins;
    treeToUpdate->games = games;

    return {wins, games};
}


StudentAI::SearchTree::~SearchTree() {
    for (const auto& pair : options) {
        delete pair.second; // Downstream SearchTree
    }
}

StudentAI::SearchTree::SearchTree(): 
    wins{0}, 
    games{0}
{}

StudentAI::SearchTree::SearchTree(int winsNum, int gamesNum): 
    wins{winsNum}, 
    games{gamesNum}
{}

// Selects the best move using the UCT formula
std::pair<Move*, StudentAI::SearchTree*> StudentAI::SearchTree::selectBestMove(bool opponent) {
    if (this->games == 0) return {nullptr, this};

    double bestScore     = std::numeric_limits<double>::lowest();
    Move* bestMove       = nullptr;
    SearchTree* bestNode = nullptr;

    for (auto& movePair : options) {
        Move* nextMove = &movePair.first;
        SearchTree* nextNode = movePair.second;

        double childWins = nextNode->wins;
        double childGames = nextNode->games;
        double parentGames = this->games;

        // If this child has no games, we should pick it to expand
        if (childGames == 0) {
            bestMove = nextMove;
            bestNode = nextNode;
            break;
        }

        // If we're selecting the best move for our opponent, we flip the games we won.
        if (opponent) childWins = childGames - childWins;

        double uctScore = (childWins / childGames) + (C * std::sqrt(std::log(parentGames) / childGames));

        if (uctScore > bestScore) {
            bestScore = uctScore;
            bestMove = nextMove;
            bestNode = nextNode;
        }
    }

    if (bestMove == nullptr) return {nullptr, this};
    else                     return {bestMove, bestNode};
}
