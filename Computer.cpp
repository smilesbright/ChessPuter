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

}


void Computer::printBoard() {

    std::cout << "\n";
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j++) {
            std::cout << board.pieces[i][j] << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    if (board.whiteToMove)
        std::cout << "white to move\n";
    if (!board.whiteToMove)
        std::cout << "black to move\n";
    std::cout << board.enpassantFile << "\n" << board.whiteKcastlingRights << 
    board.whiteQcastlingRights << board.blackKcastlingRights << board.blackQcastlingRights << 
    "\n" << numberOfMovesSinceLastCaptureOrPawnMove << ' ' << gameMoveNumber << '\n';
}


void Computer::printLegalMoves() {

    board.findLegalMoves();
    for (int i = 0; i < board.nextPositions.size(); i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << board.nextPositions.at(i).lastMove[j];
        }
        std::cout << board.nextPositions.at(i).promotionPiece << '\n';
    }
    std::cout << board.dynamicEvaluation << "\n";
}



void Computer::ucinewgameCommand() {

    board.nextPositions.clear();
}



// -- public--------------------------------------------------------------------
// Computer::positionCommand()
// input: text after "position "
// -----------------------------------------------------------------------------
void Computer::positionCommand(std::string input) {
    
    // "position startpos moves e2e4 d7d5 e4d5 d8d5 b1c3                         "
    //           ^                                  ^
    // Compare input position string against last position command
    /*if (board.nextPositions.size() > 0) {
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

    /*for (int i = 0; i < board.nextPositions.size(); i++) {
        for (int j = 0; j < board.nextPositions.at(i).nextPositions.size(); j++) {
            if (move.at(0) == board.nextPositions.at(i).nextPositions.at(j).lastMove[0]
             && move.at(1) == board.nextPositions.at(i).nextPositions.at(j).lastMove[1]
             && move.at(2) == board.nextPositions.at(i).nextPositions.at(j).lastMove[2]
             && move.at(3) == board.nextPositions.at(i).nextPositions.at(j).lastMove[3]) {
                board = board.nextPositions.at(i).nextPositions.at(j);
                break;
            }
        }
    }
    board.nextPositions.clear();    // handle better later
*/
}


// -- private-------------------------------------------------------------------
// 
//
// -----------------------------------------------------------------------------
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
            board.pieces[i][j] = startPos[i][j];
    board.enpassantFile = -3;
    board.whiteToMove = true;
    board.whiteKcastlingRights = true;
    board.whiteQcastlingRights = true;
    board.blackKcastlingRights = true;
    board.blackQcastlingRights = true;
    numberOfMovesSinceLastCaptureOrPawnMove = 0;
    gameMoveNumber = 1;

    if (input.substr(0, 5) == "moves") {
        playMoves(input.substr(6, input.length() - 6));
    }

}


