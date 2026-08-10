//
//  Utilities.hpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <math.h>
#include <set>
#include <filesystem>

// For printing when debuging
#ifndef Debug
    #define Debug(note) (std::cerr << "<" << std::filesystem::path(__FILE__).filename().string() << "> <function=" << __FUNCTION__ << "> <line=" << __LINE__ << ">: " << note << std::endl)
#else
    #define Debug(note)
#endif

// Debug flag for the Game object (do not use if playing against algorithms)
//#ifndef DEBUG_FLAG
//    #define DEBUG_FLAG
//#endif

// Debug flag for the Minimax algorithm
//#ifndef DEBUG_FLAG_MINIMAX
//    #define DEBUG_FLAG_MINIMAX
//#endif

// Debug flag for the Monte Carlo Tree Search algorithm
//#ifndef DEBUG_FLAG_MCTS
//    #define DEBUG_FLAG_MCTS
//#endif

// Debug flag for the Alpha Beta Pruning algorithm
//#ifndef DEBUG_FLAG_ALPHABETAPRUNING
//    #define DEBUG_FLAG_ALPHABETAPRUNING
//#endif

// Debug flag for the Console object
//#ifndef DEBUG_FLAG_TIME
//    #define DEBUG_FLAG_TIME
//#endif



// For piece colour
// When printing on the board
// Black - 'X'
// White - 'O'
// NoColour - ' '
enum Colour
{
    Black,
    White,
    NoColour
};
// Declaration of overloading << operator for priting the string equivalent of each enum value
std::ostream& operator << (std::ostream& os, const Colour& colour);


// Position structure to hold (x, y) coordinate of a piece on the board
struct Position
{
    int x;
    int y;
    
    
    // Operator overloading
    bool operator == (const Position &toComparePosition) const
    {
        return x == toComparePosition.x && y == toComparePosition.y;
    }
    
    bool operator != (const Position &toComparePosition) const
    {
        return x != toComparePosition.x || y != toComparePosition.y;
    }
    
    // Compares as if in a 1D array, with top-left corner as 0 and bottom-right corner as 64
    bool operator < (const Position &toComparePosition) const
    {
        return y + (x * 8) < toComparePosition.y + (toComparePosition.x * 8);
    }
    
    Position operator + (const Position &toAddPosition) const
    {
        return Position { x + toAddPosition.x, y + toAddPosition.y };
    }
    
    Position operator + (const int &iToAdd) const
    {
        return Position { x + iToAdd, y + iToAdd };
    }
    
    Position operator - (const Position &toAddPosition) const
    {
        return Position { x - toAddPosition.x, y - toAddPosition.y };
    }
    
    Position operator - (const int &iToSub) const
    {
        return Position { x - iToSub, y - iToSub };
    }

};
// Declaration of overloading << operator for priting the string equivalent of a position as (x, y)
std::ostream& operator << (std::ostream& os, const Position& pos);

// Implementation of overloading << operator for priting a vector of positions
template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& vector)
{
    os << "{";
    for (int i = 0 ; i < vector.size(); i++)
    {
        os << vector.at(i);
        
        if (i != (vector.size() - 1) )
        {
            os << ", ";
        }
    }
    os << "}";
    
    return os;
}

// Implementation of overloading << operator for priting a set of positions
template <typename T>
std::ostream& operator << (std::ostream& os, const std::set<T>& set)
{
    os << "{";
    
    typename std::set<T>::iterator setIterator = set.begin();
    
    for (; setIterator != set.end(); setIterator++)
    {
        os << * setIterator;
       
        
        if (setIterator != (--set.end()) )
        {
            os << ", ";
        }
    }
    os << "}";
    
    return os;
}

namespace std
{
    // Hash function of a position; used by std::set
    template <> class hash<Position>
    {
        public:
            std::size_t operator()(const Position& position) const
            {
                return position.y * 8 + position.x;
            }
    };

}

#endif /* Utilities_hpp */
