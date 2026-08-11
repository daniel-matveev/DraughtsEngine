//
//  Game.hpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#ifndef Game_hpp
#define Game_hpp

#include "Board.hpp"

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
    
    // Stack to keep track of the piece that can be skipped when checking vor valid moves
    std::vector<Position> toSkipPositions;
    
    // Map that stores the end positions as keys and pieces that can be skipped as values
    std::unordered_map< Position, std::vector<Position> > endPositionsToBoard;
    // Map that stores the filtered key-value pairs of the endPositionsToBoard map
    std::unordered_map< Position, std::vector<Position> > filteredEndPositionsToBoard;
    
    // Map that stores intermediate moves in multi jumping
    // Allows players to choose a path to take if multiple pieces can be captured
    std::unordered_map< Position, std::vector<Position> > intermediatePositionsToBoard;
    
    // For a selected piece it will generate the moves possible and filter through the endPositionsToBoard map
    void getValidMoves(bool bMultiJump);
    
    // Checks the next valid move for a piece given the piece position and a board state
    bool checkPiece(Position piecePosition, Board boardState, bool bMultiJump);
    
    // Checks the next valid move for a king piece given the piece position and a board state
    bool checkKing(Position piecePosition, Board boardState, bool bMultiJump);
    
    // Checks the specified direction for possible moves
    bool checkDirection(Position startPosition,
                        Board boardState,
                        Position directionPosition,
                        bool bMultiJump);
    
    // Populates the selectablePieces set if a piece can make a valid move
    // Filters through the pieces if jumps are possible
    void selectPieces();
    
    // Checks if for a given position, the piece at that position is the colour of the current player
    bool selectPiece(Position toSelectPosition);
    
    bool canCurrentlyJump(std::vector<Position> toSkipPositions);
    
    // Return a score relating how good the position is for a player
    int getEvaluation();
    
    float calculateEvaluation();
    
    // Swaps players after each turn
    void swapPlayers();
    
    // Initialises variables
    void initVariables();
    
    // clear maps to prepare for new move
    void clear();
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

    // Selects a uniform random move from the set of selectable pieces and their valid moves
    bool playRandomMove(std::mt19937 &rng);
    
    // Prints the board
    void printBoard();
    
    // Allows access to private methods from the Game class
    friend class Minimax;
    friend class AlphaBetaPruning;
    friend class MonteCarloTreeSearch;
};

#endif /* Game_hpp */
