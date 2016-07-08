// -----------------------------------------------------------------------------
// ChessPuter
// Position.hpp
// Author: Miles Bright
// -----------------------------------------------------------------------------
#ifndef POSITION_HPP
#define POSITION_HPP

#include <vector>



class Position {


public:

    char pieces[8][8];
    char lastMove[4];
    char promotionPiece;
    bool whiteToMove;
    bool whiteCanQcastle, whiteCanKcastle;
    bool blackCanQcastle, blackCanKcastle;
    // int whiteBishops, blackBishops, whitePieces, blackPieces;
    //char positionState;   // possibly use a separate class

    short enpassantFile;      // use one char instead of a short?
    int dynamicEvaluation;    // prefer functions to stored variables?
    std::vector<Position> nextPositions;


public:

    Position();
    Position(char[8][8], short, short, short, short, char, bool, bool, bool, bool, bool);
    void makeMove(int, int, int, int, char);
    void findLegalMoves();
    // other searchy and evaluatey functions
    // char* bestmove();
    void staticEval();
    void dynamicEval();
    void sortNextPositions(); 
    void threePlyEval();
    void fourPlyEval();


private: // move finding functions

    char integerToChar(short);

    void pawnMoves(int, int);
    void rookMoves(int, int);
    void knightMoves(int, int);
    void bishopMoves(int, int);
    void queenMoves(int, int);
    void kingMoves(int, int);
    // Legal move helper functions
    bool squareInBounds(int, int);
    bool emptySquare(int, int);
    bool enemyPiece(int, int);
    void movePiece(int, int, int, int);


};

#endif // POSITION_HPP
