// -----------------------------------------------------------------------------
// ChessPuter
// main.cpp
// Miles Bright
// -----------------------------------------------------------------------------
#include "Engine.hpp"
#include <iostream>
#include <thread>


// how to do threads??

int main() {

    Engine ChessPuter;
    ChessPuter.runReceiveCommands();
    return 0;
}

/* A reorganization of all the Position function implementations may look like:

Must design around the scheme of how threads work in cpp. How does shared memory work? etc
    Do I need to have global namespace whatever variables and functions for everything?
    How the hell does this bullshit work
Object-Oriented Programming!
  The position object - to represent pieces and boards.
Single responsibility principle...

Search.h or Sentinel.h (Position tree Iterator)

Position.h

Position_Setup.cpp
Position_MoveGen.cpp
Position_Analysis.cpp


Commands.h      --> with just a single function? naw, just fit this into the engine class...

Engine.h
    - one instantiation ; includes Position.h,

SearchInfo.h --> sent by reference to the Position analysis functions
    - one instantiation, a member of the Engine class

GameInfo.h


Square.h struct?

Move.h
    - 4 shorts for move coordinates. or 2 squares?






















*/
