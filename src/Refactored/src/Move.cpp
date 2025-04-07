#include "Move.h"

/*
* MOVE
*   `Move` contains a sequence of jumps (Possibly many during a chain capture)
*/
Move::Move()
{}

Move::Move(const Move& oldMove) {
    for (const Position& pos : oldMove.sequence) {
        sequence.push_back(pos);
    }
}

Move::Move(const std::vector<Position>& seq): 
    sequence{seq}
{}

Move::Move(const std::string& input) {
    if (input == "-1") return;

    // Turns a '-' delimited sequence of coordinates into a move.
    std::vector<std::string> points = split(input, "-");
    for (const auto& pt : points) {
        std::vector<std::string> coords = split(pt.substr(1, pt.size()-2), ",");

        sequence.push_back(Position(std::stoi(coords[0]), std::stoi(coords[1])));
    }
}

std::vector<std::string> Move::split(std::string input, std::string delimiter) {
    
    size_t pos = 0;
    std::vector<std::string> result;
    
    while ((pos = input.find(delimiter)) != std::string::npos) {
        result.push_back(input.substr(0, pos));
        input.erase(0, pos + delimiter.length());
    }

    result.push_back(input);
    return result;
}

std::string Move::toString() {
    std::string result;
    for (int i = 0; i < sequence.size();++i) {
        result += "(" + std::to_string(sequence[i][0]) + "," + std::to_string(sequence[i][1]) + ")";

        if (i != sequence.size() - 1) {
            result += "-";
        }
    }
    return result;
}

// todo: strictify
bool Move::isCapture() {
    return (sequence.size() > 2) || std::abs(sequence[0][0] - sequence[1][0]) > 1;
}

bool Move::operator==(const Move& rhs) const {
    return (sequence == rhs.sequence);
}