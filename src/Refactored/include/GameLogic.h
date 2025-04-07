#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <string>

#include "AI.h"
#include "Board.h"
#include "StudentAI.h"
#include "ManualAI.h"

class GameLogic {
private:
	int col, row, p, order;
	std::string mode;
	std::vector<AI*>* aiList;

public:
	GameLogic(int col, int row, int p, std::string mode, int order);
	void Manual();
	void TournamentInterface();
	void Run();
	~GameLogic();
};

#endif //GAMELOGIC_H
