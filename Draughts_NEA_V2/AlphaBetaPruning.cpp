//
//  AlphaBetaPruning.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 19/10/2024.
//

#include "AlphaBetaPruning.hpp"

// Core minimax with alpha beta pruning optimisation algorithm
float AlphaBetaPruning::alphaBetaPruning(Game toAnalyseGame, int iDepth, float fAlpha, float fBeta)
{
    // Base case scenario
    // If we have hit the desired depth or There is a winner
    if (iDepth == 0 || toAnalyseGame.getWinner() != NoColour)
    {
        this->iNumberOfLeafNodes++;
        return toAnalyseGame.calculateEvaluation();
    }
    
    // Get the current player's colour
    Colour currentPlayerColour = toAnalyseGame.getCurrentPlayerColour();
    
    // White is the maximising player
    if (currentPlayerColour == White)
    {
        // The best evaluation for white is -infinity until proven otherwise
        // => assign the smallest value we can
        float fMaxEvaluation = -INFINITY;
        
        // Get all the possible game states for white
        std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
        
        for (int i = 0 ; i < toAnalyseGames.size(); i++)
        {
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            float fEvaluation = this->alphaBetaPruning(toAnalyseGames.at(i), iDepth - 1, fAlpha, fBeta);
            
            // Compare it to the best evaluation for white and update
            fMaxEvaluation = std::max(fMaxEvaluation, fEvaluation);
            
            fAlpha = std::max(fAlpha, fEvaluation);
            
            if (fBeta <= fAlpha)
            {
                break;
            }
        }
        // return the best evaluation for white
        return fMaxEvaluation;
    }
    // Black is the minimising player
    else
    {
        // The best evaluation for white is +infinity until proven otherwise
        // => assign the largest value we can
        float fMinEvaluation = INFINITY;
        
        // Get all the possible game states for black
        std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
        
        for (int i = 0 ; i < toAnalyseGames.size(); i++)
        {
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            float fEvaluation = this->alphaBetaPruning(toAnalyseGames.at(i), iDepth - 1, fAlpha, fBeta);
            
            // Compare it to the best evaluation for black and update
            fMinEvaluation = std::min(fMinEvaluation, fEvaluation);
            
            
            
            fBeta = std::min(fBeta, fEvaluation);
            
            if (fBeta <= fAlpha)
            {
                break;
            }
        }
        // return the best evaluation for black
        return fMinEvaluation;
    }
}

// to simulate a move and return a board permuation
Game AlphaBetaPruning::simulateMove(Position toMovePosition, Game toAnalyseGame)
{
    toAnalyseGame.move(toMovePosition);
    
    return toAnalyseGame;
}

std::vector<Game> AlphaBetaPruning::getAllPossibleGames(Game toAnalyseGame)
{
    std::vector<Game> toAnalyseGames;
    
//    this->currentGameState = toAnalyseGame;
    
    std::set<Position>::iterator selectablePiecesIterator = this->currentGameState.selectablePieces.begin();
    
    // for each selectable piece
    for (; selectablePiecesIterator != toAnalyseGame.selectablePieces.end(); ++selectablePiecesIterator)
    {
        // Select it
        toAnalyseGame.select(Position {selectablePiecesIterator->x, selectablePiecesIterator->y});
        
        // Get its valid moves
        toAnalyseGame.getValidMoves(false);
        
        std::unordered_map<Position, std::vector<Position> >::iterator filteredEndPositionsToBoardIterator = toAnalyseGame.filteredEndPositionsToBoard.begin();
        
        // For each valid move
        for (; filteredEndPositionsToBoardIterator != toAnalyseGame.filteredEndPositionsToBoard.end(); ++filteredEndPositionsToBoardIterator)
        {
            // simulate that move
            Game tempGame = this->simulateMove(filteredEndPositionsToBoardIterator->first, toAnalyseGame);
            
            // add the board permuation to toAnalyseGames
            toAnalyseGames.push_back(tempGame);
        }
        
        toAnalyseGame.endPositionsToBoard.clear();
        toAnalyseGame.filteredEndPositionsToBoard.clear();
        toAnalyseGame.intermediatePositionsToBoard.clear();
        toAnalyseGame.toSkipPositions.clear();
    }
    
    return toAnalyseGames;
}


AlphaBetaPruning::AlphaBetaPruning()
{
    this->iNumberOfLeafNodes = 0;
}

AlphaBetaPruning::~AlphaBetaPruning()
{
    
}

Game AlphaBetaPruning::getBestGameState(Game toAnalyseGame, int iDepth)
{
    // to keep track of the next best board state found
    int iBestBoardStateIndex = -1;
    
    float fMinEvaluation = INFINITY;
    
    float fMaxEvaluation = -INFINITY;
    
    Colour playerColour = toAnalyseGame.getCurrentPlayerColour();
    
    // Get all the possible game states for black
    std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
    
    for (int i = 0; i < toAnalyseGames.size(); i++)
    {
        // Recursevely go through each one
        // Alternatting moves
        // Once a final position is reached the evaluation of that position is returned
        float fEvaluation = this->alphaBetaPruning(toAnalyseGames.at(i), iDepth, -INFINITY, INFINITY);
        
        // Compare it to the best evaluation for black and update
        if (playerColour == Black && fEvaluation < fMinEvaluation)
        {
            fMinEvaluation = fEvaluation;
            iBestBoardStateIndex = i;
        }
        
        else if (playerColour == White && fEvaluation > fMaxEvaluation)
        {
            fMaxEvaluation = fEvaluation;
            iBestBoardStateIndex = i;
        }
        
    }
    
    return toAnalyseGames.at(iBestBoardStateIndex);
}
