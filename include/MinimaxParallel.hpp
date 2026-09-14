

#ifndef MINIMAX_PARALLEL_HPP
#define MINIMAX_PARALLEL_HPP

#include "Minimax.hpp"

class MinimaxParallel : public Minimax
{
    public:
        MinimaxParallel();
        ~MinimaxParallel();
        int getNumMoves(Game game) { return getAllPossibleGames(game).size(); }
        Game getBestGameState(Game toAnalyseGame, int iDepth);
};

#endif // MINIMAX_PARALLEL_HPP