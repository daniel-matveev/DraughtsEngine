//
//  Game.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 09/10/2024.
//

#include "Game.hpp"

// White moves first
void Game::initVariables()
{
    this->currentPlayerColour = White;
}

Game::Game()
{
    this->initVariables();
}

Game::~Game() {}


bool Game::canCurrentlyJump(std::vector<Position> toSkipPositions)
{
    if (toSkipPositions.size() == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}


// Goes through the whole board selecting each piece one by one and getting the valid moves
// Based on whether or not the piece can take it filters the whole set
void Game::selectPieces()
{
    this->selectablePieces.clear();
    
    // Flag to keep track of whether or not any piece can jump
    bool bCanJumpOverall = false;
    
    // Loop through the whole board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // If the piece at that position is the player's colour
            if (this->selectPiece(Position {j+1, i+1}))
            {
                #ifdef DEBUG_FLAG
                    Debug("Checking Piece: {" << j+1 << ", " << i+1 << "}");
                #endif
                
                // Get its valid moves
                this->getValidMoves(false);
                
                // Create iterator to be able to iterate over the map
                std::unordered_map<Position, std::vector<Position> >::iterator filteredEndPositionsToBoardIterator = this->filteredEndPositionsToBoard.begin();
                
                // Filtering of valid pieces
                for (; filteredEndPositionsToBoardIterator != this->filteredEndPositionsToBoard.end(); ++filteredEndPositionsToBoardIterator)
                {
                    
                    bool bCanJumpCurrently = this->canCurrentlyJump(filteredEndPositionsToBoardIterator->second);
                    
                    // If the currently selected piece can jump and we have found a piece that can also jump
                    if ( bCanJumpCurrently && bCanJumpOverall)
                    {
                        this->selectablePieces.insert(this->selectedPiece.getPosition());
                    }
                    // If the currently selected piece can jump but we have not yet found a piece that can also jump
                    else if ( bCanJumpCurrently && !bCanJumpOverall)
                    {
                        // Overall we can jump
                        bCanJumpOverall = true;
                        // All the pieces inserted before this one then must not be able to jump
                        this->selectablePieces.clear();
                        this->selectablePieces.insert(this->selectedPiece.getPosition());
                    }
                    // If the currently selected piece can't jump but we found a piece that can jump
                    else if (!bCanJumpCurrently && bCanJumpOverall)
                    {
                        continue;
                    }
                    // If the currently selected piece can't jump and we have not yet found a piece that can also jump
                    else if (!bCanJumpCurrently && !bCanJumpOverall)
                    {
                        this->selectablePieces.insert(this->selectedPiece.getPosition());
                    }
                    
                }
                
                
                this->intermediatePositionsToBoard.clear();
                this->endPositionsToBoard.clear();
                this->filteredEndPositionsToBoard.clear();
                this->toSkipPositions.clear();
            }
        }
    }
    
    #ifdef DEBUG_FLAG
        Debug("Selectable pieces: " << this->selectablePieces);
    #endif
}

// Gets the valid moves for a selected piece and populates the end and filteredEndPositionsToBoard maps
// bMultiJump - allows to be called mid way through a move if the player decides to take an intermediate move
void Game::getValidMoves(bool bMultiJump)
{
    // If the selected piece is not a king
    if (!this->selectedPiece.getCrowned())
    {
        this->checkPiece(this->selectedPiece.getPosition(), this->gameBoard, bMultiJump);
    }
    // If the selected piece is a king
    else if (this->selectedPiece.getCrowned())
    {
        this->checkKing(this->selectedPiece.getPosition(), this->gameBoard, bMultiJump);
    }
    
    
    // Filtering of valid moves
    // If moves consists of taking pieces we keep those
    bool bCanJumpOverall = false;
    
    std::unordered_map<Position, std::vector<Position>>::iterator endPositionsToBoardIterator = this->endPositionsToBoard.begin();
    
    for (;endPositionsToBoardIterator != this->endPositionsToBoard.end(); ++endPositionsToBoardIterator)
    {
        bool bCanJumpCurrently = this->canCurrentlyJump(endPositionsToBoardIterator->second);;
        if ( bCanJumpCurrently && bCanJumpOverall)
        {
            this->filteredEndPositionsToBoard.insert( std::make_pair( endPositionsToBoardIterator->first, endPositionsToBoardIterator->second) );
        }
        else if ( bCanJumpCurrently && !bCanJumpOverall)
        {
            bCanJumpOverall = true;
            this->filteredEndPositionsToBoard.clear();
            this->filteredEndPositionsToBoard.insert( std::make_pair( endPositionsToBoardIterator->first, endPositionsToBoardIterator->second) );
            
        }
        else if (!bCanJumpCurrently && bCanJumpOverall)
        {
            continue;
        }
        else if (!bCanJumpCurrently && !bCanJumpOverall)
        {
            this->filteredEndPositionsToBoard.insert( std::make_pair( endPositionsToBoardIterator->first, endPositionsToBoardIterator->second) );
            
        }
    }
}

