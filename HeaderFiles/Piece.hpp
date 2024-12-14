//
//  Piece.hpp
//  Draughts_NEA

#ifndef Piece_hpp
#define Piece_hpp

#include "Utilities.hpp"

class Piece
{
private:
    // Position of piece (in the board array)
    Position piecePosition;
    
    // Flag for when the piece gets to the other side of the board (enables moves backwards)
    bool bIsCrowned;
    
    // Piece colour
    Colour pieceColour;
    
    void initVariables(Position piecePosition, bool bIsCrowned, Colour pieceColour);
    
public:
    // Default Constructor
    Piece();
    ~Piece();
    
    // Setters
    void setPosition(Position piecePosition);
    
    // Update procedures
    void updateCrowned(bool bIsCrowned);
    void updateColour(Colour pieceColour);
    
    // Getters
    Colour getColour();
    bool getCrowned();

    Position getPosition();
    
    // Overloading == operator
    // Two pieces are the same if they have the same crowned status, same colour and same position
    bool operator == (const Piece &toComparePiece) const
    {
        bool bSameCrownStatus = toComparePiece.bIsCrowned == this->bIsCrowned;
        bool bSamePieceColour = toComparePiece.pieceColour == this->pieceColour;
        bool bSamePiecePosition = toComparePiece.piecePosition == this->piecePosition;
        
        return bSameCrownStatus && bSamePieceColour && bSamePiecePosition;
    }
};

#endif /* Piece_hpp */
