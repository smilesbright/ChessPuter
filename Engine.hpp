// -----------------------------------------------------------------------------
// ChessPuter
// Computer.hpp
// Miles Bright
// -----------------------------------------------------------------------------
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Position.hpp"
#include <string>



struct GameInfo { // this is really position information

    int gameMoveNumber;
    int numberOfMovesSinceLastCaptureOrPawnMoves;
};


/*struct SearchInfo { // pass this to the Position's search functions on the go call

    int alpha;
    int beta;
    Position *currentNode;
    Position *bestLine;
    int nodes;
    int depth;
    std::string currentLine;

    // make this a class? add print functions or something
};*/



class Engine {


private:

    Position rootPosition;


    std::string positionString;

    std::vector<std::string> moveList;
    std::vector<std::string> repeats;

    char lastSquare[2];
    bool firstMove;


public:

    Engine();

    void runReceiveCommands();
    void positionCommand(std::string);
    void stopCommand();


private:

    void goCommand(std::string);
    void sendBestMove();

};

#endif // COMPUTER_HPP
