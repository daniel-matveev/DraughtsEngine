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

void Console::reset()
{
    this->player1 = Player();
    this->player2 = Player();
    
    this->mainGame = Game();
}

// To print welcome message
void Console::printWelcomeMessage()
{
    std::cout << "\nWelcome" << std::endl;
}

// To print goodbye message
void Console::printGoodbyeMessage()
{
    std::cout << "\nGoodbye" << std::endl;
}

// Main menu options
void Console::printStartOptions()
{
    std::string szText;
    
    szText =
            "\n"
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
            "\n"
            "Game Rules:\n"
            " - A regular piece can move and capture on the diagonal in a forward direction\n"
            " - If a piece can take then it has to take\n"
            " - A piece can jump over multiple opponent pieces and capture them\n"
            " - Once a piece reaches the other side of the board, it becomes a king\n"
            " - A king can move in all directions\n"
            " - The game is over once a player can no longer make a move\n"
            " - If you want to exit a game type q at any point in the console\n"
            " - If you want to get a hint type h\n"
    ;
    
    std::cout << szText << std::endl;
}

// Player selection menu
void Console::printPlayerSelection()
{
    std::string szText;
    
    szText =
            "\n"
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
            "\n"
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
                    tempPlayer.iDifficulty = 2;
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

char Console::extractInvalidInput()
{
    char cToExtract;

    std::cin.clear();
    
    std::cin.get(cToExtract);
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return cToExtract;
}

void Console::handleInvalidInput(char cExtracted)
{
    switch (cExtracted) {
        case 'q':
            std::cout << "Quiting game" << std::endl;
            break;
        case 'h':
            std::cout << "Geting help" << std::endl;
            this->mainGame = this->alphaBetaPruning.getBestGameState(this->mainGame, 6);
            break;
            
        default:
            std::cout << "Invalid Input!\n";
            std::cout << "Please re-enter [positive integer only]" << std::endl;
            break;
    }
}

// To play a game based on the selected players
void Console::startGame()
{
    // player 1 always starts
    Player currentPlayer = this->player1;
    
    // Character for allowing players to quit during a game (when playing agains an algorithm)
    char cExtracted = ' ';
    
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
            
//            this->minimax = Minimax();
            std::cin >> cExtracted;

        }
        // If the current player is a alpha beta prunning algorithm
        else if (currentPlayer.szTypeOfPlayer == "AlphaBetaPruning")
        {
            // Get the best move based on the current board state, and search depth based on the difficulty
            this->mainGame = this->alphaBetaPruning.getBestGameState(this->mainGame, currentPlayer.iDifficulty);
            
//            this->alphaBetaPruning = AlphaBetaPruning();
            std::cin >> cExtracted;

        }
        // If the current player is a monte carlo tree search algorithm
        else if (currentPlayer.szTypeOfPlayer == "MonteCarloTreeSearch")
        {
            // Get the best move based on the current board state, and number of simulations to do
            this->mainGame = this->monteCarloTreeSearch.getBestGameState(this->mainGame, this->mainGame.getCurrentPlayerColour(), currentPlayer.iDifficulty);
            
//            this->monteCarloTreeSearch = MonteCarloTreeSearch();
            std::cin >> cExtracted;
        }
    
        // If the current player is human
        else if (currentPlayer.szTypeOfPlayer == "Human")
        {
            Position toSelectPosition = Position {0, 0};
            Position toMovePosition = Position {0, 0};
            
            char cExtracted = ' ';
            

            // Allow them to select a piece
            while ( (!mainGame.select(toSelectPosition)) && (cExtracted != 'q' && cExtracted != 'h') )
            {
                std::cout << "Select:" << std::endl;
                
                std::cout << "X: ";
                std::cin >> toSelectPosition.x;
                std::cout << "\n";
                
                if (std::cin.fail())
                {
                    cExtracted = this->extractInvalidInput();
                
                    this->handleInvalidInput(cExtracted);
                    
                    continue;
                }
                

                std::cout << "Y: ";
                std::cin >> toSelectPosition.y;
                std::cout << "\n";
                
                if (std::cin.fail())
                {
                    cExtracted = this->extractInvalidInput();
                    
                    this->handleInvalidInput(cExtracted);
                    
                    continue;
                }
                    
                    
            }
                
            // And move the selected piece to a valid position
            while ((!mainGame.move(toMovePosition)) && (cExtracted != 'q' && cExtracted != 'h') )
            {
                std::cout << "Move to:" << std::endl;
                
                std::cout << "X: ";
                std::cin >> toMovePosition.x;
                std::cout << "\n";
                
                if (std::cin.fail())
                {
                    cExtracted = this->extractInvalidInput();
                
                    this->handleInvalidInput(cExtracted);
                    
                    continue;
                }
                
                std::cout << "Y: ";
                std::cin >> toMovePosition.y;
                std::cout << "\n";
                
                if (std::cin.fail())
                {
                    cExtracted = this->extractInvalidInput();
                
                    this->handleInvalidInput(cExtracted);
                    
                    continue;
                }
                
                
                
            }
        }
        
        // If a q is typed the game quits
        if (cExtracted == 'q')
        {
            // Game state is reset
            this->reset();
            
            return;
        }
        
        
        this->mainGame.printBoard();
        
        allBoards.push_back(this->mainGame.getGameBoard());
        
        // If there is a draw, the game quits
        if (this->mainGame.checkDraw(allBoards))
        {
            std::cout << "Draw" << std::endl;
            
            // Game state is reset
            this->reset();

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
    this->reset();
}

// Constructor
Console::Console()
{
    this->reset();
    this->printWelcomeMessage();
}

// Destructor
Console::~Console()
{
    this->printGoodbyeMessage();
}
