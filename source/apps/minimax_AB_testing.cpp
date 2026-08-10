/*
    minimax_AB_testing.cpp

    Testing suite to compare the output of the minimax and minimax with alpha-beta pruning algorithms. 
    The program will run a number of games between the two algorithms and output the results to a file.
*/

#include "Game.hpp"
#include "Minimax.hpp"
#include "AlphaBetaPruning.hpp"

int main(int argc, const char * argv[])
{
    Game game;

    game.printBoard();
    game.getWinner();
    Minimax minimax;
    AlphaBetaPruning alphaBetaPruning;
    std::cout << "Running minimax and alpha-beta pruning algorithms on the same game state..." << std::endl;
    Game minimaxNextBestGameState = minimax.getBestGameState(game, 3);
    std::cout << "Minimax algorithm completed." << std::endl;
    minimaxNextBestGameState.printBoard();
    
    
    Game alphaBetaNextBestGameState = alphaBetaPruning.getBestGameState(game, 3);
    std::cout << "Alpha-beta pruning algorithm completed." << std::endl;


    
    alphaBetaNextBestGameState.printBoard();

    Board minimaxBoard = minimaxNextBestGameState.getGameBoard();
    Board alphaBetaBoard = alphaBetaNextBestGameState.getGameBoard();

    if (minimaxBoard == alphaBetaBoard)
    {
        std::cout << "The next best game states are the same for both algorithms." << std::endl;
    }
    else
    {
        std::cout << "The next best game states are different for the two algorithms." << std::endl;
    }

    return 0;
}