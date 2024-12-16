//
//  AlphaBetaPruning.cpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#include "AlphaBetaPruning.hpp"

// Core minimax with alpha beta pruning optimisation algorithm
float AlphaBetaPruning::alphaBetaPruning(Game toAnalyseGame, int iDepth, float fAlpha, float fBeta)
{
    // Base case scenario
    // If we have hit the desired depth or There is a winner
    if (iDepth == 0 || toAnalyseGame.getWinner() != NoColour)
    {
        #ifdef DEBUG_FLAG_ALPHABETAPRUNING
            Debug("Depth hit");
            Debug("Board evaluation: " << toAnalyseGame.calculateEvaluation() << "\n");
        #endif
        this->iNumberOfLeafNodes++;
        return toAnalyseGame.calculateEvaluation();
    }
    
    // Get the current player's colour
    Colour currentPlayerColour = toAnalyseGame.getCurrentPlayerColour();
    
    #ifdef DEBUG_FLAG_ALPHABETAPRUNING
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
            #ifdef DEBUG_FLAG_ALPHABETAPRUNING
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
                toAnalyseGames.at(i).printBoard();
                Debug("Current Maximum Evaluation: " << fMaxEvaluation);
                Debug("Current Alpha: " << fAlpha);
                Debug("Current Beta: " << fBeta << "\n");
            #endif
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            float fEvaluation = this->alphaBetaPruning(toAnalyseGames.at(i),
                                                       iDepth - 1,
                                                       fAlpha,
                                                       fBeta);
            
            // Compare it to the best evaluation for white and update
            fMaxEvaluation = std::max(fMaxEvaluation, fEvaluation);
            
            fAlpha = std::max(fAlpha, fEvaluation);
            
            #ifdef DEBUG_FLAG_ALPHABETAPRUNING
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Move analysed: " << i + 1 << "/" << toAnalyseGames.size() );
                Debug("Current Maximum Evaluation: " << fMaxEvaluation);
                Debug("Current Alpha: " << fAlpha);
                Debug("Current Beta: " << fBeta << "\n");
            #endif
            
            if (fBeta <= fAlpha)
            {
                #ifdef DEBUG_FLAG_ALPHABETAPRUNING
                    Debug("Pruning\n");
                #endif
                break;
            }
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
            #ifdef DEBUG_FLAG_ALPHABETAPRUNING
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
                toAnalyseGames.at(i).printBoard();
                Debug("Current Minimum Evaluation: " << fMinEvaluation);
                Debug("Current Alpha: " << fAlpha);
                Debug("Current Beta: " << fBeta << "\n");
            #endif
            // Recursevely go through each one
            // Alternatting moves
            // Once a final position is reached the evaluation of that position is returned
            float fEvaluation = this->alphaBetaPruning(toAnalyseGames.at(i),
                                                       iDepth - 1,
                                                       fAlpha,
                                                       fBeta);
            
            // Compare it to the best evaluation for black and update
            fMinEvaluation = std::min(fMinEvaluation, fEvaluation);
            
            fBeta = std::min(fBeta, fEvaluation);
            
            #ifdef DEBUG_FLAG_ALPHABETAPRUNING
                Debug("Current Node ID: " << iDepth << "." << currentPlayerColour << "." << i + 1);
                Debug("Move analysed : " << i + 1 << "/" << toAnalyseGames.size() );
                Debug("Current Minimum Evaluation: " << fMinEvaluation);
                Debug("Current Alpha: " << fAlpha);
                Debug("Current Beta: " << fBeta << "\n");
            #endif
            
            if (fBeta <= fAlpha)
            {
                #ifdef DEBUG_FLAG_ALPHABETAPRUNING
                    Debug("Pruning\n");
                #endif
                break;
            }
        }
        
        toAnalyseGames.clear();
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
    
    
    
    // for each selectable piece
    for (std::set<Position>::iterator selectablePiecesIterator
            = toAnalyseGame.selectablePieces.begin();
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
                filteredEndPositionsToBoardIterator = toAnalyseGame.filteredEndPositionsToBoard.begin();
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
    
    float fAlpha = -INFINITY;
    float fBeta = INFINITY;
    
    Colour playerColour = toAnalyseGame.getCurrentPlayerColour();
    
    // Get all the possible game states for black
    std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
    
    for (int i = 0; i < toAnalyseGames.size(); i++)
    {
        #ifdef DEBUG_FLAG_ALPHABETAPRUNING
            Debug("Current Node ID: " << iDepth + 1 << "." << playerColour << "." << i + 1);
            Debug("Analysing move: " << i + 1 << "/" << toAnalyseGames.size() );
            toAnalyseGames.at(i).printBoard();
            if (toAnalyseGame.currentPlayerColour == White)
            {
                Debug("Current Maximum Evaluation: " << fMaxEvaluation);
            }
            else
            {
                Debug("Current Minimum Evaluation: " << fMinEvaluation);
            }
            Debug("Current Alpha: " << fAlpha);
            Debug("Current Beta: " << fBeta << "\n");
        #endif
        // Recursevely go through each one
        // Alternatting moves
        // Once a final position is reached the evaluation of that position is returned
        float fEvaluation = this->alphaBetaPruning(toAnalyseGames.at(i),
                                                   iDepth, fAlpha,
                                                   fBeta);
        
        // Compare it to the best evaluation for black and update
        if (playerColour == Black && fEvaluation < fMinEvaluation)
        {
            fMinEvaluation = fEvaluation;
            fBeta = std::min(fBeta, fEvaluation);
            iBestBoardStateIndex = i;
        }
        
        else if (playerColour == White && fEvaluation > fMaxEvaluation)
        {
            fMaxEvaluation = fEvaluation;
            fAlpha = std::max(fAlpha, fEvaluation);
            iBestBoardStateIndex = i;
        }
        
        #ifdef DEBUG_FLAG_ALPHABETAPRUNING
            Debug("Current Node ID: " << iDepth + 1 << "." << playerColour << "." << i + 1);
            Debug("Move analysed: " << i + 1 << "/" << toAnalyseGames.size() );
            if (toAnalyseGame.currentPlayerColour == White)
            {
                Debug("Current Maximum Evaluation: " << fMaxEvaluation);
            }
            else
            {
                Debug("Current Minimum Evaluation: " << fMinEvaluation);
            }
            Debug("Current Alpha: " << fAlpha);
            Debug("Current Beta: " << fBeta << "\n");
        #endif
        
        
        
        if (fBeta <= fAlpha)
        {
            #ifdef DEBUG_FLAG_ALPHABETAPRUNING
                Debug("Pruning\n");
            #endif
            break;
        }
        
    }
    
    #ifdef DEBUG_FLAG_TIME
        Debug("Number of different games analysed: " << this->iNumberOfLeafNodes);
    #endif

    this->iNumberOfLeafNodes = 0;
    
    Game bestGameState = toAnalyseGames.at(iBestBoardStateIndex);
    
    toAnalyseGames.clear();
    
    return bestGameState;
}
