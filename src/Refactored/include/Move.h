#ifndef MOVE_H
#define MOVE_H

#include "Utils.h"

class Move {
public:
    std::vector<Position> sequence;

    // Constructors
    Move();
    Move(const Move& move);
    Move(const std::vector<Position>& seq);
    Move(const std::string& input);

    // Methods
    std::vector<std::string> split(std::string input, std::string delimiter);
    std::string toString();
    bool isCapture();

    bool operator==(const Move& rhs) const;
};

#endif //MOVE_H