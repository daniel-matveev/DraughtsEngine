
#include "Game.hpp"
#include "Minimax.hpp"
#include "AlphaBetaPruning.hpp"

#include <iostream>
#include <random>
#include <fstream>
#include <chrono>

bool generateRandomPosition(Game &game, std::mt19937 &rng, int iMovesToAdvance)
{
    game = Game();

    for (int m = 0; m < iMovesToAdvance; m++)
    {
        if (game.getWinner() != NoColour)
        {
            return false;
        }
        game.playRandomMove(rng);

    }
    return game.getWinner() == NoColour; // Return true if the game is still ongoing, false if there's a winner
}


int main(int argc, const char * argv[])
{
    const std::vector<int> depths = {3, 4, 5, 6, 7}; 

    const int iPositionsPerDepth = 15;
    const int iMovesToAdvance = 15;

    const unsigned int iSeed = 67; // Fixed seed for reproducibility

    std::mt19937 rng(iSeed); // Fixed seed for reproducibility

    std::filesystem::create_directories("results"); // Ensure the results directory exists
    std::ofstream csv("results/benchmark_results.csv");
    csv << "depth,algorithm,position_index,nodes_visited,time_ms\n";

    
    for (int depth : depths)
    {
        for (int positionIndex = 0; positionIndex < iPositionsPerDepth; positionIndex++)
        {
            Game basePosition;
            
            while (!generateRandomPosition(basePosition, rng, iMovesToAdvance)) { }

            // Minimax
            {
                Minimax minimax;
                Game game = basePosition; // Reset to the base position for each algorithm
                auto start = std::chrono::high_resolution_clock::now();
                minimax.getBestGameState(game, depth);
                auto end = std::chrono::high_resolution_clock::now();

                double ms = std::chrono::duration<double, std::milli>(end - start).count();

                csv << depth << ",minimax," << positionIndex << "," << minimax.iNumberOfLeafNodes << "," << ms << "\n";
                csv.flush(); // Ensure data is written to the file immediately

                std::cout << "Minimax - Depth: " << depth << ", Position: " << positionIndex << ", Nodes Visited: " << minimax.iNumberOfLeafNodes << ", Time (ms): " << ms << std::endl;
            }
            // Alpha-Beta Pruning
            {
                AlphaBetaPruning alphaBeta;
                Game game = basePosition; // Reset to the base position for each algorithm
                auto start = std::chrono::high_resolution_clock::now();
                alphaBeta.getBestGameState(game, depth);
                auto end = std::chrono::high_resolution_clock::now();

                double ms = std::chrono::duration<double, std::milli>(end - start).count();

                csv << depth << ",alphabeta," << positionIndex << "," << alphaBeta.iNumberOfLeafNodes << "," << ms << "\n";
                csv.flush(); // Ensure data is written to the file immediately

                std::cout << "Alpha-Beta - Depth: " << depth << ", Position: " << positionIndex << ", Nodes Visited: " << alphaBeta.iNumberOfLeafNodes << ", Time (ms): " << ms << std::endl; 
            }
        }
    }

    csv.close();
    return 0;
}