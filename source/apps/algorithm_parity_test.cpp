/*
    algorithm_parity_test.cpp

    Testing suite to compare the output of the minimax and minimax with alpha-beta pruning algorithms. 
    The program will run a number of games between the two algorithms and output the results to a file.
*/

#include <iostream>
#include <random>

#include "Game.hpp"
#include "Minimax.hpp"
#include "AlphaBetaPruning.hpp"

int main(int argc, const char * argv[])
{
    const int iNumberOfGames = 100;
    const int iMaxMoves = 40;
    const int iSearchDepth = 3;

    std::mt19937 rng(67); // Fixed seed for reproducibility

    int mismatches = 0;

    for (int g = 0; g < iNumberOfGames; g++)
    {
        Game game;
        game.getWinner(); // Ensure selectablePices is populated

        for (int m = 0; m < iMaxMoves && game.getWinner() == NoColour; m++)
        {
            Minimax minimax;
            AlphaBetaPruning alphaBetaPruning;

            Game minimaxNextBestGameState = minimax.getBestGameState(game, iSearchDepth);
            Game alphaBetaNextBestGameState = alphaBetaPruning.getBestGameState(game, iSearchDepth);

            if (!(minimaxNextBestGameState.getGameBoard() == alphaBetaNextBestGameState.getGameBoard()))
            {
                mismatches++;

                std::cout << "Mismatch found in game " << g + 1 << ", move " << m + 1 << std::endl;
                game.printBoard();
                std::cout << "Minimax next best game state:" << std::endl;
                minimaxNextBestGameState.printBoard();
                std::cout << "Alpha-beta pruning next best game state:" << std::endl;
                alphaBetaNextBestGameState.printBoard();
                break; // Stop further moves for this game
            }

            // Advance the game
            game.playRandomMove(rng);
        }
    }

    std::cout << "Testing completed. Total mismatches: " << mismatches << " out of " << iNumberOfGames * iMaxMoves << " positions. At depth: " << iSearchDepth << std::endl;

    return mismatches == 0 ? 0 : 1; // Return 0 if no mismatches, else return 1
}