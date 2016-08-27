// -----------------------------------------------------------------------------
// Chessputer
// Engine.cpp
// Miles Bright
// -----------------------------------------------------------------------------
#include "Engine.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>


Engine::Engine() {

    lastSquare[0] = 'x';
    lastSquare[1] = 'x';
    firstMove = true;
}


void Engine::runReceiveCommands() {

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
            continue;
        }
        if (input.substr(0, 7) == "isready") {
            std::cout << "readyok\n";
            continue;
        }
        if (input.substr(0, 10) == "ucinewgame") {
            rootPosition.ucinewgameCommand();
            continue;
        }
        // position [fen <fenstring>|startpos] moves <moves>
        if (input.substr(0, 8) == "position") {
            positionCommand(input.substr(9, input.length() - 9));
            continue;
        }
        if (input.substr(0, 2) == "go") {
            //std::thread searchThread(goCommand, input.substr(3, input.length() - 3));
            //searchThread.detach();
            goCommand(input.substr(3, input.length() - 3));
            // send back bestmove <move> with puter
            continue;
        }
        if (input.substr(0, 4) == "stop") {
            // need threads for this
            // must send some bestmove
            //std::cout << "0000\n";
            continue;
        }
        if (input.substr(0, 5) == "print") {
            rootPosition.printBoard();
            continue;
        }
        if (input.substr(0, 10) == "legalmoves") {
            rootPosition.printLegalMoves();
            continue;
        }
        if (input.substr(0, 4) == "quit") {
            running = false;
            // shutdown();
            continue;
        }
    }
}



// Engine::positionCommand()
// input: text after "position "
void Engine::positionCommand(std::string input) {

    // "position startpos moves e2e4 d7d5 e4d5 d8d5 b1c3                         "
    //           ^                                  ^
    // Compare input position string against last position command
    /*if (nextPositions.size() > 0) {
        for (int i = 0; i < positionString.size(); i++) {
            if (input.at(i) != positionString.at(i)) {
                updateEngineBoard(input.substr(i, input.length() - i));
            }
        }
        if (input == "startpos                       ")
            positionString = "startpos moves                       ";
        else
            positionString = input;
    }
    else {*/
        // Reset board information for the new position
        //positionString = input;
        if (input.substr(0, 8) == "startpos")
            rootPosition.startPosition(input.substr(9, input.length() - 9));
        if (input.substr(0, 3) == "fen")
            rootPosition.setupFEN(input.substr(4, input.length() - 4));
    //}
}



void Engine::goCommand(std::string input) {

    if (firstMove) {
        if (rootPosition.whiteToMove) {
            std::cout << "bestmove d2d4\n";
            firstMove = false;
            return;
        }
    }
    // build tree to two depths
    rootPosition.findLegalMoves();
    for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
        rootPosition.nextPositions.at(i).findLegalMoves();         // benchmark this vs findAllMoves
    }
    // get rid of illegal in-check moves from the list of candidates
    for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
        for (int j = 0; j < rootPosition.nextPositions.at(i).nextPositions.size(); j++) {
            rootPosition.nextPositions.at(i).nextPositions.at(j).kingOnBoard(rootPosition.whiteToMove, &rootPosition.nextPositions.at(i));
        }
    }
    std::vector<int> movesToDelete;
    for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
        if (rootPosition.whiteToMove && rootPosition.nextPositions.at(i).dynamicEvaluation < -9000) { // <<== define this constant
            movesToDelete.push_back(i);
            continue;
        }
        else if (!rootPosition.whiteToMove && rootPosition.nextPositions.at(i).dynamicEvaluation > 9000) {
            movesToDelete.push_back(i);
            continue;
        }
    }
    for (int a = 0; a < movesToDelete.size(); a++) {
        rootPosition.nextPositions.erase(rootPosition.nextPositions.begin() + movesToDelete.at(a));
        for (int b = 0; b < movesToDelete.size(); b++) {
            movesToDelete.at(b) --;
        }
    }
    movesToDelete.clear();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Analyze every Position in Depth 2 with a two ply eval for a 4 ply total evaluation
    for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
        for (int j = 0; j < rootPosition.nextPositions.at(i).nextPositions.size(); j++) {
                rootPosition.nextPositions.at(i).nextPositions.at(j).twoPlyEval();//threePlyEval();
        }
    }
    // Evaluate down to level 1
    for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
        rootPosition.nextPositions.at(i).dynamicEval();
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // penalty for moves leaving the same square as the last destination square
    for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
        if (rootPosition.nextPositions.at(i).lastMove[0] == lastSquare[0] && rootPosition.nextPositions.at(i).lastMove[1] == lastSquare[1]) {
            if (rootPosition.whiteToMove)
                rootPosition.nextPositions.at(i).dynamicEvaluation -= 48;
            else
                rootPosition.nextPositions.at(i).dynamicEvaluation += 48;
        }
    }
    sendBestMove();
}



void Engine::sendBestMove() {

    srand(time(NULL));
    int index = rand() % (rootPosition.nextPositions.size());
    Position *bestContinuation = &rootPosition.nextPositions.at(index);
    if (rootPosition.whiteToMove) {
        for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
            if (rootPosition.nextPositions.at(i).dynamicEvaluation > bestContinuation->dynamicEvaluation)
                bestContinuation = &rootPosition.nextPositions.at(i);
        }
    }
    else {
        for (int i = 0; i < rootPosition.nextPositions.size(); i++) {
            if (rootPosition.nextPositions.at(i).dynamicEvaluation < bestContinuation->dynamicEvaluation)
                bestContinuation = &rootPosition.nextPositions.at(i);
        }
    }
    std::cout << "bestmove " << bestContinuation->lastMove[0] << bestContinuation->lastMove[1] <<
        bestContinuation->lastMove[2] << bestContinuation->lastMove[3] << bestContinuation->promotionPiece << '\n';
    lastSquare[0] = bestContinuation->lastMove[2];
    lastSquare[1] = bestContinuation->lastMove[3];
    rootPosition.nextPositions.clear();
}


