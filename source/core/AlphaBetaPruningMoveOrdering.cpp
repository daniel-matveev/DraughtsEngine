#include "AlphaBetaPruningMoveOrdering.hpp"

AlphaBetaPruningMoveOrdering::AlphaBetaPruningMoveOrdering() : AlphaBetaPruning()
{
}

AlphaBetaPruningMoveOrdering::~AlphaBetaPruningMoveOrdering()
{
}

Game AlphaBetaPruningMoveOrdering::getBestGameState(Game toAnalyseGame, int depth, int shallowDepth)
{
    this->iNumberOfLeafNodes = 0;
    // Get all possible games
    std::vector<Game> possibleGames = this->getAllPossibleGames(toAnalyseGame);
    
    std::vector<std::pair<float, int>> shallowEvaluations(possibleGames.size());
    for (int i = 0; i < possibleGames.size(); i++)
    {
        float evaluation = this->alphaBetaPruning(possibleGames[i], shallowDepth, -INFINITY, INFINITY);
        shallowEvaluations[i] = {evaluation, i};
    }

    Colour playerColour = toAnalyseGame.getCurrentPlayerColour();

    std::stable_sort(shallowEvaluations.begin(), shallowEvaluations.end(),
    [&](const auto& a, const auto& b) {
        return playerColour == White ? a.first > b.first : a.first < b.first;
    });

    int iBestBoardStateIndex = -1;
    
    float fMinEvaluation = INFINITY;
    
    float fMaxEvaluation = -INFINITY;

    float fAlpha = -INFINITY;
    float fBeta = INFINITY;

    for (int i = 0; i < shallowEvaluations.size(); i++)
    {
        int index = shallowEvaluations[i].second;

        float fEvaluation = this->alphaBetaPruning(possibleGames[index], depth, fAlpha, fBeta);

        if (playerColour == White && fEvaluation > fMaxEvaluation)
        {
            fMaxEvaluation = fEvaluation;
            iBestBoardStateIndex = index;
    
            fAlpha = std::max(fAlpha, fMaxEvaluation);
        }
        else if (playerColour == Black && fEvaluation < fMinEvaluation)
        {

            fMinEvaluation = fEvaluation;
            iBestBoardStateIndex = index;
            
            fBeta = std::min(fBeta, fMinEvaluation);
        }

        if (fBeta <= fAlpha)
        {
            break; // Alpha-beta pruning
        }
    }

    return possibleGames[iBestBoardStateIndex];
}