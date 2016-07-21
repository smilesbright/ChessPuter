// -----------------------------------------------------------------------------
// ChessPuter
// Position.hpp
// Author: Miles Bright
// -----------------------------------------------------------------------------
#ifndef POSITION_HPP
#define POSITION_HPP

#include <vector>



class Position {


protected:    // accessible by Computer class

    char pieces[8][8];      // use bitboards?
    bool whiteToMove;
    bool whiteQcastlingRights, whiteKcastlingRights;
    bool blackQcastlingRights, blackKcastlingRights;
    short enpassantFile;

public:

    char lastMove[4];
    char promotionPiece;
    double dynamicEvaluation;    // prefer functions to stored variables?
    std::vector<Position> nextPositions;    // use a lighter container?

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
    void twoPlyEval();
    void threePlyEval();
    void fourPlyEval();
    void positionEval();
    void kingOnBoard(bool, Position*);


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