// -- private-------------------------------------------------------------------
// Computer::setupFEN()
//
// ex: rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq e3 0 1
// -----------------------------------------------------------------------------
void Computer::setupFEN(std::string input) {

    board.whiteKcastlingRights = false;
    board.whiteQcastlingRights = false;
    board.blackKcastlingRights = false;
    board.blackQcastlingRights = false;
    board.enpassantFile = -3;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            board.pieces[y][x] = '.';
    char character;
    int y = 0;
    int x = 0;
    int fenSpan;
    for (fenSpan = 0; fenSpan < 64; fenSpan++) {
        character = input.at(fenSpan);
        if (character == 'p') {
            board.pieces[y][x] = 'p';
            x ++;
            continue;
        }
        if (character == 'P') {
            board.pieces[y][x] = 'P';
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
            board.pieces[y][x] = 'r';
            x ++;
            continue;
        }
        if (character == 'n') {
            board.pieces[y][x] = 'n';
            x ++;
            continue;
        }
        if (character == 'b') {
            board.pieces[y][x] = 'b';
            x ++;
            continue;
        }
        if (character == 'q') {
            board.pieces[y][x] = 'q';
            x ++;
            continue;
        }
        if (character == 'k') {
            board.pieces[y][x] = 'k';
            x ++;
            continue;
        }
        if (character == 'R') {
            board.pieces[y][x] = 'R';
            x ++;
            continue;
        }
        if (character == 'N') {
            board.pieces[y][x] = 'N';
            x ++;
            continue;
        }
        if (character == 'B') {
            board.pieces[y][x] = 'B';
            x ++;
            continue;
        }
        if (character == 'Q') {
            board.pieces[y][x] = 'Q';
            x ++;
            continue;
        }
        if (character == 'K') {
            board.pieces[y][x] = 'K';
            x ++;
            continue;
        }
        if (character == ' ') {                                                                  
            break;
        }
        
    }
    setSideToMove(input.substr(fenSpan + 1, input.length() - fenSpan - 1));
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Computer::setSideToMove(std::string input) {

    if (input.at(0) == 'w')
        board.whiteToMove = true;
    if (input.at(0) == 'b')
        board.whiteToMove = false;
    setCastlingRights(input.substr(2, input.length() - 2));
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Computer::setCastlingRights(std::string input) {

    int fieldCounter = 0;
    for (int i = 0; i < 4; i++) {
        if (input.at(i) == 'K') {
            board.whiteKcastlingRights = true;
            fieldCounter ++;
        }
        if (input.at(i) == 'Q') {
            board.whiteQcastlingRights = true;
            fieldCounter ++;
        }
        if (input.at(i) == 'k') {
            board.blackKcastlingRights = true;
            fieldCounter ++;
        }
        if (input.at(i) == 'q') {
            board.blackQcastlingRights = true;
            fieldCounter ++;
        }
    }
    setEnpassantSquares(input.substr(fieldCounter + 1, input.length() - 1));
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Computer::setEnpassantSquares(std::string input) {

    char character = input.at(0);
    bool squareExists = false;
    if (character == 'a') {
        board.enpassantFile = 0;
        squareExists = true;
    }
    if (character == 'b') {
        board.enpassantFile = 1;
        squareExists = true;
    }
    if (character == 'c') {
        board.enpassantFile = 2;
        squareExists = true;
    }
    if (character == 'd') {
        board.enpassantFile = 3;
        squareExists = true;
    }
    if (character == 'e') {
        board.enpassantFile = 4;
        squareExists = true;
    }
    if (character == 'f') {
        board.enpassantFile = 5;
        squareExists = true;
    }
    if (character == 'g') {
        board.enpassantFile = 6;
        squareExists = true;
    }
    if (character == 'h') {
        board.enpassantFile = 7;
        squareExists = true;
    }
    if (squareExists) {
        setMoveNumbers(input.substr(3, input.length() - 3));
    }
    else
        (setMoveNumbers(input.substr(2, input.length() - 2)));
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
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


// -- private-------------------------------------------------------------------
// Computer::playMoves()
// Plays moves on the computer's internal chess board given in position command
// -----------------------------------------------------------------------------
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
    board.enpassantFile = -3;
    if (input.at(4) != ' ') {
        if (board.whiteToMove) {
            if (input.at(4) == 'q' || input.at(4) == 'Q')
                board.makeMove(y1, x1, y2, x2, 'Q');
            if (input.at(4) == 'r' || input.at(4) == 'R')
                board.makeMove(y1, x1, y2, x2, 'R');
            if (input.at(4) == 'n' || input.at(4) == 'N')
                board.makeMove(y1, x1, y2, x2, 'N');
            if (input.at(4) == 'b' || input.at(4) == 'B')
                board.makeMove(y1, x1, y2, x2, 'B');
            board.whiteToMove = false;
        }
        else {
            if (input.at(4) == 'q' || input.at(4) == 'Q')
                board.makeMove(y1, x1, y2, x2, 'q');
            if (input.at(4) == 'r' || input.at(4) == 'R')
                board.makeMove(y1, x1, y2, x2, 'r');
            if (input.at(4) == 'n' || input.at(4) == 'N')
                board.makeMove(y1, x1, y2, x2, 'n');
            if (input.at(4) == 'b' || input.at(4) == 'B')
                board.makeMove(y1, x1, y2, x2, 'b');
            board.whiteToMove = true;
        }
        
        playMoves(input.substr(6, input.length() - 6));
    }
    else {
        board.makeMove(y1, x1, y2, x2, ' ');
        if (board.whiteToMove)
            board.whiteToMove = false;
        else
            board.whiteToMove = true;
        playMoves(input.substr(5, input.length() - 5));
    }
}





////////////////////////////////////////////////////////////////////////////////
//////////////////////////      GO FUNCTIONS      //////////////////////////////
////////////////////////////////////////////////////////////////////////////////





// -- public--------------------------------------------------------------------
// Computer::goCommand()
// Incremental Development
// -----------------------------------------------------------------------------
void Computer::goCommand(std::string input) {

    // build tree to two depths
    board.findLegalMoves();
    for (int i = 0; i < board.nextPositions.size(); i++) {
        board.nextPositions.at(i).findLegalMoves();         // benchmark this vs findAllMoves
    }
    // get rid of illegal in-check moves, and possibly other garbage moves later on
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < board.nextPositions.size(); i++) {
        for (int j = 0; j < board.nextPositions.at(i).nextPositions.size(); j++) {
            board.nextPositions.at(i).nextPositions.at(j).staticEval(); // write completely different, shorter, faster function for this purpose
            // prune out garbage moves along with king moves - which are possible with just 2 plies obvs.
        }
    }
    std::vector<int> movesToDelete;
    for (int i = 0; i < board.nextPositions.size(); i++) {
        for (int j = 0; j < board.nextPositions.at(i).nextPositions.size(); j++) {
            if (board.whiteToMove && board.nextPositions.at(i).nextPositions.at(j).dynamicEvaluation < -5000) {
                movesToDelete.push_back(i);
                break;
            }
            else if (!board.whiteToMove && board.nextPositions.at(i).nextPositions.at(j).dynamicEvaluation > 5000) {
                movesToDelete.push_back(i);
                break;
            }
        }
    }
    for (int a = 0; a < movesToDelete.size(); a++) {
        board.nextPositions.erase(board.nextPositions.begin() + movesToDelete.at(a));
        for (int b = 0; b < movesToDelete.size(); b++) {
            movesToDelete.at(b) --;
        }
        //std::cout << movesToDelete.at(a) << "\n";
    }
    movesToDelete.clear();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Analyze every Position in Depth 2 with a two ply eval for a 4 ply total evaluation
    for (int i = 0; i < board.nextPositions.size(); i++) {
        for (int j = 0; j < board.nextPositions.at(i).nextPositions.size(); j++) {
                board.nextPositions.at(i).nextPositions.at(j).twoPlyEval();//threePlyEval();
        }
    }
    // Evaluate down to level 0
    for (int i = 0; i < board.nextPositions.size(); i++) {
        board.nextPositions.at(i).dynamicEval();
    }
    board.dynamicEval();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //usleep(88888);
    srand(time(NULL));
    int index = rand() % (board.nextPositions.size());
    Position *bestContinuation = &board.nextPositions.at(index);
    int bestEval = board.nextPositions.at(index).dynamicEvaluation;
    if (board.whiteToMove) {
        for (int i = 0; i < board.nextPositions.size(); i++) {
            if (board.nextPositions.at(i).dynamicEvaluation > bestEval) {
                bestEval = board.nextPositions.at(i).dynamicEvaluation;
                bestContinuation = &board.nextPositions.at(i);
            }
        }
    }
    else {
        for (int i = 0; i < board.nextPositions.size(); i++) {
            if (board.nextPositions.at(i).dynamicEvaluation < bestEval) {
                bestEval = board.nextPositions.at(i).dynamicEvaluation;
                bestContinuation = &board.nextPositions.at(i);
            }
        }
    }

    std::cout << "bestmove " << bestContinuation->lastMove[0] << bestContinuation->lastMove[1] << 
        bestContinuation->lastMove[2] << bestContinuation->lastMove[3] << bestContinuation->promotionPiece << '\n';
    board.nextPositions.clear();
    
}












