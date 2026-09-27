//
//  Minimax.cpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#include "Minimax.hpp"

// Core minimax algorithm
float Minimax::minimax(Game toAnalyseGame, int iDepth, long long& iLeafNodes)
{
    // Base case scenario
    // If we have hit the desired depth or There is a winner
    if (iDepth == 0 || toAnalyseGame.getWinner() != NoColour)
    {
        #ifdef DEBUG_FLAG_MINIMAX
            Debug("Depth hit");
            Debug("Board evaluation: " << toAnalyseGame.calculateEvaluation() << "\n");
        #endif
        iLeafNodes++;
        return toAnalyseGame.calculateEvaluation();
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
        float fMaxEvaluation = -INFINITY;
        
        // Get all the possible game states for white
        std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
        
        for (int i = 0 ; i < toAnalyseGames.size(); i++)
        {
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
                toAnalyseGames.at(i).printBoard();
                Debug("Current Maximum Evaluation: " << fMaxEvaluation << "\n");
            #endif
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            float fEvaluation = this->minimax(toAnalyseGames.at(i), iDepth - 1, iLeafNodes);
            
            // Compare it to the best evaluation for white and update
            fMaxEvaluation = std::max(fMaxEvaluation, fEvaluation);
            
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Move analysed: " << i + 1 << "/" << toAnalyseGames.size() );
                Debug("Current Maximum Evaluation: " << fMaxEvaluation << "\n");
            #endif
        }
        
        toAnalyseGames.clear();
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
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
                toAnalyseGames.at(i).printBoard();
                Debug("Current Minimum Evaluation: " << fMinEvaluation << "\n");

            #endif
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            float fEvaluation = this->minimax(toAnalyseGames.at(i), iDepth - 1, iLeafNodes);
            
            // Compare it to the best evaluation for black and update
            fMinEvaluation = std::min(fMinEvaluation, fEvaluation);
                        
            #ifdef DEBUG_FLAG_MINIMAX
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Move analysed : " << i + 1 << "/" << toAnalyseGames.size() );
                Debug("Current Minimum Evaluation: " << fMinEvaluation << "\n");
            #endif
        }
        
        toAnalyseGames.clear();
        // return the best evaluation for black
        return fMinEvaluation;
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
    
    // for each selectable piece
    for (std::set<Position>::iterator selectablePiecesIterator =
            toAnalyseGame.selectablePieces.begin();
         selectablePiecesIterator != toAnalyseGame.selectablePieces.end();
         ++selectablePiecesIterator)
    {
        // Select it
        toAnalyseGame.select(Position {selectablePiecesIterator->x,
                                       selectablePiecesIterator->y});
        
        // Get its valid moves
        toAnalyseGame.getValidMoves(false);
        
        // For each valid move
        for (std::unordered_map<Position, std::vector<Position> >::iterator
                filteredEndPositionsToBoardIterator
                = toAnalyseGame.filteredEndPositionsToBoard.begin();
             filteredEndPositionsToBoardIterator != toAnalyseGame.filteredEndPositionsToBoard.end();
             ++filteredEndPositionsToBoardIterator)
        {
            // simulate that move
            Game tempGame = this->simulateMove(filteredEndPositionsToBoardIterator->first,
                                               toAnalyseGame);
            
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

Minimax::~Minimax() { }

Game Minimax::getBestGameState(Game toAnalyseGame, int iDepth)
{
    Colour playerColour = toAnalyseGame.getCurrentPlayerColour();
    
    long long iTotalLeafNodes = 0;
    
    // Get all the possible game states for black
    std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
    
    std::vector<float> scores(toAnalyseGames.size());

    #pragma omp parallel for schedule(dynamic) reduction(+:iTotalLeafNodes)
    for (int i = 0; i < toAnalyseGames.size(); i++)
    {
        long long iLocalLeafNodes = 0;
        // Recursevely go through each one
        // Alternatting moves
        // Once a final position is reached the evaluation of that position is returned
        scores[i] = this->minimax(toAnalyseGames.at(i), iDepth, iLocalLeafNodes);

        iTotalLeafNodes += iLocalLeafNodes;
    }

    this->iNumberOfLeafNodes = iTotalLeafNodes;

    #ifdef DEBUG_FLAG_MINIMAX
        Debug("Outer loop finished");
        Debug("Move scores: ");
        for (int i = 0; i < scores.size(); i++)
        {
            Debug("Move " << i + 1 << ": " << scores[i]);
        }
    #endif

    #ifdef DEBUG_FLAG_TIME
        Debug("Number of different games analysed: " << this->iNumberOfLeafNodes);
    #endif

    // Find the index of the best score
    int iBestBoardStateIndex = 0;
    float fBestScore = scores[0];

    for (int i = 1; i < scores.size(); i++)
    {
        if (playerColour == White && scores[i] > fBestScore)
        {
            fBestScore = scores[i];
            iBestBoardStateIndex = i;
        }
        else if (playerColour == Black && scores[i] < fBestScore)
        {
            fBestScore = scores[i];
            iBestBoardStateIndex = i;
        }
    }
    
    return toAnalyseGames.at(iBestBoardStateIndex);
}
