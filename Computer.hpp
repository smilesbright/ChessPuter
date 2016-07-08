// -----------------------------------------------------------------------------
// Chessputer
// Computer.hpp
// Miles Bright
// -----------------------------------------------------------------------------
#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "Position.hpp"
#include <string>


class Computer {


private: // Game and engine data

    Position board;

    int wtime, btime, winc, binc;
    int gameMoveNumber;
    int numberOfMovesSinceLastCaptureOrPawnMove;
    std::string positionString;



    const int PAWN_VALUE = 10;
    const int ROOK_VALUE = 50;
    const int KNIGHT_VALUE = 30;
    const int BISHOP_VALUE = 30;
    const int QUEEN_VALUE = 90;
    const int KING_VALUE = 9000;


public: // GUI / console communication functions

    Computer();
    void printBoard();
    void printLegalMoves();
    void ucinewgameCommand();
    void positionCommand(std::string);
    void goCommand(std::string);
    void stopCommand();


private: // Computer board setup functions

    void updateComputerBoard(std::string);
    void startPosition(std::string);
    void setupFEN(std::string);
    void setSideToMove(std::string);
    void setCastlingRights(std::string);
    void setEnpassantSquares(std::string);
    void setMoveNumbers(std::string);
    void playMoves(std::string);


};

#endif // COMPUTER_HPP
