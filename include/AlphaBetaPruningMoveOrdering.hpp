#ifndef ALPHA_BETA_PRUNING_MOVE_ORDERING_HPP
#define ALPHA_BETA_PRUNING_MOVE_ORDERING_HPP

#include "AlphaBetaPruning.hpp"

class AlphaBetaPruningMoveOrdering : public AlphaBetaPruning
{
    public:
        AlphaBetaPruningMoveOrdering();
        ~AlphaBetaPruningMoveOrdering();

        int iShallowNumberOfLeafNodes = 0;
        int iNumberTiedMoves = 0;

        Game getBestGameState(Game toAnalyseGame, int depth, int shallowDepth);
};


#endif