// -----------------------------------------------------------------------------
// ChessPuter
// main.cpp
// Miles Bright
// -----------------------------------------------------------------------------
#include "Computer.hpp"
#include <iostream>



// -----------------------------------------------------------------------------
//  main()
// The main method creates a Computer object, parses text commands from stdin,
// deletes extraneous whitespace, and passes the appropriate string parameters
// to their respective Computer class functions.
// -----------------------------------------------------------------------------
int main() {

    Computer puter;
    std::string input;
    bool running = true;
    while (running) {
        std::cin.clear();
        std::getline(std::cin, input);
        // Clear extraneous whitespace in command string
        while (input.at(0) == ' ')
            input = input.substr(1, input.length() - 1);
        for (int i = 0; i < input.length(); i++) {
            if (input.at(i) == ' ' && input.at(i - 1) == ' ') {
                input.erase(input.begin() + i);
                i--;
            }
        }
        // Put extra space at the end of the string for substr() and at()
        input = input + "                       ";
        if (input.substr(0, 3) == "uci" && input.find("newgame") == -1) {
            std::cout << "id name ChessPuter\nid author Miles Bright\n";
            // list changeable options here
            std::cout << "uciok\n";
        }
        if (input.substr(0, 7) == "isready") {
            std::cout << "readyok\n";
        }
        if (input.substr(0, 10) == "ucinewgame") {
            puter.ucinewgameCommand();
        }
        // position [fen <fenstring>|startpos] moves <moves>
        if (input.substr(0, 8) == "position") {
            puter.positionCommand(input.substr(9, input.length() - 9));
        }
        if (input.substr(0, 2) == "go") {
            puter.goCommand(input.substr(3, input.length() - 3));
            // send back bestmove <move> with puter
        }
        if (input.substr(0, 4) == "stop") {
            // need threads for this
            // must send some bestmove
            std::cout << "0000\n";
        }
        if (input.substr(0, 5) == "print") {
            puter.printBoard();
        }
        if (input.substr(0, 10) == "legalmoves") {
            puter.printLegalMoves();
        }
        if (input.substr(0, 4) == "quit") {
            running = false;
        }
    }

    return 0;
}
