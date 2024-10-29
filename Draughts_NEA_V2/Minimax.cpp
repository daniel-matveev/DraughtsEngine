//
//  Minimax.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 19/10/2024.
//

#include "Minimax.hpp"

// Core minimax algorithm
int Minimax::minimax(Game toAnalyseGame, int iDepth)
{
    // Base case scenario
    // If we have hit the desired depth or There is a winner
    if (iDepth == 0 || toAnalyseGame.getWinner() != NoColour)
    {
        this->iNumberOfLeafNodes++;
        return toAnalyseGame.getEvaluation();
    }
    
    // Get the current player's colour
    Colour currentPlayerColour = toAnalyseGame.getCurrentPlayerColour();
    
    // White is the maximising player
    if (currentPlayerColour == White)
    {
        // The best evaluation for white is -infinity until proven otherwise
        // => assign the smallest value we can
        int iMaxEvaluation = INT_MIN;
        
        // Get all the possible game states for white
        std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
        
        for (int i = 0 ; i < toAnalyseGames.size(); i++)
        {
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            int iEvaluation = this->minimax(toAnalyseGames.at(i), iDepth - 1);
            
            // Compare it to the best evaluation for white and update
            iMaxEvaluation = std::max(iMaxEvaluation, iEvaluation);
        }
        // return the best evaluation for white
        return iMaxEvaluation;
    }
    // Black is the minimising player
    else
    {
        // The best evaluation for white is +infinity until proven otherwise
        // => assign the largest value we can
        int iMinEvaluation = INT_MAX;
        
        // Get all the possible game states for black
        std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
        
        for (int i = 0 ; i < toAnalyseGames.size(); i++)
        {
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            int iEvaluation = this->minimax(toAnalyseGames.at(i), iDepth - 1);
            
            // Compare it to the best evaluation for black and update
            iMinEvaluation = std::min(iMinEvaluation, iEvaluation);
        }
        
        // return the best evaluation for black
        return iMinEvaluation;
    }
}

// to simulate a move and return a board permuation
Game Minimax::simulateMove(Position toMovePosition, Game toAnalyseGame)
{
    toAnalyseGame.move(toMovePosition);
    
    return toAnalyseGame;
}

std::vector<Game> Minimax::getAllPossibleGames(Game toAnalyseGame)
{
    std::vector<Game> toAnalyseGames;
    
    this->currentGameState = toAnalyseGame;
    
    std::set<Position>::iterator selectablePiecesIterator = this->currentGameState.selectablePieces.begin();
    
    // for each selectable piece
    for (; selectablePiecesIterator != this->currentGameState.selectablePieces.end(); ++selectablePiecesIterator)
    {
        // Select it
        this->currentGameState.select(Position {selectablePiecesIterator->x, selectablePiecesIterator->y});
        
        // Get its valid moves
        this->currentGameState.getValidMoves(false);
        
        std::unordered_map<Position, Board>::iterator filteredEndPositionsToBoardIterator = this->currentGameState.filteredEndPositionsToBoard.begin();
        
        // For each valid move
        for (; filteredEndPositionsToBoardIterator != this->currentGameState.filteredEndPositionsToBoard.end(); ++filteredEndPositionsToBoardIterator)
        {
            // simulate that move
            Game tempGame = this->simulateMove(filteredEndPositionsToBoardIterator->first, this->currentGameState);
            
            // add the board permuation to toAnalyseGames
            toAnalyseGames.push_back(tempGame);
        }
        
        this->currentGameState.endPositionsToBoard.clear();
        this->currentGameState.filteredEndPositionsToBoard.clear();
        this->currentGameState.intermediatePositionsToBoard.clear();
    }
    
    return toAnalyseGames;
}


Minimax::Minimax()
{
    this->iNumberOfLeafNodes = 0;
}

Minimax::~Minimax()
{
    
}

Game Minimax::getBestGameState(Game toAnalyseGame, int iDepth)
{
    // to keep track of the next best board state found
    int iBestBoardStateIndex = -1;
    
    int iMinEvaluation = INT_MAX;
    
    int iMaxEvaluation = INT_MIN;
    
    Colour playerColour = toAnalyseGame.getCurrentPlayerColour();
    
    // Get all the possible game states for black
    std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
    
    for (int i = 0; i < toAnalyseGames.size(); i++)
    {
        // Recursevely go through each one
        // Alternatting moves
        // Once a final position is reached the evaluation of that position is returned
        int iEvaluation = this->minimax(toAnalyseGames.at(i), iDepth);
        
        // Compare it to the best evaluation for black and update
        if (playerColour == Black && iEvaluation < iMinEvaluation)
        {
            iMinEvaluation = iEvaluation;
            iBestBoardStateIndex = i;
        }
        else if (playerColour == White && iEvaluation > iMaxEvaluation)
        {
            iMaxEvaluation = iEvaluation;
            iBestBoardStateIndex = i;
        }
    }
    
    return toAnalyseGames.at(iBestBoardStateIndex);
}
