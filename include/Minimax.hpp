//
//  Minimax.hpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#ifndef Minimax_hpp
#define Minimax_hpp

#include "Game.hpp"

class Minimax
{
private:
    
    float minimax(Game toAnalyseGame, int iDepth);
    
    // helper function
    // Will return a 'copy' of the game state with the opponent move executed
    Game simulateMove(Position toMovePosition, Game toAnalyseGame);
    
    // Will return all the game states with the posible moves executed for the oponent
    std::vector<Game> getAllPossibleGames(Game toAnalyseGame);
    
public:
    // Constructor / Destructor
    Minimax();
    ~Minimax();
    
    // For testing
    int iNumberOfLeafNodes;
    
    // Will run the possible moves for that player and on each one call minimax to determine which one is the best
    Game getBestGameState(Game toAnalyseGame, int iDepth);
};

#endif /* Minimax_hpp */
