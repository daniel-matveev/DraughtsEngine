//
//  Piece.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 09/10/2024.
//

#include "Piece.hpp"

void Piece::initVariables(Position piecePosition, bool bIsCrowned, Colour pieceColour)
{
    this->piecePosition = piecePosition;
    this->bIsCrowned = bIsCrowned;
    this->pieceColour = pieceColour;
}

// Default constructor

Piece::Piece()
{
    // Variables initialised to dummy data
    // The Board object must use the default constructor
    this->piecePosition = {0, 0};
    this->bIsCrowned = false;
    this->pieceColour = NoColour;
}

Piece::~Piece() {}

// Update the position of the piece
void Piece::setPosition(Position piecePosition)
{
    this->piecePosition = piecePosition;
}

// Update status of the piece
void Piece::updateCrowned(bool bIsCrowned)
{
    this->bIsCrowned = bIsCrowned;
}

// Updates colour of the piece
void Piece::updateColour(Colour pieceColour)
{
    this->pieceColour = pieceColour;
}

// Returns colour of the piece
Colour Piece::getColour()
{
    return this->pieceColour;
}

// Returns crowned status
bool Piece::getCrowned()
{
    return this->bIsCrowned;
}


Position Piece::getPosition()
{
    return this->piecePosition;
}
