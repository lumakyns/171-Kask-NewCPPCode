#ifndef UTILS_H
#define UTILS_H

#include <exception>
#include <vector>
#include <map>
#include <string>

class Position {
public:
    int x, y;
    
    // Constructors
    Position(int x, int y);
    Position(const Position& pos);

    // Methods - Operators
    int operator[](const int index) const;
    bool operator==(const Position& lp) const;
    bool operator<(const Position& rp) const;
};

class Direction {
public:
    const std::map<std::string, const std::vector<Position>> list = {
        {"W", std::vector<Position>{Position(-1, -1), Position(-1, 1)}},
        {"B", std::vector<Position>{Position(1, -1),  Position(1, 1)}} 
    };
        
    std::vector<Position> operator[](const std::string& index);
    Direction() = default;
};

class MoveBuildError : public std::exception
{};

class IndexOutOfBoundError : public std::exception 
{};

class InvalidMoveError : public std::exception
{};

class InvalidParameterError : public std::exception
{};


#endif // UTILS_H