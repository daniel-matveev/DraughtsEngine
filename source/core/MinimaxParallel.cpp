#include "MinimaxParallel.hpp"

MinimaxParallel::MinimaxParallel() {}

MinimaxParallel::~MinimaxParallel() {}


Game MinimaxParallel::getBestGameState(Game toAnalyseGame, int iDepth)
{
    Colour playerColour = toAnalyseGame.getCurrentPlayerColour();

    // Get all the possible games for the current player
    std::vector<Game> toAnalyseGames = this->getAllPossibleGames(toAnalyseGame);
    
    // Create a vector to store the scores for each possible game
    std::vector<float> scores(toAnalyseGames.size());
    
    // Use OpenMP to parallelize the minimax evaluation of each possible game
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < toAnalyseGames.size(); ++i)
    {
        scores[i] = this->minimax(toAnalyseGames[i], iDepth);
    }
 
    // Find the index of the best score
    int bestIndex = 0;
    float bestScore = scores[0];
    
    for (int i = 1; i < scores.size(); ++i)
    {
        if (playerColour == White && scores[i] > bestScore)
        {
            bestScore = scores[i];
            bestIndex = i;
        }
        else if (playerColour == Black && scores[i] < bestScore)
        {
            bestScore = scores[i];
            bestIndex = i;
        }
    }
    
    // Return the game state corresponding to the best score
    return toAnalyseGames[bestIndex];
}