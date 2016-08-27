// -----------------------------------------------------------------------------
// ChessPuter
// Position.hpp
// Author: Miles Bright
// -----------------------------------------------------------------------------
#ifndef POSITION_HPP
#define POSITION_HPP

#include <vector>
#include <string>



class Position {


public:

    char pieces[8][8];      // use bitboards?    pointer arithmetic?
    bool whiteToMove;
    bool whiteQcastlingRights, whiteKcastlingRights;
    bool blackQcastlingRights, blackKcastlingRights;
    short enpassantFile;

public:

    char lastMove[4];
    char promotionPiece;
    double dynamicEvaluation;
    std::vector<Position> nextPositions;

    int gameMoveNumber;
    int numberOfMovesSinceLastCaptureOrPawnMove;

public:

    Position();
    Position(char[8][8], short, short, short, short, char, bool, bool, bool, bool, bool);
    void makeMove(int, int, int, int, char);
    void findLegalMoves();
    // other searchy and evaluaty functions
    // char* bestmove();
    void staticEval();
    void dynamicEval();
    void sortNextPositions();
    void onePlyEval();
    void twoPlyEval();
    void threePlyEval();
    void fourPlyEval();
    void positionEval();
    void kingOnBoard(bool, Position*);

    void findBestMove(/* time parameters */);


public: // root position setup, FEN string parsing, and commands

    void printBoard();
    void printLegalMoves();
    void ucinewgameCommand();

    void startPosition(std::string);
    void setupFEN(std::string);
    void setSideToMove(std::string);
    void setCastlingRights(std::string);
    void setEnpassantSquares(std::string);
    void setMoveNumbers(std::string);
    void playMoves(std::string);





private: // move making functions

    char integerToChar(short);

    void pawnMoves(int, int);
    void rookMoves(int, int);
    void knightMoves(int, int);
    void bishopMoves(int, int);
    void queenMoves(int, int);
    void kingMoves(int, int);
    // Legal move helper functions
    bool whiteLegalToKcastle();
    bool whiteLegalToQcastle();
    bool blackLegalToKcastle();
    bool blackLegalToQcastle();
    bool squareInBounds(int, int);
    bool emptySquare(int, int);
    bool enemyPiece(int, int);

    void movePiece(int, int, int, int);


};

#endif // POSITION_HPP