// Checks left and right moves and returns if for the current position (piecePosition) the piece can jump in at least one direction
// If can jump in at least one direction -> return true
// else -> return false
bool Game::checkPiece(Position piecePosition, Board boardState, bool bMultiJump)
{
    // if white check up
    if ( boardState.getPiece(piecePosition).getColour() == White )
    {
        bool bHasJumpedNW = this->checkDirection(piecePosition, boardState, Position {-1, -1}, bMultiJump);
    
        bool bHasJumpedNE = this->checkDirection(piecePosition, boardState, Position {+1, -1}, bMultiJump);

        return bHasJumpedNW || bHasJumpedNE;
    }
    
    // If black check down
    else if ( boardState.getPiece(piecePosition).getColour() == Black )
    {
        bool bHasJumpedSW = this->checkDirection(piecePosition, boardState, Position {-1, +1}, bMultiJump);
        
        bool bHasJumpedSE = this->checkDirection(piecePosition, boardState, Position {+1, +1}, bMultiJump);

        
        return bHasJumpedSW || bHasJumpedSE;
    }
    
    return false;
}

// Checks all 4 directions and reurn if for the current position the king piece can jump in at least one direction
// If can jump in at least one direction -> return true
// else -> return false
bool Game::checkKing(Position piecePosition, Board boardState, bool bMultiJump)
{
    
    bool bHasJumpedNW = this->checkDirection(piecePosition, boardState, Position {-1, -1}, bMultiJump);
    
    bool bHasJumpedNE = this->checkDirection(piecePosition, boardState, Position {+1, -1}, bMultiJump);
    
    bool bHasJumpedSW = this->checkDirection(piecePosition, boardState, Position {-1, +1}, bMultiJump);
    
    bool bHasJumpedSE = this->checkDirection(piecePosition, boardState, Position {+1, +1}, bMultiJump);


    return (bHasJumpedNW || bHasJumpedNE || bHasJumpedSW || bHasJumpedSE);
}

// Checks left side of piece for any jumps
// Returns if it can make one successful jump
// startPosition - the position of the piece moving
// boardState - board permuatation as the piece is jumping
// directionPosition - direction the pieces are moving
//            -> White - -1 (Up)
//            -> Black - +1 (Down)
bool Game::checkDirection(Position startPosition, Board boardState, Position directionPosition, bool bMultiJump)
{
    // To store position of piece to remove in case we can jump
    Position toSkipPosition;
    // temp position for moving and checking if any pieces are ahead
    Position tempPosition = startPosition;
    // Flag to keep track of whether that piece has jumped
    bool bHasJumped = false;
    
    // Looping 2 spaces ahead
    for (int i = 0; i < 2; i++)
    {
        // Update the position we are checking {Horizontal offset, Vertical Offset}
        tempPosition = tempPosition + directionPosition;
        
        #ifdef DEBUG_FLAG
            Debug("Checking position: " << tempPosition);
        #endif
        
        // Edge cases
        if (tempPosition.x > 8 || tempPosition.x < 1)
        {
            bHasJumped = false;
  
            break;
        }
        if (tempPosition.y > 8 || tempPosition.y < 1)
        {
            bHasJumped = false;
            
            break;
        }
        
        
        
        // Space to check
        Piece atPositionPiece = boardState.getPiece(tempPosition);
        
        // If the space to check has a piece of the player's colour
        // Break
        if (atPositionPiece.getColour() == this->currentPlayerColour)
        {
            bHasJumped = false;
            
            break;
        }
        // If the space to check is empty
        else if (atPositionPiece.getColour() == NoColour)
        {
            // And we are jumping
            if (bHasJumped)
            {
                this->toSkipPositions.push_back(toSkipPosition);
                
                #ifdef DEBUG_FLAG
                    Debug("Pieces to skip stack: " << this->toSkipPositions);
                #endif
                
                // Move the piece to the new position
                boardState.movePiece(tempPosition, boardState.getPiece(startPosition));
                
                // Remove the piece we jumped over
                boardState.removePiece(toSkipPosition);
            
                
                bool bCanJump = false;
                // If the piece moving is a king
                if (boardState.getPiece(tempPosition).getCrowned())
                {
                    // check if it can jump again
                    bCanJump = this->checkKing(tempPosition, boardState, true);
                    
                }
                // If the pice moving is a regular piece
                else
                {
                    // Check if it can jump again
                    bCanJump = this->checkPiece(tempPosition, boardState, true);
                }
                
                
                
                // If it cannot = > reached terminal position
                // => Insert to the end positions map
                if (!bCanJump)
                {
                    this->endPositionsToBoard.insert( { std::make_pair( tempPosition, this->toSkipPositions ) } );
                    
                    if (this->toSkipPositions.size() > 0)
                    {
                        this->toSkipPositions.pop_back();
                    }
                }
                // If it can jump => intermediate move
                else
                {
                    this->intermediatePositionsToBoard.insert( std::make_pair(tempPosition, this->toSkipPositions) );
                    
                    if (this->toSkipPositions.size() > 0)
                    {
                        this->toSkipPositions.pop_back();
                    }
                }
                
            }
            
            // And are multi jumping but we haven't jumped
            // Not a valid move
            else if (bMultiJump == true && bHasJumped == false)
            {
                break;
            }
            // Simple move, no jumps
            else if (bMultiJump == false && bHasJumped == false)
            {
                boardState.movePiece(tempPosition, boardState.getPiece(startPosition));
                this->endPositionsToBoard.insert( { std::make_pair( tempPosition, this->toSkipPositions ) } );
                
                break;
            }
        }
        // If it is of the opposite player colour
        else
        {
            // And the piece hasn't jumped yet
            // => it can jump
            if (bHasJumped == false)
            {
                bHasJumped = true;
                toSkipPosition = tempPosition;
            }
            // But if on the second iteration there is another piece of opposite colour
            // => we cannot jump
            else
            {
                bHasJumped = false;
            }
        }
    }
    #ifdef DEBUG_FLAG

        Debug("At position: " << tempPosition);

        Debug("Pieces to skip stack: " << this->toSkipPositions);
    #endif
    
    return bHasJumped;
}

