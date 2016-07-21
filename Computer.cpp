// -----------------------------------------------------------------------------
// Chessputer
// Computer.cpp
// Miles Bright
// -----------------------------------------------------------------------------
#include "Computer.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>


Computer::Computer() {
    lastSquare[0] = 'x';
    lastSquare[1] = 'x';
}


void Computer::runReceiveCommands() {

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
            ucinewgameCommand();
        }
        // position [fen <fenstring>|startpos] moves <moves>
        if (input.substr(0, 8) == "position") {
            positionCommand(input.substr(9, input.length() - 9));
        }
        if (input.substr(0, 2) == "go") {
            goCommand(input.substr(3, input.length() - 3));
            // send back bestmove <move> with puter
        }
        if (input.substr(0, 4) == "stop") {
            // need threads for this
            // must send some bestmove
            //std::cout << "0000\n";
        }
        if (input.substr(0, 5) == "print") {
            printBoard();
        }
        if (input.substr(0, 10) == "legalmoves") {
            printLegalMoves();
        }
        if (input.substr(0, 4) == "quit") {
            running = false;
        }
    }
}



void Computer::printBoard() {

    std::cout << "\n";
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j++) {
            std::cout << pieces[i][j] << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    if (whiteToMove)
        std::cout << "white to move\n";
    if (!whiteToMove)
        std::cout << "black to move\n";
    std::cout << enpassantFile << "\n" << whiteKcastlingRights <<
    whiteQcastlingRights << blackKcastlingRights << blackQcastlingRights <<
    "\n" << numberOfMovesSinceLastCaptureOrPawnMove << ' ' << gameMoveNumber << '\n';
}



void Computer::printLegalMoves() {

    findLegalMoves();
    for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << nextPositions.at(i).lastMove[j];
        }
        std::cout << nextPositions.at(i).promotionPiece << '\n';
    }
    std::cout << dynamicEvaluation << "\n";
}



void Computer::ucinewgameCommand() {

    nextPositions.clear();
    moveList.clear();
    repeats.clear();
}



// Computer::positionCommand()
// input: text after "position "
void Computer::positionCommand(std::string input) {

    // "position startpos moves e2e4 d7d5 e4d5 d8d5 b1c3                         "
    //           ^                                  ^
    // Compare input position string against last position command
    /*if (nextPositions.size() > 0) {
        for (int i = 0; i < positionString.size(); i++) {
            if (input.at(i) != positionString.at(i)) {
                updateComputerBoard(input.substr(i, input.length() - i));
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
            startPosition(input.substr(9, input.length() - 9));
        if (input.substr(0, 3) == "fen")
            setupFEN(input.substr(4, input.length() - 4));
    //}
}



void Computer::updateComputerBoard(std::string move) {

    /*for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            if (move.at(0) == nextPositions.at(i).nextPositions.at(j).lastMove[0]
             && move.at(1) == nextPositions.at(i).nextPositions.at(j).lastMove[1]
             && move.at(2) == nextPositions.at(i).nextPositions.at(j).lastMove[2]
             && move.at(3) == nextPositions.at(i).nextPositions.at(j).lastMove[3]) {
                board = nextPositions.at(i).nextPositions.at(j);
                break;
            }
        }
    }
    nextPositions.clear();    // handle better later
*/
}



void Computer::startPosition(std::string input) {

    char startPos[8][8] =
//         0   1   2   3   4   5   6   7
        {{'r','n','b','q','k','b','n','r'},//  0

         {'p','p','p','p','p','p','p','p'},//  1

         {'.','.','.','.','.','.','.','.'},//  2

         {'.','.','.','.','.','.','.','.'},//  3

         {'.','.','.','.','.','.','.','.'},//  4

         {'.','.','.','.','.','.','.','.'},//  5

         {'P','P','P','P','P','P','P','P'},//  6

         {'R','N','B','Q','K','B','N','R'}};// 7

//         0   1   2   3   4   5   6   7

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            pieces[i][j] = startPos[i][j];
    enpassantFile = -3;
    whiteToMove = true;
    whiteKcastlingRights = true;
    whiteQcastlingRights = true;
    blackKcastlingRights = true;
    blackQcastlingRights = true;
    numberOfMovesSinceLastCaptureOrPawnMove = 0;
    gameMoveNumber = 1;

    if (input.substr(0, 5) == "moves") {
        playMoves(input.substr(6, input.length() - 6));
    }

}



