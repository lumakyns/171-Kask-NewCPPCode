#ifndef AI_H
#define AI_H

#include "Move.h"
#include "Board.h"

class AI {
protected:
	int col, row, p, player;
    

public:
	AI(int col, int row, int p): col{col}, row{row}, p{p}
	{}

	virtual Move GetMove(Move move) = 0;
	virtual ~AI(){}
};

#endif //AI_H
