#ifndef ALPHA_BETA_PRUNING_MOVE_ORDERING_HPP
#define ALPHA_BETA_PRUNING_MOVE_ORDERING_HPP

#include "AlphaBetaPruning.hpp"

class AlphaBetaPruningMoveOrdering : AlphaBetaPruning
{
    public:
        AlphaBetaPruningMoveOrdering();
        ~AlphaBetaPruningMoveOrdering();

        Game getBestGameState(Game toAnalyseGame, int depth, int shallowDepth);
};


#endif