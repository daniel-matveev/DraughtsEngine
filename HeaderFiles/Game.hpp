//
//  Game.hpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 09/10/2024.
//

#ifndef Game_hpp
#define Game_hpp

#include "Board.hpp"

#include <set>

class Game
{
private:
    // Board object
    Board gameBoard;
    
    // Stores a valid selected piece during a turn
    Piece selectedPiece;
    
    // Keeps track of who's turn it is
    Colour currentPlayerColour;
    
    // Stores the pieces that can be selected by the player during move
    std::set<Position> selectablePieces;
    
    // Map that stores the end positions as keys and the board permutation as values
    std::unordered_map< Position, Board > endPositionsToBoard;
    // Map that stores the filtered key-value pairs of the endPositionsToBoard map
    std::unordered_map< Position, Board > filteredEndPositionsToBoard;
    
    // Map that stores intermediate moves in multi jumping
    // Allows players to choose a path to take if multiple pieces can be captured
    std::unordered_map< Position, Board > intermediatePositionsToBoard;
    
    // For a selected piece it will generate the moves possible and filter through the endPositionsToBoard map
    void getValidMoves(bool bMultiJump);
    
    // Checks the next valid move for a piece given the piece position and a board state
    bool checkPiece(Position piecePosition, Board boardState, bool bMultiJump);
    
    // Checks the next valid move for a king piece given the piece position and a board state
    bool checkKing(Position piecePosition, Board boardState, bool bMultiJump);
    
    // Checks the left side of the piece for possible moves
    bool checkLeft(Position startPosition, Board boardState, int iDirection, bool bMultiJump);
    
    // Checks the right side of the piece for possible moves
    bool checkRight(Position startPosition, Board boardState, int iDirection, bool bMultiJump);
    
    // Populates the selectablePieces set if a piece can make a valid move
    // Filters through the pieces if jumps are possible
    void selectPieces();
    
    // Checks if for a given position, the piece at that position is the colour of the current player
    bool selectPiece(Position toSelectPosition);
    
    bool canCurrentlyJump(Board toCompareBoard);
    
    // Return a score relating how good the position is for a player
    int getEvaluation();
    
    float calculateEvaluation();
    
    // Swaps players after each turn
    void swapPlayers();
    
    // Initialises variables
    void initVariables();
public:
    // Constructor / Destructor
    Game();
    ~Game();
    
    // For movement
    // To select a piece
    bool select(Position toCheckPosition);
    
    // To move the selected piece to a valid position
    bool move(Position toCheckPosition);
    
    // To implement the threefold repetition rule
    // Checks the stack of the moves made throughout the game for 3 exact board configurations
    // Returns true if 3 boards are made
    bool checkDraw(std::vector<Board> allBoards);
    
    // Returns colour of the winning player
    Colour getWinner();
    
    // Returns the current player colour
    Colour getCurrentPlayerColour();
    
    Board getGameBoard();
    
    // Prints the board
    void printBoard();
    
    
    
    // Allows access to private methods from the Game class
    friend class Minimax;
    friend class AlphaBetaPruning;
    friend class MonteCarloTreeSearch;
};

#endif /* Game_hpp */
