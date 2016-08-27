// -----------------------------------------------------------------------------
// Position_Setup.cpp
// 
// Root position setup functions including FEN string parsing
//
// ChessPuter
// Author: Miles Bright
// -----------------------------------------------------------------------------
#include "Position.hpp"
#include <iostream>



Position::Position() {

    enpassantFile = -3;
    dynamicEvaluation = 0;
    promotionPiece = ' ';
}



void Position::printBoard() {

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



void Position::printLegalMoves() {

    findLegalMoves();
    for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << nextPositions.at(i).lastMove[j];
        }
        std::cout << nextPositions.at(i).promotionPiece << '\n';
    }
    std::cout << dynamicEvaluation << "\n";
}



void Position::ucinewgameCommand() {

    nextPositions.clear();
    //moveList.clear();
    //repeats.clear();
}



void Position::startPosition(std::string input) {

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
void Position::setupFEN(std::string input) {

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



void Position::setSideToMove(std::string input) {

    if (input.at(0) == 'w')
        whiteToMove = true;
    if (input.at(0) == 'b')
        whiteToMove = false;
    setCastlingRights(input.substr(2, input.length() - 2));
}



void Position::setCastlingRights(std::string input) {

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



void Position::setEnpassantSquares(std::string input) {

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



void Position::setMoveNumbers(std::string input) {

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



void Position::playMoves(std::string input) {

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










