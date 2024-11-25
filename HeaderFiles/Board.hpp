//
//  Board.hpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 09/10/2024.
//

#ifndef Board_hpp
#define Board_hpp

#include "Piece.hpp"

class Board
{
private:
    // 8x8 array of Piece objects
    Piece board [8][8] ;
    // size for a standard draughts board (8)
    int iSize;
    
    // Keep track of the pieces left on the board
    int iBlackLeft;
    int iBlackKingsLeft;
    int iWhiteLeft;
    int iWhiteKingsLeft;
    
    // Initialisers
    void initVariables();
    void initBoard();
    
    void setUpBoard();
    
    
public:
    // Constructor / Destructor
    Board();
    ~Board();
    
    // Setters
    void movePiece(Position newPosition, Piece selectedPiece);
    
    void removePiece(Position toRemovePosition);
    void removePieces(std::vector<Position> toRemovePositions);
    
    // Getters
    Piece getPiece(Position piecePosition);

    int getBlackLeft();
    int getBlackKingsLeft();
    int getWhiteLeft();
    int getWhiteKingsLeft();
    
    
    // Utilities
    void printBoard();
    
    // Overloading == operator
    // If 2 boards don't have the same pieces at the same locations they are not the same
    bool operator == (const Board &toCompareBoard) const
    {
        // Go though all the board positions
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // If the pieces differ at any position
                if ( !( toCompareBoard.board[i][j] == this->board[i][j] ) )
                {
                    // Boards not the same
                    return false;
                }
            }
        }
        
        return true;
    }
};

#endif /* Board_hpp */
