//
//  Utilities.cpp
//  Draughts_NEA

#include "Utilities.hpp"

// Implementation of overloading << for Position and Colour
std::ostream& operator << (std::ostream& os, const Position& pos)
{
    os << '(' << pos.x << ", " << pos.y << ")";
    return os;
}


std::ostream& operator << (std::ostream& os, const Colour& colour)
{
    switch (colour)
    {
        case White:
            os << "White";
            break;
            
        case Black:
            os << "Black";
            break;
            
        case NoColour:
            os << "NoColour";
            break;
            
        default:
            break;
    }
    
    return os;
}
