//
//  Minimax.cpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#include "Minimax.hpp"

// Core minimax algorithm
int Minimax::minimax(Game toAnalyseGame, int iDepth)
{
    // Base case scenario
    // If we have hit the desired depth or There is a winner
    if (iDepth == 0 || toAnalyseGame.getWinner() != NoColour)
    {
        #ifdef DEBUG_FLAG_MINIMAX
            Debug("Depth hit");
            Debug("Board evaluation: " << toAnalyseGame.getEvaluation() << "\n");
        #endif
        this->iNumberOfLeafNodes++;
        return toAnalyseGame.getEvaluation();
    }
    
    // Get the current player's colour
    Colour currentPlayerColour = toAnalyseGame.getCurrentPlayerColour();
    
    #ifdef DEBUG_FLAG_MINIMAX
        Debug("Checking moves for: " << currentPlayerColour);
    #endif
    
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
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
                toAnalyseGames.at(i).printBoard();
                Debug("Current Maximum Evaluation: " << iMaxEvaluation << "\n");
            #endif
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            int iEvaluation = this->minimax(toAnalyseGames.at(i), iDepth - 1);
            
            // Compare it to the best evaluation for white and update
            iMaxEvaluation = std::max(iMaxEvaluation, iEvaluation);
            
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Move analysed: " << i + 1 << "/" << toAnalyseGames.size() );
                Debug("Current Maximum Evaluation: " << iMaxEvaluation << "\n");
            #endif
        }
        
        toAnalyseGames.clear();
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
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
                toAnalyseGames.at(i).printBoard();
                Debug("Current Minimum Evaluation: " << iMinEvaluation << "\n");

            #endif
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            int iEvaluation = this->minimax(toAnalyseGames.at(i), iDepth - 1);
            
            // Compare it to the best evaluation for black and update
            iMinEvaluation = std::min(iMinEvaluation, iEvaluation);
                        
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Move analysed : " << i + 1 << "/" << toAnalyseGames.size() );
                Debug("Current Minimum Evaluation: " << iMinEvaluation << "\n");
            #endif
        }
        
        toAnalyseGames.clear();
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
    
//    this->currentGameState = toAnalyseGame;
    
    std::set<Position>::iterator selectablePiecesIterator = toAnalyseGame.selectablePieces.begin();
    
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
        
        toAnalyseGame.clear();
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
    
    this->iNumberOfLeafNodes = 0;
    
    // Get all the possible game states for black
    std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
    
    for (int i = 0; i < toAnalyseGames.size(); i++)
    {
        #ifdef DEBUG_FLAG_MINIMAX
            Debug("Current Node ID: " << iDepth + 1 << "." << playerColour << "." << i + 1);
            Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
            toAnalyseGames.at(i).printBoard();
            if (toAnalyseGame.currentPlayerColour == White)
            {
                Debug("Current Maximum Evaluation: " << iMaxEvaluation << "\n");
            }
            else
            {
                Debug("Current Minimum Evaluation: " << iMinEvaluation << "\n");
            }
            
        #endif
        // Recursevely go through each one
        // Alternatting moves
        // Once a final position is reached the evaluation of that position is returned
        int iEvaluation = this->minimax(toAnalyseGames.at(i), iDepth);
        
        // Compare it to the best evaluation for black and update
        if (playerColour == Black && iEvaluation <= iMinEvaluation)
        {
            iMinEvaluation = iEvaluation;
            iBestBoardStateIndex = i;
        }
        else if (playerColour == White && iEvaluation >= iMaxEvaluation)
        {
            iMaxEvaluation = iEvaluation;
            iBestBoardStateIndex = i;
        }
        #ifdef DEBUG_FLAG_MINIMAX
            Debug("Current Node ID: " << iDepth + 1 << "." << playerColour << "." << i + 1);
            Debug("Move analysed: " << i + 1 << "/" << toAnalyseGames.size() );
            if (toAnalyseGame.currentPlayerColour == White)
            {
                Debug("Current Maximum Evaluation: " << iMaxEvaluation << "\n");
            }
            else
            {
                Debug("Current Minimum Evaluation: " << iMinEvaluation << "\n");
            }
        #endif
    }
    
    #ifdef DEBUG_FLAG_TIME
        Debug("Number of different games analysed: " << this->iNumberOfLeafNodes);
    #endif
    
    return toAnalyseGames.at(iBestBoardStateIndex);
}
