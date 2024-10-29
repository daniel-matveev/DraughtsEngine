//
//  Console.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 26/10/2024.
//

#include "Console.hpp"

// Player struct default constructor
Player::Player()
{
    // To determine if a player has been initialised yet or not
    this->szTypeOfPlayer = "None";
    
    this->iDifficulty = 0;
}

// To print welcome message
void Console::printWelcomeMessage()
{
    std::cout << "Welcome" << std::endl;
}

// To print goodbye message
void Console::printGoodbyeMessage()
{
    std::cout << "Goodbye" << std::endl;
}

// Main menu options
void Console::printStartOptions()
{
    std::string szText;
    
    szText =
    
            "1 - Start Game\n"
            "2 - Game Rules\n"
            "3 - Quit\n"
            "Option: ";
    
    std::cout << szText;
}

// To print the game rules
void Console::printGameRules()
{
    std::string szText;
    
    szText =
            "Game Rules:";
    
    std::cout << szText << std::endl;
}

// Player selection menu
void Console::printPlayerSelection()
{
    std::string szText;
    
    szText =
            "1 - Minimax\n"
            "2 - Alpha Beta Pruning\n"
            "3 - Monte Carlo Tree Search\n"
            "4 - Human\n"
            "Option: ";
    std::cout << szText;
}

// Player difficulty menu
void Console::printDifficultyLevels()
{
    std::string szText;
    
    szText  =
            "1 - Easy\n"
            "2 - Medium\n"
            "3 - Hard\n"
            "Option: ";
    
    std::cout << szText;
}

// To assign values to the players attributes
void Console::selectPlayer(int iPlayer, int iDifficulty)
{
    // Creating a temporary player and assigning it values based on the user input
    Player tempPlayer;
    
    switch (iPlayer)
    {
        // 1 - Minimax
        case 1:
            tempPlayer.szTypeOfPlayer = "Minimax";
            
            switch (iDifficulty)
            {
                case 1:
                    tempPlayer.iDifficulty = 1;
                    break;
                    
                case 2:
                    tempPlayer.iDifficulty = 2;
                    break;
                    
                case 3:
                    tempPlayer.iDifficulty = 4;
                    break;
                    
                default:
                    break;
            }
            
            break;
        // 2 - Alpha Beta Pruning
        case 2:
            tempPlayer.szTypeOfPlayer = "AlphaBetaPruning";
            
            switch (iDifficulty)
            {
                case 1:
                    tempPlayer.iDifficulty = 1;
                    break;
                    
                case 2:
                    tempPlayer.iDifficulty = 4;
                    break;
                    
                case 3:
                    tempPlayer.iDifficulty = 6;
                    break;
                    
                default:
                    break;
            }
            
            break;
        // 3 - Monte Carlo Tree Search
        case 3:
            tempPlayer.szTypeOfPlayer = "MonteCarloTreeSearch";
            
            switch (iDifficulty)
            {
                case 1:
                    tempPlayer.iDifficulty = 10;
                    break;
                    
                case 2:
                    tempPlayer.iDifficulty = 50;
                    break;
                    
                case 3:
                    tempPlayer.iDifficulty = 100;
                    break;
                    
                default:
                    break;
            }
            
            break;
        // 4 - Human
        case 4:
            tempPlayer.szTypeOfPlayer = "Human";
            break;
            
        default:
            break;
    }
    
    // If player1 has not yet been assigned any values
    if (this->player1.szTypeOfPlayer == "None")
    {
        // Make it equal to the temporary player
        this->player1 = tempPlayer;
    }
    // If player2 has not yet been assigned any values
    else if (this->player2.szTypeOfPlayer == "None")
    {
        // Make it equal to the temporary player
        this->player2 = tempPlayer;
    }
    
}

