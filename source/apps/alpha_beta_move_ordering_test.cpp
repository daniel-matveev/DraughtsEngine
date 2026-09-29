#include <iostream>
#include <random>
#include <fstream>
#include <chrono>

#include "Game.hpp"
#include "Minimax.hpp"
#include "AlphaBetaPruningMoveOrdering.hpp"
#include "AlphaBetaPruning.hpp"

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

    const int iMovesToAdvance = 15;
    const int iPositionsPerDepth = 15;

    const unsigned int seed = 67; // Fixed seed for reproducibility
    std::mt19937 rng(seed); 

    std::filesystem::create_directories("results/alpha_beta_move_ordering_test"); // Ensure the results directory exists
    std::ofstream csv("results/alpha_beta_move_ordering_test/alpha_beta_move_ordering_test.csv");
    csv << "position_id,depth,shallow_depth,algorithm,phase,nodes,score,num_moves,num_tied_moves\n";

    for (int positionIndex = 0; positionIndex < iPositionsPerDepth; positionIndex++)
    {
        Game basePosition;

        while (!generateRandomPosition(basePosition, rng, iMovesToAdvance)) { }

        Minimax testingMinimax;

        int iNumberOfMoves = testingMinimax.getNumMoves(basePosition);

        if (iNumberOfMoves == 1)
        {
            positionIndex--;
            continue; // Skip this position if there's only one move available
        }

        std::cout << "Position " << positionIndex << " generated with " << iNumberOfMoves << " moves available." << std::endl;

        for (int depth : depths)
        {
            {
                Minimax minimaxParallel;
                
                Game game = basePosition;

                minimaxParallel.getBestGameState(game, depth);

                csv << positionIndex << "," << depth << ",N/A,Minimax,full," << minimaxParallel.iNumberOfLeafNodes << "," << minimaxParallel.fBestEvaluation << "," << iNumberOfMoves << ",N/A\n";
                csv.flush();

                std::cout << "Position " << positionIndex << ", Depth " << depth << ", Minimax completed. Nodes: " << minimaxParallel.iNumberOfLeafNodes << ", Score: " << minimaxParallel.fBestEvaluation << std::endl;
            }

            {
                AlphaBetaPruning alphaBetaPruning;

                Game game = basePosition;

                alphaBetaPruning.getBestGameState(game, depth);

                csv << positionIndex << "," << depth << ",N/A,AlphaBetaPruning,full," << alphaBetaPruning.iNumberOfLeafNodes << "," << alphaBetaPruning.fBestEvaluation << "," << iNumberOfMoves << ",N/A\n";
                csv.flush();

                std::cout << "Position " << positionIndex << ", Depth " << depth << ", AlphaBetaPruning completed. Nodes: " << alphaBetaPruning.iNumberOfLeafNodes << ", Score: " << alphaBetaPruning.fBestEvaluation << std::endl;
            }

            for (int shallowDepth = 0; shallowDepth <= depth; shallowDepth++)
            {
                AlphaBetaPruningMoveOrdering alphaBetaPruningMoveOrdering;

                Game game = basePosition;

                alphaBetaPruningMoveOrdering.getBestGameState(game, depth, shallowDepth);

                int shallowNodes = alphaBetaPruningMoveOrdering.iShallowNumberOfLeafNodes;
                int fullNodes = alphaBetaPruningMoveOrdering.iNumberOfLeafNodes - shallowNodes;

                csv << positionIndex << "," << depth << "," << shallowDepth << ",AlphaBetaPruningMoveOrdering,shallow," << shallowNodes << "," << alphaBetaPruningMoveOrdering.fBestEvaluation << "," << iNumberOfMoves << "," << alphaBetaPruningMoveOrdering.iNumberTiedMoves << "\n";
                csv << positionIndex << "," << depth << "," << shallowDepth << ",AlphaBetaPruningMoveOrdering,full," << fullNodes << "," << alphaBetaPruningMoveOrdering.fBestEvaluation << "," << iNumberOfMoves << "," << alphaBetaPruningMoveOrdering.iNumberTiedMoves << "\n";
                csv.flush();

                std::cout << "Position " << positionIndex << ", Depth " << depth << ", Shallow Depth " << shallowDepth << ", AlphaBetaPruningMoveOrdering completed. Shallow Nodes: " << shallowNodes << ", Full Nodes: " << fullNodes << ", Score: " << alphaBetaPruningMoveOrdering.fBestEvaluation << ", Tied Moves: " << alphaBetaPruningMoveOrdering.iNumberTiedMoves << std::endl;
            }
        }
    }
    csv.close();
    return 0;
}