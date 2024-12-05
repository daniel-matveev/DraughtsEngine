//
//  Utilities.hpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 10/10/2024.
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


#ifndef Debug
    #define Debug(note) (std::cerr << "<" << std::filesystem::path(__FILE__).filename().string() << "> <function=" << __FUNCTION__ << "> <line=" << __LINE__ << ">: " << note << std::endl)
#else
    #define Debug(note)
#endif


#ifndef DEBUG_FLAG
    #define DEBUG_FLAG
#endif
//#ifndef DEBUG_FLAG_MINIMAX
//    #define DEBUG_FLAG_MINIMAX
//#endif
//
//#ifndef DEBUG_FLAG_MCTS
//    #define DEBUG_FLAG_MCTS
//#endif

//#ifndef DEBUG_FLAG_ALPHABETAPRUNING
//    #define DEBUG_FLAG_ALPHABETAPRUNING
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
                std::size_t hashedX = std::hash<int>()(position.x);
                std::size_t hashedY = std::hash<int>()(position.y);
                
                return hashedX ^ (hashedY << 1);
            }
    };

}


#endif /* Utilities_hpp */



