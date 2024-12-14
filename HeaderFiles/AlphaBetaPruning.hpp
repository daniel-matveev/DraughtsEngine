//
//  AlphaBetaPruning.hpp
//  Draughts_NEA


#ifndef AlphaBetaPruning_hpp
#define AlphaBetaPruning_hpp

#include "Game.hpp"


class AlphaBetaPruning
{
private:
    
    float alphaBetaPruning(Game toAnalyseGame, int iDepth, float fAlpha, float fBeta);
    
    // helper function
    // Will return a 'copy' of the game state with the opponent move executed
    Game simulateMove(Position toMovePosition, Game toAnalyseGame);
    
    // Will return all the game states with the posible moves executed for the oponent
    std::vector<Game> getAllPossibleGames(Game toAnalyseGame);
    
public:
    AlphaBetaPruning();
    ~AlphaBetaPruning();
    
    // For testing
    int iNumberOfLeafNodes;
    
    // Will run the possible moves for that player and on each one call minimax to determine which one is the best
    Game getBestGameState(Game toAnalyseGame, int depth);
};

#endif /* AlphaBetaPruning_hpp */
