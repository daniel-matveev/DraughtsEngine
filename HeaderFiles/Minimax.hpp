//
//  Minimax.hpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 19/10/2024.
//

#ifndef Minimax_hpp
#define Minimax_hpp

#include "Game.hpp"

class Minimax
{
private:
    
    int minimax(Game toAnalyseGame, int iDepth);
    
    // helper function
    // Will return a 'copy' of the game state with the opponent move executed
    Game simulateMove(Position toMovePosition, Game toAnalyseGame);
    
    // Will return all the game states with the posible moves executed for the oponent
    std::vector<Game> getAllPossibleGames(Game toAnalyseGame);
    
public:
    Minimax();
    ~Minimax();
    
    // For testing
    int iNumberOfLeafNodes;
    
    // Will run the possible moves for that player and on each one call minimax to determine which one is the best
    Game getBestGameState(Game toAnalyseGame, int iDepth);
};

#endif /* Minimax_hpp */
