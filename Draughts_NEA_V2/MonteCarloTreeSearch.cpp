//
//  MonteCarloTreeSearch.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 22/10/2024.
//

#include "MonteCarloTreeSearch.hpp"


// Node cosntructors
// Assigns a game state for that node
Node::Node(Game gameState)
{
    this->iNumberOfVisits = 0;
    this->fTotalScore = 0;
    this->gameState = gameState;
    
}

Node::Node()
{
    this->iNumberOfVisits = 0;
    this->fTotalScore = 0;
}

// MCTS constructor
MonteCarloTreeSearch::MonteCarloTreeSearch()
{
    this->iNumberOfSimulationsInRollOut = 10;
    this->rootNode = new Node();
}

// MCTS destructor
MonteCarloTreeSearch::~MonteCarloTreeSearch()
{
    // Free up all the allocated memory
    if (this->rootNode == nullptr)
    {
        if (!isLeafNode(* this->rootNode))
        {
            this->deleteTree(this->rootNode);
        }

    }
}

// Recursevely traverses the tree and deletes the nodes if they are leaf nodes
void MonteCarloTreeSearch::deleteTree(Node * currentNode)
{
    int iSize = (int) currentNode->childNodes.size();
    for (int i = 0; i < iSize; i++)
    {
        this->deleteTree(currentNode->childNodes.at(0));
        
        currentNode->childNodes.erase(currentNode->childNodes.begin());
    }
//    free(currentNode);
    delete currentNode;
}