// To play a game based on the selected players
void Console::startGame()
{
    // player 1 always starts
    Player currentPlayer = this->player1;
    
    // Character for allowing players to quit during a game (when playing agains an algorithm)
    char cToQuit = ' ';
    
    this->mainGame.printBoard();
    
    std::vector<Board> allBoards;
    
    allBoards.push_back(this->mainGame.getGameBoard());
    
    // While there is no winner
    while (this->mainGame.getWinner() == NoColour)
    {
        // Cue for who's move it is
        std::cout << this->mainGame.getCurrentPlayerColour() << " to move." << std::endl;
        
        // If the current player is a minimax algorithm
        if (currentPlayer.szTypeOfPlayer == "Minimax")
        {
            // Get the best move based on the current board state, and search depth based on the difficulty
            this->mainGame = this->minimax.getBestGameState(this->mainGame, currentPlayer.iDifficulty);
            
            this->minimax = Minimax();
            std::cin >> cToQuit;

        }
        // If the current player is a alpha beta prunning algorithm
        else if (currentPlayer.szTypeOfPlayer == "AlphaBetaPruning")
        {
            // Get the best move based on the current board state, and search depth based on the difficulty
            this->mainGame = this->alphaBetaPruning.getBestGameState(this->mainGame, currentPlayer.iDifficulty);
            
            this->alphaBetaPruning = AlphaBetaPruning();
            std::cin >> cToQuit;

        }
        // If the current player is a monte carlo tree search algorithm
        else if (currentPlayer.szTypeOfPlayer == "MonteCarloTreeSearch")
        {
            // Get the best move based on the current board state, and number of simulations to do
            this->mainGame = this->monteCarloTreeSearch.getBestGameState(this->mainGame, this->mainGame.getCurrentPlayerColour(), currentPlayer.iDifficulty);
            
            this->monteCarloTreeSearch = MonteCarloTreeSearch();
            std::cin >> cToQuit;
        }
    
        // If the current player is human
        else if (currentPlayer.szTypeOfPlayer == "Human")
        {
            Position toSelectPosition;
            Position toMovePosition;
            
            
            // To give the player a choice to get a hint or to quit the game before proceeding to make a move
            char cQuitOrHint;
            
            std::cout << "Quit or Hint (q/h): ";
            std::cin >> cQuitOrHint;
            // If they want to quit
            if (cQuitOrHint == 'q')
            {
                cToQuit = 'q';
                continue;
            }
            // If they wnat a hint
            else if (cQuitOrHint == 'h')
            {
                // a move is made by the alpha beta pruning algorithm witha depth of 8
                this->mainGame = this->alphaBetaPruning.getBestGameState(this->mainGame, 8);
                
                continue;
            }
            
            // Allow them to select a piece
            do
            {
                std::cout << "Select:" << std::endl;
                std::cout << "X: ";
                std::cin >> toSelectPosition.x;

                std::cout << "Y: ";
                std::cin >> toSelectPosition.y;
                
            } while (!mainGame.select(toSelectPosition));

            // And move the selected piece to a valid position
            do
            {
                std::cout << "Move to:" << std::endl;
                std::cout << "X: ";
                std::cin >> toMovePosition.x;

                std::cout << "Y: ";
                std::cin >> toMovePosition.y;
            } while (!mainGame.move(toMovePosition));
        }
        
        // If a q is typed the game quits
        if (cToQuit == 'q')
        {
            // Game state is reset
            this->mainGame = Game();
            
            return;
        }
        
        
        this->mainGame.printBoard();
        
        allBoards.push_back(this->mainGame.getGameBoard());
        
        // If there is a draw, the game quits
        if (this->mainGame.checkDraw(allBoards))
        {
            std::cout << "Draw" << std::endl;
            
            // Game state is reset
            this->mainGame = Game();

            return;
        }
        
        // Change the current player
        if (this->mainGame.getCurrentPlayerColour() == White)
        {
            currentPlayer = player1;
        }
        else
        {
            currentPlayer = player2;
        }
        
    }
    // If the game end and there is winner
    // Print the colour of the winner
    std::cout << this->mainGame.getWinner() << " won!" << std::endl;
    
    // Game state is reset
    this->mainGame = Game();
}

// Constructor
Console::Console()
{
    this->printWelcomeMessage();
}

// Destructor
Console::~Console()
{
    this->printGoodbyeMessage();
}
