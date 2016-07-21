// -----------------------------------------------------------------------------
// ChessPuter
// Computer.hpp
// Miles Bright
// -----------------------------------------------------------------------------
#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "Position.hpp"
#include <string>


class Computer: private Position {


private:    // chess game and analysis information

    int gameMoveNumber;
    int numberOfMovesSinceLastCaptureOrPawnMove;
    std::string positionString;

    std::vector<std::string> moveList;
    std::vector<std::string> repeats;

    // std::vector<Candidate> candidates;
    char lastSquare[2];


    const int PAWN_VALUE = 10;
    const int ROOK_VALUE = 50;
    const int KNIGHT_VALUE = 30;    // make these a struct and pass to eval function?
    const int BISHOP_VALUE = 30;
    const int QUEEN_VALUE = 90;
    const int KING_VALUE = 9000;

    int alpha; // best possible white score
    int beta; // best possible black score

    int confidence; // likelihood a current score is better than alpha or beta


public: // GUI / console communication functions - need communication handling class

    Computer();
    void runReceiveCommands();
    void printBoard();
    void printLegalMoves();
    void ucinewgameCommand();
    void positionCommand(std::string);
    void goCommand(std::string);            // just send parameters to a position function
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

    void sendBestMove();

};

#endif // COMPUTER_HPP
