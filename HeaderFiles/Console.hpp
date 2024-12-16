//
//  Console.hpp
//  Draughts_NEA
//
//  Created by Daniel Matveev 
//

#ifndef Console_hpp
#define Console_hpp

#include "Game.hpp"

#include "Minimax.hpp"
#include "AlphaBetaPruning.hpp"
#include "MonteCarloTreeSearch.hpp"

// Struct for player
struct Player
{
    // To hold the algorithm used by the player, or if human
    std::string szTypeOfPlayer;
    
    // If an algorithm used to have a difficulty
    int iDifficulty;
    
    // Default constructor
    Player();
};

class Console
{
private:
    // Main game state
    Game mainGame;
    
    // Instance for each algorithm to be called if needed
    Minimax minimax;
    AlphaBetaPruning alphaBetaPruning;
    MonteCarloTreeSearch monteCarloTreeSearch;
    
    // 2 players
    Player player1;
    Player player2;
    
    char extractInvalidInput();
    void handleInvalidInput(char cExtracted);
    
    // Printing messages
    void printWelcomeMessage();
    
    void printGoodbyeMessage();
    
    void reset();
    
public:
    // Constructor / Destructor
    Console();
    ~Console();
    
    // Print menu options
    void printStartOptions();
    
    void printGameRules();
    
    void printPlayerSelection();
    
    void printDifficultyLevels();
    
    // Player selection
    void selectPlayer(int iPlayer, int iDifficulty);
    
    // To play a game based on the player selections
    void startGame();
    
};

#endif /* Console_hpp */