// ex: rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq e3 0 1
//
void Computer::setupFEN(std::string input) {

    whiteKcastlingRights = false;
    whiteQcastlingRights = false;
    blackKcastlingRights = false;
    blackQcastlingRights = false;
    enpassantFile = -3;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            pieces[y][x] = '.';
    char character;
    int y = 0;
    int x = 0;
    int fenSpan;
    for (fenSpan = 0; fenSpan < 64; fenSpan++) {
        character = input.at(fenSpan);
        if (character == 'p') {
            pieces[y][x] = 'p';
            x ++;
            continue;
        }
        if (character == 'P') {
            pieces[y][x] = 'P';
            x ++;
            continue;
        }
        if (character == '1') {
            x += 1;
            continue;
        }
        if (character == '2') {
            x += 2;
            continue;
        }
        if (character == '3') {
            x += 3;
            continue;
        }
        if (character == '4') {
            x += 4;
            continue;
        }
        if (character == '5') {
            x += 5;
            continue;
        }
        if (character == '6') {
            x += 6;
            continue;
        }
        if (character == '7') {
            x += 7;
            continue;
        }
        if (character == '8') {
            x += 8;
            continue;
        }
        if (character == '/') {
            y ++;
            x = 0;
            continue;
        }
        if (character == 'r') {
            pieces[y][x] = 'r';
            x ++;
            continue;
        }
        if (character == 'n') {
            pieces[y][x] = 'n';
            x ++;
            continue;
        }
        if (character == 'b') {
            pieces[y][x] = 'b';
            x ++;
            continue;
        }
        if (character == 'q') {
            pieces[y][x] = 'q';
            x ++;
            continue;
        }
        if (character == 'k') {
            pieces[y][x] = 'k';
            x ++;
            continue;
        }
        if (character == 'R') {
            pieces[y][x] = 'R';
            x ++;
            continue;
        }
        if (character == 'N') {
            pieces[y][x] = 'N';
            x ++;
            continue;
        }
        if (character == 'B') {
            pieces[y][x] = 'B';
            x ++;
            continue;
        }
        if (character == 'Q') {
            pieces[y][x] = 'Q';
            x ++;
            continue;
        }
        if (character == 'K') {
            pieces[y][x] = 'K';
            x ++;
            continue;
        }
        if (character == ' ') {
            break;
        }

    }
    setSideToMove(input.substr(fenSpan + 1, input.length() - fenSpan - 1));
}



void Computer::setSideToMove(std::string input) {

    if (input.at(0) == 'w')
        whiteToMove = true;
    if (input.at(0) == 'b')
        whiteToMove = false;
    setCastlingRights(input.substr(2, input.length() - 2));
}



void Computer::setCastlingRights(std::string input) {

    int fieldCounter = 0;
    for (int i = 0; i < 4; i++) {
        if (input.at(i) == 'K') {
            whiteKcastlingRights = true;
            fieldCounter ++;
        }
        if (input.at(i) == 'Q') {
            whiteQcastlingRights = true;
            fieldCounter ++;
        }
        if (input.at(i) == 'k') {
            blackKcastlingRights = true;
            fieldCounter ++;
        }
        if (input.at(i) == 'q') {
            blackQcastlingRights = true;
            fieldCounter ++;
        }
    }
    setEnpassantSquares(input.substr(fieldCounter + 1, input.length() - 1));
}



void Computer::setEnpassantSquares(std::string input) {

    char character = input.at(0);
    bool squareExists = false;
    if (character == 'a') {
        enpassantFile = 0;
        squareExists = true;
    }
    if (character == 'b') {
        enpassantFile = 1;
        squareExists = true;
    }
    if (character == 'c') {
        enpassantFile = 2;
        squareExists = true;
    }
    if (character == 'd') {
        enpassantFile = 3;
        squareExists = true;
    }
    if (character == 'e') {
        enpassantFile = 4;
        squareExists = true;
    }
    if (character == 'f') {
        enpassantFile = 5;
        squareExists = true;
    }
    if (character == 'g') {
        enpassantFile = 6;
        squareExists = true;
    }
    if (character == 'h') {
        enpassantFile = 7;
        squareExists = true;
    }
    if (squareExists) {
        setMoveNumbers(input.substr(3, input.length() - 3));
    }
    else
        (setMoveNumbers(input.substr(2, input.length() - 2)));
}



void Computer::setMoveNumbers(std::string input) {

    char num = input.at(0);
    if (num == '1' || num == '2' || num == '3' || num == '4' || num == '5' ||
        num == '6' || num == '7' || num == '8' || num == '9' || num == '0') {
        numberOfMovesSinceLastCaptureOrPawnMove = std::stoi(input.substr(0, 2));
        input = input.substr(2, input.length() - 2);
    }
    if (input.at(0) == ' ')
        input = input.substr(1, input.length() - 1);
    num = input.at(0);
    if (num == '1' || num == '2' || num == '3' || num == '4' || num == '5' ||
        num == '6' || num == '7' || num == '8' || num == '9' || num == '0') {
        if (input.at(1) == ' ') {
            gameMoveNumber = std::stoi(input.substr(0, 1));
        }
        if (input.at(1) != ' ') {
            gameMoveNumber = std::stoi(input.substr(0, 2));
        }
        if (input.at(2) != 'm') {
            gameMoveNumber = std::stoi(input.substr(0, 3));
        }
    }
    int mLoc = input.find("moves");
    if (mLoc != -1) {
        playMoves(input.substr(mLoc + 6, input.length() - mLoc - 6));
    }

}



