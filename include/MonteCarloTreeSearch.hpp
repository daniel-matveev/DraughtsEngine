//
//  MonteCarloTreeSearch.hpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#ifndef MonteCarloTreeSearch_hpp
#define MonteCarloTreeSearch_hpp

#include "Game.hpp"


// Node structure for the tree search
struct Node
{
    int iNumberOfVisits;
    float fTotalScore;
    
    // The game state at that node
    Game gameState;
    
    // Pointers to the child nodes of the current node
    std::vector< Node * > childNodes;
    
    // Constructors
    Node();
    Node(Game gameState);
};

class MonteCarloTreeSearch
{
private:
    
    // The initial game state passed to be analysed
    Node * rootNode;
    
    Colour playerColour;

    std::mt19937 rng;
    
    // To simulate multiple games from the same node
    int iNumberOfSimulationsInRollOut;
    
    // To keep track how many simulations have been made
    // Also used in the UCB formula
    float iNumberOfTotalSimulations;
    
    
    // Checks if the node passed has any children
    bool isLeafNode(Node toCheckNode);
    
    // Calculates the UCB score for a given node
    float getUCBScore(Node toCalculateNode);
    
    // Game simulation to completion
    float rollOut(Node toRolloutNode);
    
    // helper function
    // Will return a 'copy' of the game state with the opponent move executed
    Game simulateMove(Position toMovePosition, Game toAnalyseGame);
    
    // Recursive tree traversal function until a child node is found
    float selectNode(Node * currentNode);
    
    // To free up the memory allocated during the recursive tree traversal
    void deleteTree(Node * currentNode);
    
public:
    // Constructor / Destructor
    MonteCarloTreeSearch(unsigned int seed = std::random_device{}());
    ~MonteCarloTreeSearch();
    
    // Will return the best move found after a set number of simulations have been run
    // Before each simulation the tree is traversed, nodes are expanded.
    Game getBestGameState(Game toAnalyseGame, Colour currentPlayerColour, int iNumberOfSimulations);
    
};

#endif /* MonteCarloTreeSearch_hpp */
