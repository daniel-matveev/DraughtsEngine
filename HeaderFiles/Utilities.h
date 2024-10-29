//
//  Utilities.h
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 09/10/2024.
//

#ifndef Utilities_h
#define Utilities_h

#include <stdio.h>
#include <iostream>

enum Colour
{
    Black,
    White,
    NoColour
};


struct Position
{
    int x;
    int y;
    
    bool operator == (const Position &toComparePosition) const
    {
        return x == toComparePosition.x && y == toComparePosition.y;
    }
    
    bool operator != (const Position &toComparePosition) const
    {
        return x != toComparePosition.x || y != toComparePosition.y;
    }
    
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

    friend std::ostream& operator << (std::ostream& os, const Position& pos);
};

std::ostream& operator << (std::ostream& os, const Position& pos)
{
    os << "(" << pos.x << ", " << pos.y << ")";
    return os;
}

namespace std
{
    template <> class hash<Position>
    {
        public:
            int operator()(const Position& position) const
            {
                 return position.y + (position.x * 8);
             }
    };

}


#endif /* Utilities_h */