// validates that the piece at that position is of the current player's colour
bool Game::selectPiece(Position toSelectPosition)
{
    // Get the piece at the position
    this->selectedPiece = this->gameBoard.getPiece(toSelectPosition);
    
    // If its colour is the same as the current player's colour
    if (this->selectedPiece.getColour() == this->currentPlayerColour)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// For console
// If the piece the user tries to select is not in the set of selectable pieces returns false
bool Game::select(Position toCheckPosition)
{
    // Create iterator
    std::set<Position>::iterator selectablePiecesIterator = selectablePieces.find(toCheckPosition);
    
    // If the position not found
    if (selectablePiecesIterator == selectablePieces.end())
    {
        return false;
    }
    else
    {
        // Position found
        // Selected piece is the peice at that position
        this->selectedPiece = this->gameBoard.getPiece(toCheckPosition);
        
        // get the valid moves of that piece
        this->getValidMoves(false);
        
        
        return true;
    }
}

// For console
// Returns true if the end position has been inputed and updates the boardState accordingly
bool Game::move(Position toCheckPosition)
{
    std::unordered_map<Position, std::vector<Position> >::iterator filteredEndPositionsToBoardIterator = this->filteredEndPositionsToBoard.begin();
    
    for (; filteredEndPositionsToBoardIterator != this->filteredEndPositionsToBoard.end(); ++filteredEndPositionsToBoardIterator)
    {
        // If a valid end move entered
        if (filteredEndPositionsToBoardIterator->first == toCheckPosition)
        {
            // Make the gameBoard the same as the board associated with that move
            this->gameBoard.movePiece(filteredEndPositionsToBoardIterator->first, this->selectedPiece);
            this->gameBoard.removePieces(filteredEndPositionsToBoardIterator->second);

            // Move ends
            // => swap players
            this->swapPlayers();
            
            this->intermediatePositionsToBoard.clear();
            this->endPositionsToBoard.clear();
            this->filteredEndPositionsToBoard.clear();
            this->toSkipPositions.clear();
            
            return true;
        }
    }
    
    // If it is not in the end positions map, it may be in the intermediate move
    std::unordered_map<Position, std::vector<Position> >::iterator intermediatePositionsToBoardIterator = this->intermediatePositionsToBoard.begin();
    
    for (; intermediatePositionsToBoardIterator != this->intermediatePositionsToBoard.end(); ++intermediatePositionsToBoardIterator)
    {
        // If a valid intermediate move entered
        if (intermediatePositionsToBoardIterator->first == toCheckPosition)
        {
            // Make the gameBoard the same as the board associated with that move
            this->gameBoard.movePiece(intermediatePositionsToBoardIterator->first, this->selectedPiece);
            this->gameBoard.removePieces(intermediatePositionsToBoardIterator->second);
            
            this->intermediatePositionsToBoard.clear();
            this->endPositionsToBoard.clear();
            this->filteredEndPositionsToBoard.clear();
            this->toSkipPositions.clear();
            // Move not ended
            
            // Update the position of the selected piece
            this->selectedPiece = this->gameBoard.getPiece(toCheckPosition);
            
            // Get the new valid end positions, this time as if ti were multi jumping
            this->getValidMoves(true);
            
            return false;
        }
    }
    
    return false;
}

// Swaps players turns
void Game::swapPlayers()
{
    if (this->currentPlayerColour == White)
    {
        this->currentPlayerColour = Black;
    }
    else if (this->currentPlayerColour == Black)
    {
        this->currentPlayerColour = White;
    }
}

// Basic evaluation function
// number of white pieces - number of black pieces
int Game::getEvaluation()
{
    return this->gameBoard.getWhiteLeft() - this->gameBoard.getBlackLeft();
}

float Game::calculateEvaluation()
{
    float fOverallWhiteEvaluation = 0;
    
    float fWhiteBoardFilter [8][8] = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.1, 0.0, 0.1, 0.0, 0.1, 0.0, 0.1, 0.0},
        {0.0, 0.3, 0.0, 0.3, 0.0, 0.3, 0.0, 0.3},
        {0.1, 0.0, 0.3, 0.0, 0.3, 0.0, 0.3, 0.0},
        {0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0},
        {0.0, 0.0, 0.7, 0.0, 0.7, 0.0, 0.7, 0.0},
        {0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0},
        {0.0, 0.0, 1.5, 0.0, 0.0, 0.0, 1.5, 0.0},
    };
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ( this->gameBoard.getPiece(Position {j + 1, i + 1}).getColour() == White )
            {
                fOverallWhiteEvaluation = fOverallWhiteEvaluation + (fWhiteBoardFilter[i][j]);
            }
        }
    }
   
    fOverallWhiteEvaluation =   fOverallWhiteEvaluation +
                                ( (float) this->gameBoard.getWhiteLeft() * 1) +
                                ( (float) this->gameBoard.getWhiteKingsLeft() * 2);
    
    
    float fOverallBlackEvaluation = 0;
    
    float fBlackBoardFilter [8][8] = {
        {0.0, 1.5, 0.0, 0.0, 0.0, 1.5, 0.0, 0.0},
        {0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
        {0.0, 0.3, 0.0, 0.3, 0.0, 0.3, 0.0, 0.0},
        {0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
        {0.0, 0.3, 0.0, 0.5, 0.0, 0.5, 0.0, 0.1},
        {0.3, 0.0, 0.3, 0.0, 0.3, 0.0, 0.3, 0.0},
        {0.0, 0.1, 0.0, 0.1, 0.0, 0.1, 0.0, 0.1},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    };
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ( this->gameBoard.getPiece(Position {j + 1, i + 1}).getColour() == Black )
            {
                fOverallBlackEvaluation = fOverallBlackEvaluation + (fBlackBoardFilter[i][j]);
            }
        }
    }
    
    fOverallBlackEvaluation =   fOverallBlackEvaluation +
                                ( (float) this->gameBoard.getBlackLeft() * 1) +
                                ( (float) this->gameBoard.getBlackKingsLeft() * 2);
    
    return fOverallWhiteEvaluation - fOverallBlackEvaluation;
}

