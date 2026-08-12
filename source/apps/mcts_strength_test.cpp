
#include <iostream>
#include <random>
#include <fstream>
#include <chrono>

#include "Game.hpp"
#include "MonteCarloTreeSearch.hpp"

int main(int argc, const char * argv[])
{
    const int iNumberOfGames = 200;
    const int iMaxMoves = 80;
    const int iSimulations = 50;

    const int iSeed = 67; // Fixed seed for reproducibility
    std::mt19937 rng(iSeed); 

    int mctsWins = 0;
    int randomWins = 0;
    int draws = 0;

    std::ofstream csv("./results/mcts_strength_results.csv");
    csv << "game_index,mcts_colour,winner,moves\n";

    for (int g = 0; g < iNumberOfGames; g++)
    {
        Game game;
        game.getWinner(); // Ensure selectablePices is populated

        MonteCarloTreeSearch mcts;

        Colour mctsPlayerColour = (g % 2 == 0) ? Black : White; // Alternate starting player for MCTS

        int m = 0;
        for (; m < iMaxMoves && game.getWinner() == NoColour; m++)
        {
            if (game.getCurrentPlayerColour() == mctsPlayerColour)
            {
                game = mcts.getBestGameState(game, mctsPlayerColour, iSimulations);
            }
            else
            {
                game.playRandomMove(rng);
            }
            game.getWinner();
        }

        Colour winner = game.getWinner();
        if (winner == mctsPlayerColour) mctsWins++;
        else if (winner == NoColour) draws++;
        else randomWins++;

        csv << g << "," << mctsPlayerColour << "," << winner << "," << m << "\n";
        csv.flush();

        std::cout << "game " << g << ": mcts=" << mctsPlayerColour << " winner=" << winner << " moves=" << m << "\n";
    }


    std::cout << "Testing completed. MCTS wins: " << mctsWins << ", Random wins: " << randomWins << ", Draws: " << draws << " out of " << iNumberOfGames<< std::endl;

    return 0; // Return 0 to indicate success
}