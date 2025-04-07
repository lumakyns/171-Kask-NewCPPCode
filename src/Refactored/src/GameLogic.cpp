#include "GameLogic.h"

GameLogic::GameLogic(int col, int row, int p, std::string mode, int order):
    col{col},
    row{row},
    p{p},
    mode{mode},
    order{order},
    aiList{new std::vector<AI*>()}
{}

void GameLogic::Manual() {
	int player = 1, winPlayer = 0;
	bool init = true;
	Move move;

	Board board(col, row, p);
    board.initializeGame();
    board.showBoard();

	while (true) {
        // Get current player's move
		move = ((*aiList)[player - 1])->GetMove(move);
		std::cout << move.toString() << "\n";

		try {
			board.makeMove(move, player);
		}
		
        catch (InvalidMoveError) {
			std::cerr << "Invalid Move!" << std::endl;
			winPlayer = player == 1 ? 2 : 1;
			break;
		}

        // Decide winner, if there is one
		winPlayer = board.isWin(player);
		board.showBoard();

		if (winPlayer != 0) break;
		player = player == 1 ? 2 : 1;
	}

	if (winPlayer == -1) std::cout << "Tie\n";

	else std::cout << "Player " << winPlayer << " wins!\n";
}

void GameLogic::TournamentInterface() {
	StudentAI ai(col, row, p);
    
	while (true) {
		std::string instr;
		std::cin >> instr;

		Move result = ai.GetMove(Move(instr));

		std::cout << result.toString() << "\n";
	}
}

void GameLogic::Run() {
    if (mode == "t") {
        TournamentInterface();
        return;
    }

    AI* studentAi = nullptr;
    AI* manualAi = nullptr;

	if (mode == "m" or mode == "manual") {
        studentAi = new StudentAI(col, row, p);
        manualAi = new ManualAI(col, row, p);
    }

    else if (mode == "s" or mode == "self") {
        studentAi = new StudentAI(col, row, p);
        manualAi = new StudentAI(col, row, p);
    }

    // Decide player order
    if (order == 1) {
        aiList->push_back(manualAi);
        aiList->push_back(studentAi);
    }

    else {
        aiList->push_back(studentAi);
        aiList->push_back(manualAi);
    }

    Manual();
}

GameLogic::~GameLogic() {
	while (!aiList->empty()) {
		delete aiList->back();
		aiList->pop_back();
	}
}
