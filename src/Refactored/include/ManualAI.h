#ifndef MANUALAI_H
#define MANUALAI_H

#include <iostream>

#include "AI.h"

class ManualAI : public AI {
public:
    Board board;
	ManualAI(int col, int row, int p);
	virtual Move GetMove(Move board);
};

#endif //MANUALAI_H
