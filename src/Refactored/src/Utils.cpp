#include "Utils.h"

Position::Position(int x, int y): 
    x{x}, 
    y{y}
{}

Position::Position(const ::Position& pos): 
    x{pos.x}, 
    y{pos.y}
{}

int Position::operator[](const int index) const {
    if (index == 0)      return x;
    else if (index == 1) return y;
    else throw           IndexOutOfBoundError();
}

bool Position::operator==(const Position& lp) const {
    return lp.x == x && lp.y == y;
}

bool Position::operator<(const Position& rp) const {
    if (x < rp.x) return true;
    if (x > rp.x) return false;

    if (y < rp.y) return true;
    else          return false;
}

std::vector<Position> Direction::operator[] (const std::string& index) {
    return list.at(index);
}