#ifndef STUDENTAI_H
#define STUDENTAI_H

#include <random>

#include "AI.h"
#include "Board.h"

// NOTE: The following part should be completed by students.
//       Students can modify anything except the class name and exisiting functions and variables.
class StudentAI : public AI {
public:
    Board board;
	StudentAI(int col, int row, int p);
	virtual Move GetMove(Move board);
};

#endif //STUDENTAI_H