void Computer::playMoves(std::string input) {

    if (input.at(0) == ' ')
        return;
    int y1 = input.at(1) - '0';
    y1 = 8 - y1;
    int y2 = input.at(3) - '0';
    y2 = 8 - y2;
    int x1, x2;
    char target1 = input.at(0);
    char target2 = input.at(2);
    char comparator = 'a';
    for (int i = 0; i < 8; i++) {
        if (target1 == comparator)
            x1 = i;
        if (target2 == comparator)
            x2 = i;
        comparator ++;
    }
    enpassantFile = -3;
    if (input.at(4) != ' ') {
        if (whiteToMove) {
            if (input.at(4) == 'q' || input.at(4) == 'Q')
                makeMove(y1, x1, y2, x2, 'Q');
            if (input.at(4) == 'r' || input.at(4) == 'R')
                makeMove(y1, x1, y2, x2, 'R');
            if (input.at(4) == 'n' || input.at(4) == 'N')
                makeMove(y1, x1, y2, x2, 'N');
            if (input.at(4) == 'b' || input.at(4) == 'B')
                makeMove(y1, x1, y2, x2, 'B');
            whiteToMove = false;
        }
        else {
            if (input.at(4) == 'q' || input.at(4) == 'Q')
                makeMove(y1, x1, y2, x2, 'q');
            if (input.at(4) == 'r' || input.at(4) == 'R')
                makeMove(y1, x1, y2, x2, 'r');
            if (input.at(4) == 'n' || input.at(4) == 'N')
                makeMove(y1, x1, y2, x2, 'n');
            if (input.at(4) == 'b' || input.at(4) == 'B')
                makeMove(y1, x1, y2, x2, 'b');
            whiteToMove = true;
        }

        playMoves(input.substr(6, input.length() - 6));
    }
    else {
        makeMove(y1, x1, y2, x2, ' ');
        if (whiteToMove)
            whiteToMove = false;
        else
            whiteToMove = true;
        playMoves(input.substr(5, input.length() - 5));
    }
}



void Computer::goCommand(std::string input) {

    // build tree to two depths
    findLegalMoves();
    for (int i = 0; i < nextPositions.size(); i++) {
        nextPositions.at(i).findLegalMoves();         // benchmark this vs findAllMoves
    }
    // get rid of illegal in-check moves from the list of candidates
    for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            nextPositions.at(i).nextPositions.at(j).kingOnBoard(whiteToMove, &nextPositions.at(i));
        }
    }
    std::vector<int> movesToDelete;
    for (int i = 0; i < nextPositions.size(); i++) {
        if (whiteToMove && nextPositions.at(i).dynamicEvaluation < -9000) {
            movesToDelete.push_back(i);
            continue;
        }
        else if (!whiteToMove && nextPositions.at(i).dynamicEvaluation > 9000) {
            movesToDelete.push_back(i);
            continue;
        }
    }
    for (int a = 0; a < movesToDelete.size(); a++) {
        nextPositions.erase(nextPositions.begin() + movesToDelete.at(a));
        for (int b = 0; b < movesToDelete.size(); b++) {
            movesToDelete.at(b) --;
        }
    }
    movesToDelete.clear();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Analyze every Position in Depth 2 with a two ply eval for a 4 ply total evaluation
    for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
                nextPositions.at(i).nextPositions.at(j).twoPlyEval();//threePlyEval();
        }
    }
    // Evaluate down to level 1
    for (int i = 0; i < nextPositions.size(); i++) {
        nextPositions.at(i).dynamicEval();
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // penalty for moves leaving the same square as the last destination square
    for (int i = 0; i < nextPositions.size(); i++) {
        if (nextPositions.at(i).lastMove[0] == lastSquare[0] && nextPositions.at(i).lastMove[1] == lastSquare[1]) {
            if (whiteToMove)
                nextPositions.at(i).dynamicEvaluation -= 48;
            else
                nextPositions.at(i).dynamicEvaluation += 48;
        }
    }
    sendBestMove();
}



void Computer::sendBestMove() {

    srand(time(NULL));
    int index = rand() % (nextPositions.size());
    Position *bestContinuation = &nextPositions.at(index);
    if (whiteToMove) {
        for (int i = 0; i < nextPositions.size(); i++) {
            if (nextPositions.at(i).dynamicEvaluation > bestContinuation->dynamicEvaluation)
                bestContinuation = &nextPositions.at(i);
        }
    }
    else {
        for (int i = 0; i < nextPositions.size(); i++) {
            if (nextPositions.at(i).dynamicEvaluation < bestContinuation->dynamicEvaluation)
                bestContinuation = &nextPositions.at(i);
        }
    }
    std::cout << "bestmove " << bestContinuation->lastMove[0] << bestContinuation->lastMove[1] <<
        bestContinuation->lastMove[2] << bestContinuation->lastMove[3] << bestContinuation->promotionPiece << '\n';
    lastSquare[0] = bestContinuation->lastMove[2];
    lastSquare[1] = bestContinuation->lastMove[3];
    nextPositions.clear();
}
