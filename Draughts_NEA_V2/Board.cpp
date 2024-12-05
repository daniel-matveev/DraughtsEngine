//
//  Board.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 09/10/2024.
//

#include "Board.hpp"


// Initialises the main variables of the board object
void Board::initVariables()
{
    this->iSize = 8;
    
    this->iBlackLeft = 12;
    this->iWhiteLeft = 12;
    
    this->iBlackKingsLeft = 0;
    this->iWhiteKingsLeft = 0;
}

// Initilises the board array with a Piece object at each index
void Board::initBoard()
{
    for (int i = 0; i < this->iSize; i++)
    {
        for (int j = 0; j < this->iSize; j++)
        {
            board[i][j].setPosition( Position {j, i} + 1 );
            board[i][j].updateCrowned(false);
        }
    }
    
}

// Adds the coresponding colour of the piece
// Based on how an initial game of Draughts looks like
void Board::setUpBoard()
{
    // Controls Row / y
    for (int i = 0; i < this->iSize; i++)
    {
//      Controls Column / x
        for (int j = 0; j < this->iSize; j++)
        {
            // If the row is one of the first 3
            // AND the column number modulus 2 = to the row number + 1 modulus 2
            // Creates the patern: 1, 3, 5, 7
            //                     0, 2, 4, 6
            // Set the colour to these pieces Black
            if ( ( i >= 0 && i <= 2) && ( j % 2 == (i + 1) % 2 ) )
            {
                this->board[i][j].updateColour(Black);
            }
            // Same condition but for the last 3 rows
            // Set the colour to these pieces White
            else if ( ( i >= 5 && i <= 7) && ( j % 2 == (i + 1) % 2 ) )
            {
                this->board[i][j].updateColour(White);
            }
        }
    }
}

// Constructor
// Calls the default constructor of the Piece object
Board::Board()
{
    this->initVariables();
    this->initBoard();
    
    this->setUpBoard();
    
}

Board::~Board() { }

// Function which will move a selected peice to a new location on the board
// Main assumption that the move is a valid move
void Board::movePiece(Position newPosition, Piece selectedPiece)
{
    // If a piece's new position is on the top or bottom row we can assume the piece
    // has made it's way to the other side so we can crown it
    bool bPieceIsCrowned = false;
    if (newPosition.y == 1 || newPosition.y == 8 || selectedPiece.getCrowned())
    {
        bPieceIsCrowned = true;
        
        // But the piece has not been crowned yet
        if (!selectedPiece.getCrowned())
        {
            // Increment the number of kings on the board
            switch (selectedPiece.getColour())
            {
                case White:
                    this->iWhiteKingsLeft = this->iWhiteKingsLeft + 1;
                    break;
                    
                case Black:
                    this->iBlackKingsLeft = this->iBlackKingsLeft + 1;
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // Update the colour of the pieces in the board array
    // At the new position set the colour of the space to the piece selected
    
    // Convert position coordinates into index values
    newPosition = newPosition - 1;
    
    this->board[newPosition.y] [newPosition.x].updateColour(selectedPiece.getColour());
    
    
    // At the old position set the colour of the piece as NoColour and not crowned
    Position selectedPiecePosition = selectedPiece.getPosition();
    
    // Convert position coordinates into index values
    selectedPiecePosition = selectedPiecePosition - 1;
    
    this->board[selectedPiecePosition.y] [selectedPiecePosition.x].updateColour(NoColour);
    this->board[selectedPiecePosition.y] [selectedPiecePosition.x].updateCrowned(false);
    
    // If a piece is corwned update the status of the new position to also be crowned
    if (bPieceIsCrowned)
    {
        this->board[newPosition.y] [newPosition.x].updateCrowned(bPieceIsCrowned);
    }
}

// Takes in a position to remove
void Board::removePiece(Position toRemovePosition)
{
    // gets the piece at that position
    Piece toRemovePiece = this->getPiece(toRemovePosition);
    
    // Determines its colour and decrements the number of pieces left for a colour accordingly
    if (toRemovePiece.getColour() == White)
    {
        this->iWhiteLeft = this->iWhiteLeft - 1;
        if (toRemovePiece.getCrowned())
        {
            this->iWhiteKingsLeft = this->iWhiteKingsLeft - 1;
        }
    }
    else if (toRemovePiece.getColour() == Black)
    {
        this->iBlackLeft = this->iBlackLeft - 1;
        if (toRemovePiece.getCrowned())
        {
            this->iBlackKingsLeft = this->iBlackKingsLeft - 1;
        }
    }
    
    // Sets the position to remove as NoColour
    
    // Convert position coordinates into index values
    toRemovePosition = toRemovePosition - 1;
    this->board [toRemovePosition.y] [toRemovePosition.x].updateColour(NoColour);
    this->board [toRemovePosition.y] [toRemovePosition.x].updateCrowned(false);
}

void Board::removePieces(std::vector<Position> toRemovePositions)
{
    for (int i = 0; i < toRemovePositions.size(); i++)
    {
        this->removePiece(toRemovePositions.at(i));
    }
}

Piece Board::getPiece(Position piecePosition)
{
    // Convert position coordinates into index values
    piecePosition = piecePosition - 1;
    return this->board[piecePosition.y][piecePosition.x];
}

// Returns the number of black pieces left
int Board::getBlackLeft()
{
    return this->iBlackLeft;
}

int Board::getBlackKingsLeft()
{
    return this->iBlackKingsLeft;
}

// Returns the number of white pieces left
int Board::getWhiteLeft()
{
    return this->iWhiteLeft;
}

int Board::getWhiteKingsLeft()
{
    return this->iWhiteKingsLeft;
}


// Print the board
void Board::printBoard()
{
    char cPieceSymbol;
    
    
    std::cout << "   | 1 || 2 || 3 || 4 || 5 || 6 || 7 || 8 |" << std::endl;
    std::cout << "---+---++---++---++---++---++---++---++---+" << std::endl;
    for (int i = 0; i < this->iSize; i++)
    {
        std::cout << ' ' << i+1 << ' ';
        for (int j = 0; j < this->iSize; j++)
        {
            cPieceSymbol = ' ';
            
            Colour pieceColour = this->getPiece({j+1, i+1}).getColour();
            if (pieceColour == White)
            {
                cPieceSymbol = 'o';
            }
            else if (pieceColour == Black)
            {
                cPieceSymbol = 'x';
            }
            if (this->getPiece({j+1, i+1}).getCrowned())
            {
                cPieceSymbol = cPieceSymbol - 32;
            }
            std::cout << "| " << cPieceSymbol << " |";
        }
        std::cout << std::endl;
        std::cout << "---+---++---++---++---++---++---++---++---+" << std::endl;
    }
}

