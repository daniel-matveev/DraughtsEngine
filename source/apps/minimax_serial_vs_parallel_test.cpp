#include "Minimax.hpp"
#include "MinimaxParallel.hpp"

#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

#include <omp.h>

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

int main(int argc, char* argv[])
{
    const std::vector<int> depths = {3, 4, 5, 6, 7};

    const int iPositionsPerDepth = 15;
    const int iMovesToAdvance = 15;

    const unsigned int iSeed = 67; // Fixed seed for reproducibility
    std::mt19937 rng(iSeed); 

    std::filesystem::create_directories("results/minimax_serial_vs_parallel_test"); // Ensure the results directory exists
    std::ofstream csv("results/minimax_serial_vs_parallel_test/minimax_serial_vs_parallel_results.csv");
    csv << "game_id,depth,threads,serial_wall_ms,parallel_wall_ms,move_aggrement,node_count_serial,node_count_parallel\n";

    for (int depth : depths)
    {
        for (int positionIndex = 0; positionIndex < iPositionsPerDepth; positionIndex++)
        {
            Game basePosition;
            
            while (!generateRandomPosition(basePosition, rng, iMovesToAdvance)) { }

            // Minimax Serial
            
            Minimax minimaxSerial;
            Game gameSerial = basePosition; // Reset to the base position for each algorithm
            minimaxSerial.iNumberOfLeafNodes = 0; // Reset the leaf node counter
            auto startSerial = std::chrono::high_resolution_clock::now();
            Game bestGameSerial = minimaxSerial.getBestGameState(gameSerial, depth);
            auto endSerial = std::chrono::high_resolution_clock::now();
            double serialWallMs = std::chrono::duration<double, std::milli>(endSerial - startSerial).count();
            int nodeCountSerial = minimaxSerial.iNumberOfLeafNodes.load(); // Get the leaf node count for serial

            // Minimax Parallel
            MinimaxParallel minimaxParallel;
            Game gameParallel = basePosition; // Reset to the base position for each algorithm
            minimaxParallel.iNumberOfLeafNodes = 0; // Reset the leaf node counter
            auto startParallel = std::chrono::high_resolution_clock::now();
            Game bestGameParallel = minimaxParallel.getBestGameState(gameParallel, depth);
            auto endParallel = std::chrono::high_resolution_clock::now();
            double parallelWallMs = std::chrono::duration<double, std::milli>(endParallel - startParallel).count();
            int nodeCountParallel = minimaxParallel.iNumberOfLeafNodes.load(); // Get the leaf node count for parallel

            int numMovesParallel = minimaxParallel.getNumMoves(basePosition);

            if (numMovesParallel == 1) 
            {
                positionIndex--; // Decrement positionIndex to retry this depth with a new random position
                std::cout << "Skipping position " << positionIndex + 1 << " at depth " << depth << " due to only one possible move." << std::endl;
                continue;
            }

            // Check if both algorithms agree on the best move
            bool moveAgreement = (bestGameSerial.getGameBoard() == bestGameParallel.getGameBoard());

            csv << positionIndex << "," << depth << "," << omp_get_max_threads() << "," << serialWallMs << "," << parallelWallMs << "," << moveAgreement << "," << nodeCountSerial << "," << nodeCountParallel << "\n";
            csv.flush();
            
            std::cout << "Depth: " << depth << ", Position: " << positionIndex << ", Serial Time: " << serialWallMs << " ms, Parallel Time: " << parallelWallMs << " ms, Move Agreement: " << moveAgreement << ", Serial Nodes: " << nodeCountSerial << ", Parallel Nodes: " << nodeCountParallel << std::endl;
        }
    }

    csv.close();

    return 0;
}