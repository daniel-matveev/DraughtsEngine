//
//  Utilities.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 10/10/2024.
//

#include "Utilities.hpp"

// Implementation of overloading << for Position and Colour
std::ostream& operator << (std::ostream& os, const Position& pos)
{
    os << '(' << pos.x << ", " << pos.y << ")";
    return os;
}

//template <typename T>
//std::ostream& operator << (std::ostream& os, const std::vector<T>& vector)
//{
//    os << "{";
//    for (int i = 0 ; i < vector.size(); i++)
//    {
//        os << vector.at(i);
//        
//        if (i != (vector.size() - 1) )
//        {
//            os << ", ";
//        }
//    }
//    os << "}\n";
//    
//    return os;
//}


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