// Checks for board configurations
// And if 3 of the same boards are found a draw is declared
bool Game::checkDraw(std::vector<Board> allBoards)
{
    // Counter for the current board configuration we are searching the stack
    int iBoardCounter;
    
    // For every board configuration (b0), starting at the top of the stack
    for (int i = (int) allBoards.size() - 1; i >= 0; i--)
    {
        iBoardCounter = 1;
        // Check all board configurations between b0 and the first board in the stack
        for (int j = i - 1; i >= 0; i--)
        {
            // If they are both identical
            if (allBoards.at(i) == allBoards.at(j))
            {
                // Increase the counter
                iBoardCounter = iBoardCounter + 1;
                
                // If 3 of the same board configurations are seen return true to signify a draw
                if (iBoardCounter == 3)
                {
                    return true;
                }
            }
        }
    }
    // No draw
    return false;
}


// Checks for winner before allowing a player to make a move
// Checks to see if any pieces can be selected
Colour Game::getWinner()
{
    this->selectPieces();
    
    // If the current player has no more selectable pieces
    if (this->selectablePieces.size() == 0)
    {
        // for white
        if (this->currentPlayerColour == White)
        {
            // Black won
            return Black;
        }
        // for black
        else if (this->currentPlayerColour == Black)
        {
            // white won
            return White;
        }

    }

    return NoColour;
    
}

Board Game::getGameBoard()
{
    return this->gameBoard;
}

// Utility for printing 
Colour Game::getCurrentPlayerColour()
{
    return this->currentPlayerColour;
}

// For printing the board
void Game::printBoard()
{
    this->gameBoard.printBoard();
}
