// -----------------------------------------------------------------------------
// ChessPuter
// main.cpp
// Miles Bright
// -----------------------------------------------------------------------------
#include "Computer.hpp"
#include <iostream>
#include <thread>


int main() {

    std::cout << sizeof(Position) << std::endl;
    Computer ChessPuter;
    ChessPuter.runReceiveCommands();

    return 0;
}

/* A reorganization of all the Position function implementations may look like:

Position_Setup.cpp
Position_MoveGen.cpp
Position_Analysis.cpp

*/