// Returns true if the node passed is a leaf node
// (If there are no children)
bool MonteCarloTreeSearch::isLeafNode(Node toCheckNode)
{
    if (toCheckNode.childNodes.size() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Calculates the UCB score for a given node
float MonteCarloTreeSearch::getUCBScore(Node toCalculateNode)
{
    // Would get division by 0
    // Not allowed
    // But need to return +inf
    if (toCalculateNode.iNumberOfVisits == 0)
    {
        return INT_MAX;
    }
    
    // Formula
    float C = 2;
    float dNodeMean = ( toCalculateNode.fTotalScore) / ((float) toCalculateNode.iNumberOfVisits);
    
    return dNodeMean + C * sqrt( log(this->iNumberOfTotalSimulations) / toCalculateNode.iNumberOfVisits );
}

// Simulates multiple games from a starting game position
float MonteCarloTreeSearch::rollOut(Node toRolloutNode)
{
    int iTotalScore = 0;
    
    unsigned long iNumberOfSelectablePieces;
    unsigned long iNumberOfEndMoves;
    
    int iRandomToSelectPiece;
    int iRandomMove;
    
    int iWins = 0;
    int iLoses = 0;
    
    Game tempGame;
    
    for (int i = 0; i < this->iNumberOfSimulationsInRollOut; i++)
    {
        // To store the game state in the node without altering the game state in the node
        tempGame = toRolloutNode.gameState;
        
        while (tempGame.getWinner() == NoColour)
        {
            // Make a random piece selection from the selectable pieces
            iNumberOfSelectablePieces = (int) tempGame.selectablePieces.size();
            
            iRandomToSelectPiece = rand() % iNumberOfSelectablePieces;
            
            std::set<Position>::iterator selectablePiecesIterator = tempGame.selectablePieces.begin();
            
            std::advance(selectablePiecesIterator, iRandomToSelectPiece);
            
            tempGame.select( * selectablePiecesIterator );
            
            
            // Make a random move from the selected piece
            iNumberOfEndMoves = tempGame.filteredEndPositionsToBoard.size();
            
            iRandomMove = rand() % iNumberOfEndMoves;
            
            std::unordered_map<Position, std::vector<Position> >::iterator filteredEndPositionsToBoardIterator = tempGame.filteredEndPositionsToBoard.begin();
            
            std::advance(filteredEndPositionsToBoardIterator, iRandomMove);
            
            tempGame.move(filteredEndPositionsToBoardIterator->first);
        
        }
    
        
        // If the player that won in the tempGame is the same player that we are running the search for
        // Increase their total score by 10
        if (tempGame.getWinner() == this->playerColour)
        {
            iWins = iWins + 1;
        }
        // If not increse it by 0
        else
        {
            iLoses = iLoses + 1;
        }
    }
    #ifdef DEBUG_FLAG_MCTS
        Debug("Ratio of wins to loses: " << iWins << ":" << iLoses);
    #endif
    
    iTotalScore = (10 * iWins) + iLoses;
    // Return the average of the games
    return ( (float) iTotalScore ) / this->iNumberOfSimulationsInRollOut;
}

// Returns a game state given a move to do
Game MonteCarloTreeSearch::simulateMove(Position toMovePosition, Game toAnalyseGame)
{
    toAnalyseGame.move(toMovePosition);
    
    return toAnalyseGame;
}

// Recursive function that traverses, expands the nodes and back propagates the results back up the tree
float MonteCarloTreeSearch::selectNode(Node & currentNode)
{
    // If the current node is a leaf node
    
    #ifdef DEBUG_FLAG_MCTS
        Debug("Checking node: " << &currentNode);
        Debug("Number of visits: " << currentNode.iNumberOfVisits);
        Debug("Total score: " << currentNode.fTotalScore);
    #endif
    
    if (this->isLeafNode(currentNode))
    {
        // And it hasn't been visited
        if (currentNode.iNumberOfVisits == 0)
        {
            // Simulate games from it and get its average win rate based on random moves
            #ifdef DEBUG_FLAG_MCTS
                Debug("Leaf node. Rolling out node: " << &currentNode);
            #endif
            float fFinalScore = this->rollOut(currentNode);

            // Increment the values of that node accordingly
            currentNode.iNumberOfVisits = currentNode.iNumberOfVisits + 1;
            currentNode.fTotalScore = currentNode.fTotalScore + fFinalScore;
            
            // Return the score for back propagation for the parent nodes
            return fFinalScore;
        }
        // But it has been visited
        // Node expansion
        else
        {
            // If a node is a terminal state (a player has won) it cannot be expanded
            if (currentNode.gameState.getWinner() != NoColour)
            {
                #ifdef DEBUG_FLAG_MCTS
                    Debug("Game over. Rolling out node: " << &currentNode);
                #endif
                // No games will be simulated, just get the average value of that node
                float fFinalScore = this->rollOut( currentNode );
                return fFinalScore;
            }
            
            #ifdef DEBUG_FLAG_MCTS
                Debug("Leaf node. Expanding node: " << &currentNode);
            #endif
            
            // For each selectable piece
            std::set<Position>::iterator selectablePiecesIterator = currentNode.gameState.selectablePieces.begin();
            
            for (; selectablePiecesIterator != currentNode.gameState.selectablePieces.end(); ++selectablePiecesIterator)
            {
                // Select that piece
                currentNode.gameState.select(Position {selectablePiecesIterator->x, selectablePiecesIterator->y});
                
                // Get its valid moves
                currentNode.gameState.getValidMoves(false);

                // And for each valid move
                std::unordered_map<Position, std::vector<Position> >::iterator filteredEndPositionsToBoardIterator = currentNode.gameState.filteredEndPositionsToBoard.begin();

                for (; filteredEndPositionsToBoardIterator != currentNode.gameState.filteredEndPositionsToBoard.end(); ++filteredEndPositionsToBoardIterator)
                {
                    // Make a new node where that move is taken
                    Game tempGame = this->simulateMove(filteredEndPositionsToBoardIterator->first, currentNode.gameState);

                    // Allocate memory for new node
                    Node * childNode = new Node(tempGame);
                    
                    // Add the node created to the vector of child nodes
                    currentNode.childNodes.push_back(childNode);
                }

                // Clear game state for the current piece to be selected
                currentNode.gameState.clear();

            }
            #ifdef DEBUG_FLAG_MCTS
                Debug("Number of children: " << currentNode.childNodes.size());
            #endif
            
            // Once the node is expanded, select the first child of that node and do a rollout
            float fFinalScore = this->rollOut( * currentNode.childNodes.at(0));
            
            // Update its values accordingly
            currentNode.childNodes.at(0)->iNumberOfVisits = currentNode.childNodes.at(0)->iNumberOfVisits + 1;
            currentNode.childNodes.at(0)->fTotalScore = currentNode.childNodes.at(0)->fTotalScore + fFinalScore;
            
           
            // return the score for backpropagation
            return fFinalScore;
        }
        
    }
    // If the node is not a leaf node
    // Select the child node with the highest UCB score
    else
    {
        // Variables to keep track of scores and the child index with the highest score
        float iBestScore = -1;
        int iCurrentBestScoreIndex = -1;

        // Go thorugh is child
        for (int i = 0; i < currentNode.childNodes.size(); i++)
        {
            // Compute its UCB score
            float iScore = this->getUCBScore( * currentNode.childNodes.at(i));

            // If its score is higher than any other previously seen
            if (iScore > iBestScore)
            {
                // Update the values
                iBestScore = iScore;
                iCurrentBestScoreIndex = i;
            }
        }
        #ifdef DEBUG_FLAG_MCTS
            Debug("Selecting child node: " << iCurrentBestScoreIndex);
        #endif
        
        // Select the node with the highest found UCB score
        // The fFinalScore is the score returned by the leaf nodes / child nodes in the tree that have been rolled out
        float fFinalScore = this->selectNode( * currentNode.childNodes.at(iCurrentBestScoreIndex));
        
        // Update the values of the node accordingly
        currentNode.iNumberOfVisits = currentNode.iNumberOfVisits + 1;
        currentNode.fTotalScore = currentNode.fTotalScore + fFinalScore;
        
        // Return or backpropagation
        return fFinalScore;
    }
}

// Returns a game state it has found as the most optimal
// Based on the given current game state and player colour and the number of simulated games
Game MonteCarloTreeSearch::getBestGameState(Game toAnalyseGame, Colour currentPlayerColour, int iNumberOfSimulations)
{
    // At the root of the tree will be the initial given game state
    this->rootNode = new Node(toAnalyseGame);
    
    this->playerColour = currentPlayerColour;
    
    this->iNumberOfTotalSimulations = 0;
    
    // For the given number of total simulations
    // traverse the tree from the root node
    for (; this->iNumberOfTotalSimulations < iNumberOfSimulations; this->iNumberOfTotalSimulations++)
    {
        this->selectNode( * this->rootNode );
    }
    
    // Once the tree has been expanded
    // Select a child node from the root node that maximised the UCB score
    float iBestScore = -1;
    int iCurrentBestScoreIndex = -1;

    for (int i = 0; i < this->rootNode->childNodes.size(); i++)
    {
        float iScore = this->getUCBScore( * this->rootNode->childNodes.at(i));

        if (iScore > iBestScore)
        {
            iBestScore = iScore;
            iCurrentBestScoreIndex = i;
        }
    }
    // Store the game state
    Game bestGame = this->rootNode->childNodes.at(iCurrentBestScoreIndex)->gameState;
    
    // delete the tree, and deallocate the allocated memory
    this->deleteTree(this->rootNode);
    
    // return the best found game state
    return bestGame;
}
