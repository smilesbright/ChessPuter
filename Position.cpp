// -----------------------------------------------------------------------------
// Position.cpp
// ChessPuter
// Author: Miles Bright
// -----------------------------------------------------------------------------
#include "Position.hpp"

#include <math.h>
#include <iostream>


Position::Position() {

    enpassantFile = -3;
    dynamicEvaluation = 0;
    promotionPiece = ' ';
}


Position::Position(char oldPieces[8][8], short y1, short x1, short y2, short x2, char promoPiece,
        bool wTm, bool WKC, bool WQC, bool BKC, bool BQC) {

    promotionPiece = promoPiece;
    enpassantFile = -3;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            pieces[y][x] = oldPieces[y][x];
    makeMove(y1, x1, y2, x2, promotionPiece);
    if (wTm)
        whiteToMove = false;
    if (!wTm)
        whiteToMove = true;
    whiteKcastlingRights = WKC;
    whiteQcastlingRights = WQC;
    blackKcastlingRights = BKC;
    blackQcastlingRights = BQC;
    dynamicEvaluation = 0;

    char c = 'a';
    for (int i = 0; i < 8; i++) {
        if (x1 == i)
            lastMove[0] = c;
        if (x2 == i)
            lastMove[2] = c;
        c++;
    }
    lastMove[1] = integerToChar(y1);
    lastMove[3] = integerToChar(y2);
}


char Position::integerToChar(short number) {

    if (number == 0)
        return '8';
    if (number == 1)
        return '7';
    if (number == 2)
        return '6';
    if (number == 3)
        return '5';
    if (number == 4)
        return '4';
    if (number == 5)
        return '3';
    if (number == 6)
        return '2';
    if (number == 7)
        return '1';
    return '0';     // to avoid warnings with clang
}



// Guaranteed legal moves
void Position::makeMove(int y1, int x1, int y2, int x2, char promoPiece) {
    
    if (promoPiece != ' ') {
        pieces[y2][x2] = promoPiece;
        pieces[y1][x1] = '.';
    }
    else {
        // check if enpassant capture
        if (pieces[y1][x1] == 'P' && x1 != x2 && pieces[y2][x2] == '.') {
            pieces[y2][x2] = pieces[y1][x1];
            pieces[y1][x1] = '.';
            pieces[y2 + 1][x2] = '.';
        }
        else if (pieces[y1][x1] == 'p' && x1 != x2 && pieces[y2][x2] == '.') {
            pieces[y2][x2] = pieces[y1][x1];
            pieces[y1][x1] = '.';
            pieces[y2 - 1][x2] = '.';
        }
        // check if double pawn move from home rank
        else if (pieces[y1][x1] == 'P' && y1 == 6 && y2 == 4) {
            enpassantFile = x1;
            pieces[y2][x2] = pieces[y1][x1];
            pieces[y1][x1] = '.';
        }
        else if (pieces[y1][x1] == 'p' && y1 == 1 && y2 == 3) {
            enpassantFile = x1;
            pieces[y2][x2] = pieces[y1][x1];
            pieces[y1][x1] = '.';
        }
        // check if king move
        else if (pieces[y1][x1] == 'K') {
            if (x1 == 4 && x2 == 6) {
                pieces[7][6] = 'K';
                pieces[7][5] = 'R';
                pieces[7][4] = '.';
                pieces[7][7] = '.';
            }
            else if (x1 == 4 && x2 == 2) {
                pieces[7][2] = 'K';
                pieces[7][3] = 'R';
                pieces[7][4] = '.';
                pieces[7][0] = '.';
            }
            else {
                pieces[y2][x2] = 'K';
                pieces[y1][x1] = '.';
            }
            whiteKcastlingRights = false;
            whiteQcastlingRights = false;
        }
        else if (pieces[y1][x1] == 'k') {
            if (x1 == 4 && x2 == 6) {
                pieces[0][6] = 'k';
                pieces[0][5] = 'r';
                pieces[0][4] = '.';
                pieces[0][7] = '.';
            }
            else if (x1 == 4 && x2 == 2) {
                pieces[0][2] = 'k';
                pieces[0][3] = 'r';
                pieces[0][4] = '.';
                pieces[0][0] = '.';
            }
            else {
                pieces[y2][x2] = 'k';
                pieces[y1][x1] = '.';
            }
            blackKcastlingRights = false;
            blackQcastlingRights = false;
        }
        // check if rook move disabling castling
        else if (pieces[y1][x1] == 'R') {
            if (y1 == 7 && x1 == 7)
                whiteKcastlingRights = false;
            if (y1 == 7 && x1 == 0)
                whiteQcastlingRights = false;
            pieces[y2][x2] = pieces[y1][x1];
            pieces[y1][x1] = '.';
        }
        else if (pieces[y1][x1] == 'r') {
            if (y1 == 0 && x1 == 7)
                blackKcastlingRights = false;
            if (y1 == 0 && x1 == 0)
                blackQcastlingRights = false;
            pieces[y2][x2] = pieces[y1][x1];
            pieces[y1][x1] = '.';
        }
        else {
            pieces[y2][x2] = pieces[y1][x1];
            pieces[y1][x1] = '.';
        }
    }
}



// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::findLegalMoves() {
    
    if (whiteToMove) {
        for (int y = 0; y < 8; y++) {       // loop through piece * list instead?
            for (int x = 0; x < 8; x++) {
                if (pieces[y][x] == '.') {
                    continue;
                }
                if (pieces[y][x] == 'P') {
                    pawnMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'R') {
                    rookMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'N') {
                    knightMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'B') {
                    bishopMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'Q') {
                    queenMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'K') {
                    kingMoves(y, x);
                }
            }
        }
    }
    else {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (pieces[y][x] == '.') {
                    continue;
                }
                if (pieces[y][x] == 'p') {
                    pawnMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'r') {
                    rookMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'n') {
                    knightMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'b') {
                    bishopMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'q') {
                    queenMoves(y, x);
                    continue;
                }
                if (pieces[y][x] == 'k') {
                    kingMoves(y, x);
                }
            }
        }
    }
}



// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::pawnMoves(int y, int x) {

    if (whiteToMove) {
        if (emptySquare(y - 1, x)) {
            movePiece(y, x, y - 1, x);
            if (y == 6 && emptySquare(y - 2, x))
                movePiece(y, x, y - 2, x);
        }
        if (x != 0 && enemyPiece(y - 1, x - 1))
            movePiece(y, x, y - 1, x - 1);
        if (x != 7 && enemyPiece(y - 1, x + 1))
            movePiece(y, x, y - 1, x + 1);
        if (y == 3) {       // can poss capture enpassant
            if (x - 1 == enpassantFile)
                movePiece(3, x, 2, x - 1);
            if (x + 1 == enpassantFile)
                movePiece(3, x, 2, x + 1);
        }
    }
    else {
        if (emptySquare(y + 1, x)) {
            movePiece(y, x, y + 1, x);
            if (y == 1 && emptySquare(y + 2, x))
                movePiece(y, x, y + 2, x);
        }
        if (x != 0 && enemyPiece(y + 1, x - 1))
            movePiece(y, x, y + 1, x - 1);
        if (x != 7 && enemyPiece(y + 1, x + 1))
            movePiece(y, x, y + 1, x + 1);
        if (y == 4) {       // can poss capture enpassant
            if (x - 1 == enpassantFile)
                movePiece(4, x, 2, x - 1);
            if (x + 1 == enpassantFile)
                movePiece(4, x, 2, x + 1);
        }
    }
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::rookMoves(int y, int x) {

    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y + squareCounter, x)) {
            movePiece(y, x, y + squareCounter, x);
            continue;
        }
        else if (enemyPiece(y + squareCounter, x)) {
            movePiece(y, x, y + squareCounter, x);
            break;
        }
        else {
            break;
        }
    }
    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y - squareCounter, x)) {
            movePiece(y, x, y - squareCounter, x);
            continue;
        }
        else if (enemyPiece(y - squareCounter, x)) {
            movePiece(y, x, y - squareCounter, x);
            break;
        }
        else {
            break;
        }
    }
    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y, x - squareCounter)) {
            movePiece(y, x, y, x - squareCounter);
            continue;
        }
        else if (enemyPiece(y, x - squareCounter)) {
            movePiece(y, x, y, x - squareCounter);
            break;
        }
        else {
            break;
        }
    }
    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y, x + squareCounter)) {
            movePiece(y, x, y, x + squareCounter);
            continue;
        }
        else if (enemyPiece(y, x + squareCounter)) {
            movePiece(y, x, y, x + squareCounter);
            break;
        }
        else {
            break;
        }
    }
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::knightMoves(int y, int x) {

    if (enemyPiece(y + 1, x + 2) || emptySquare(y + 1, x + 2))
        movePiece(y, x, y + 1, x + 2);
    if (enemyPiece(y + 1, x - 2) || emptySquare(y + 1, x - 2))
        movePiece(y, x, y + 1, x - 2);
    if (enemyPiece(y - 1, x + 2) || emptySquare(y - 1, x + 2))
        movePiece(y, x, y - 1, x + 2);
    if (enemyPiece(y - 1, x - 2) || emptySquare(y - 1, x - 2))
        movePiece(y, x, y - 1, x - 2);
    if (enemyPiece(y + 2, x + 1) || emptySquare(y + 2, x + 1))
        movePiece(y, x, y + 2, x + 1);
    if (enemyPiece(y + 2, x - 1) || emptySquare(y + 2, x - 1))
        movePiece(y, x, y + 2, x - 1);
    if (enemyPiece(y - 2, x + 1) || emptySquare(y - 2, x + 1))
        movePiece(y, x, y - 2, x + 1);
    if (enemyPiece(y - 2, x - 1) || emptySquare(y - 2, x - 1))
        movePiece(y, x, y - 2, x - 1);
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::bishopMoves(int y, int x) {

    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y + squareCounter, x + squareCounter)) {
            movePiece(y, x, y + squareCounter, x + squareCounter);
            continue;
        }
        else if (enemyPiece(y + squareCounter, x + squareCounter)) {
            movePiece(y, x, y + squareCounter, x + squareCounter);
            break;
        }
        else {
            break;
        }
    }
    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y - squareCounter, x + squareCounter)) {
            movePiece(y, x, y - squareCounter, x + squareCounter);
            continue;
        }
        else if (enemyPiece(y - squareCounter, x + squareCounter)) {
            movePiece(y, x, y - squareCounter, x + squareCounter);
            break;
        }
        else {
            break;
        }
    }
    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y + squareCounter, x - squareCounter)) {
            movePiece(y, x, y + squareCounter, x - squareCounter);
            continue;
        }
        else if (enemyPiece(y + squareCounter, x - squareCounter)) {
            movePiece(y, x, y + squareCounter, x - squareCounter);
            break;
        }
        else {
            break;
        }
    }
    for (int squareCounter = 1; squareCounter < 8; squareCounter++) {
        if (emptySquare(y - squareCounter, x - squareCounter)) {
            movePiece(y, x, y - squareCounter, x - squareCounter);
            continue;
        }
        else if (enemyPiece(y - squareCounter, x - squareCounter)) {
            movePiece(y, x, y - squareCounter, x - squareCounter);
            break;
        }
        else {
            break;
        }
    }
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::queenMoves(int y, int x) {

    rookMoves(y, x);
    bishopMoves(y, x);
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::kingMoves(int y, int x) {

    if (emptySquare(y + 1, x) || enemyPiece(y + 1, x))
        movePiece(y, x, y + 1, x);
    if (emptySquare(y - 1, x) || enemyPiece(y - 1, x))
        movePiece(y, x, y - 1, x);
    if (emptySquare(y, x + 1) || enemyPiece(y, x + 1))
        movePiece(y, x, y, x + 1);
    if (emptySquare(y, x - 1) || enemyPiece(y, x - 1))
        movePiece(y, x, y, x - 1);
    if (emptySquare(y + 1, x + 1) || enemyPiece(y + 1, x + 1))
        movePiece(y, x, y + 1, x + 1);
    if (emptySquare(y + 1, x - 1) || enemyPiece(y + 1, x - 1))
        movePiece(y, x, y + 1, x - 1);
    if (emptySquare(y - 1, x + 1) || enemyPiece(y - 1, x + 1))
        movePiece(y, x, y - 1, x + 1);
    if (emptySquare(y - 1, x - 1) || enemyPiece(y - 1, x - 1))
        movePiece(y, x, y - 1, x - 1);
    if (whiteToMove && whiteKcastlingRights && whiteLegalToKcastle())
        movePiece(7, 4, 7, 6);
    if (whiteToMove && whiteQcastlingRights && whiteLegalToQcastle())
        movePiece(7, 4, 7, 2);
    if (!whiteToMove && blackKcastlingRights && blackLegalToKcastle())
        movePiece(0, 4, 0, 6);
    if (!whiteToMove && blackQcastlingRights && blackLegalToQcastle())
        movePiece(0, 4, 0, 2);
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
bool Position::whiteLegalToKcastle() {

    if (pieces[7][5] != '.' || pieces[7][6] != '.')
        return false;
    if (pieces[6][2] == 'n' || pieces[6][3] == 'n' || pieces[6][4] == 'n' || pieces[6][6] == 'n' || pieces[6][7] == 'n'
     || pieces[5][3] == 'n' || pieces[5][4] == 'n' || pieces[5][5] == 'n' || pieces[5][6] == 'n' || pieces[5][7] == 'n')
        return false;
    if (pieces[6][3] == 'p' || pieces[6][4] == 'p' || pieces[6][5] == 'p' || pieces[6][6] == 'p' || pieces[6][7] == 'p')
        return false;
    if (pieces[6][6] == 'k' || pieces[6][7] == 'k')
        return false;
    if (pieces[7][3] == 'r' || pieces[6][4] == 'r' || pieces[6][5] == 'r' || pieces[6][6] == 'r' || pieces[6][3] == 'b'
     || pieces[6][4] == 'b' || pieces[6][5] == 'b' || pieces[6][6] == 'b' || pieces[6][7] == 'b' || pieces[7][3] == 'q'
     || pieces[6][3] == 'q' || pieces[6][4] == 'q' || pieces[6][5] == 'q' || pieces[6][6] == 'q' || pieces[6][7] == 'q')
        return false;
    int y;  // spans board up and down
    for (y = 6; y != 0; y--) {      // search e file for attacking rooks or queens
        if (pieces[y][4] != '.')
            break;
        else if (pieces[y - 1][4] == 'r' || pieces[y - 1][4] == 'q')
            return false;
    }
    for (y = 6; y != 0; y--) {      // search f file for attacking rooks or queens
        if (pieces[y][5] != '.')
            break;
        else if (pieces[y - 1][5] == 'r' || pieces[y - 1][5] == 'q')
            return false;
    }
    for (y = 6; y != 0; y--) {      // search g file for attacking rooks or queens
        if (pieces[y][6] != '.')
            break;
        else if (pieces[y - 1][6] == 'r' || pieces[y - 1][6] == 'q')
            return false;
    }
    int x;  // spans board left and right
    for (x = 3; x != 0; x--) {      // search home rank for rooks or queens to left of king 
        if (pieces[7][x] != '.')
            break;
        else if (pieces[7][x - 1] == 'r' || pieces[7][x - 1] == 'q')
            return false;
    }
    x = 3;
    for (y = 6; y != 3; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x - 1] == 'b' || pieces[y - 1][x - 1] == 'q')
            return false;
        x--;
    }
    x = 4;
    for (y = 6; y != 2; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x - 1] == 'b' || pieces[y - 1][x - 1] == 'q')
            return false;
        x--;
    }
    x = 5;
    for (y = 6; y != 1; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x - 1] == 'b' || pieces[y - 1][x - 1] == 'q')
            return false;
        x--;
    }
    x = 5;
    for (y = 6; y != 4; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x + 1] == 'b' || pieces[y - 1][x + 1] == 'q')
            return false;
        x++;
    }
    if (pieces[6][6] == '.') {
        if (pieces[5][7] == 'b' || pieces[5][7] == 'q')
            return false;
    }
    return true;
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
bool Position::whiteLegalToQcastle() {

    if (pieces[7][1] != '.' || pieces[7][2] != '.' || pieces[7][3] != '.' || pieces[5][5] == 'n')
        return false;
    if (pieces[6][0] == 'n' || pieces[6][1] == 'n' || pieces[6][2] == 'n' || pieces[6][4] == 'n' || pieces[6][5] == 'n'
     || pieces[6][6] == 'n' || pieces[5][1] == 'n' || pieces[5][2] == 'n' || pieces[5][3] == 'n' || pieces[5][4] == 'n')
        return false;
    if (pieces[6][1] == 'p' || pieces[6][2] == 'p' || pieces[6][3] == 'p' || pieces[6][4] == 'p' || pieces[6][5] == 'p')
        return false;
    if (pieces[6][1] == 'k' || pieces[6][2] == 'k')
        return false;
    if (pieces[7][5] == 'r' || pieces[6][2] == 'r' || pieces[6][3] == 'r' || pieces[6][4] == 'r' || pieces[6][1] == 'b'
     || pieces[6][2] == 'b' || pieces[6][3] == 'b' || pieces[6][4] == 'b' || pieces[6][5] == 'b' || pieces[7][3] == 'q'
     || pieces[6][1] == 'q' || pieces[6][2] == 'q' || pieces[6][3] == 'q' || pieces[6][4] == 'q' || pieces[6][5] == 'q')
        return false;
    int y;  // spans board up and down
    for (y = 6; y != 0; y--) {      // search e file for attacking rooks or queens
        if (pieces[y][4] != '.')
            break;
        else if (pieces[y - 1][4] == 'r' || pieces[y - 1][4] == 'q')
            return false;
    }
    for (y = 6; y != 0; y--) {      // search d file for attacking rooks or queens
        if (pieces[y][3] != '.')
            break;
        else if (pieces[y - 1][3] == 'r' || pieces[y - 1][3] == 'q')
            return false;
    }
    for (y = 6; y != 0; y--) {      // search c file for attacking rooks or queens
        if (pieces[y][2] != '.')
            break;
        else if (pieces[y - 1][2] == 'r' || pieces[y - 1][2] == 'q')
            return false;
    }
    int x;  // spans board left and right
    for (x = 5; x != 7; x++) {      // search home rank for rooks or queens to right of king 
        if (pieces[7][x] != '.')
            break;
        else if (pieces[7][x + 1] == 'r' || pieces[7][x + 1] == 'q')
            return false;
    }
    x = 3;
    for (y = 6; y != 3; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x - 1] == 'b' || pieces[y - 1][x - 1] == 'q')
            return false;
        x--;
    }
    x = 2;
    for (y = 6; y != 4; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x - 1] == 'b' || pieces[y - 1][x - 1] == 'q')
            return false;
        x--;
    }
    if (pieces[6][1] == '.') {
        if (pieces[5][0] == 'b' || pieces[5][0] == 'q')
            return false;
    }
    x = 5;
    for (y = 6; y != 4; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x + 1] == 'b' || pieces[y - 1][x + 1] == 'q')
            return false;
        x++;
    }
    x = 4;
    for (y = 6; y != 3; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x + 1] == 'b' || pieces[y - 1][x + 1] == 'q')
            return false;
        x++;
    }
    x = 3;
    for (y = 6; y != 2; y--) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y - 1][x + 1] == 'b' || pieces[y - 1][x + 1] == 'q')
            return false;
        x++;
    }
    return true;
}



// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
bool Position::blackLegalToKcastle() {

    if (pieces[0][5] != '.' || pieces[0][6] != '.')
        return false;
    if (pieces[1][2] == 'N' || pieces[1][3] == 'N' || pieces[1][4] == 'N' || pieces[1][6] == 'N' || pieces[1][7] == 'N'
     || pieces[2][3] == 'N' || pieces[2][4] == 'N' || pieces[2][5] == 'N' || pieces[2][6] == 'N' || pieces[2][7] == 'N')
        return false;
    if (pieces[1][3] == 'P' || pieces[1][4] == 'P' || pieces[1][5] == 'P' || pieces[1][6] == 'P' || pieces[1][7] == 'P')
        return false;
    if (pieces[1][1] == 'K' || pieces[1][7] == 'K')
        return false;
    if (pieces[0][3] == 'R' || pieces[1][4] == 'R' || pieces[1][5] == 'R' || pieces[1][6] == 'R' || pieces[1][3] == 'B'
     || pieces[1][4] == 'B' || pieces[1][5] == 'B' || pieces[1][6] == 'B' || pieces[1][7] == 'B' || pieces[0][3] == 'Q'
     || pieces[1][3] == 'Q' || pieces[1][4] == 'Q' || pieces[1][5] == 'Q' || pieces[1][6] == 'Q' || pieces[1][7] == 'Q')
        return false;
    int y;  // spans board up and down
    for (y = 1; y != 7; y++) {      // search e file for attacking rooks or queens
        if (pieces[y][4] != '.')
            break;
        else if (pieces[y + 1][4] == 'R' || pieces[y + 1][4] == 'Q')
            return false;
    }
    for (y = 1; y != 7; y++) {      // search f file for attacking rooks or queens
        if (pieces[y][5] != '.')
            break;
        else if (pieces[y + 1][5] == 'R' || pieces[y + 1][5] == 'Q')
            return false;
    }
    for (y = 1; y != 7; y++) {      // search g file for attacking rooks or queens
        if (pieces[y][6] != '.')
            break;
        else if (pieces[y + 1][6] == 'R' || pieces[y + 1][6] == 'Q')
            return false;
    }
    int x;  // spans board left and right
    for (x = 3; x != 0; x--) {      // search home rank for rooks or queens to left of king 
        if (pieces[0][x] != '.')
            break;
        else if (pieces[0][x - 1] == 'R' || pieces[0][x - 1] == 'Q')
            return false;
    }
    x = 3;
    for (y = 1; y != 4; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x - 1] == 'B' || pieces[y + 1][x - 1] == 'Q')
            return false;
        x--;
    }
    x = 4;
    for (y = 1; y != 5; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x - 1] == 'B' || pieces[y + 1][x - 1] == 'Q')
            return false;
        x--;
    }
    x = 5;
    for (y = 1; y != 6; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x - 1] == 'B' || pieces[y + 1][x - 1] == 'Q')
            return false;
        x--;
    }
    x = 5;
    for (y = 1; y != 3; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x + 1] == 'B' || pieces[y + 1][x + 1] == 'Q')
            return false;
        x++;
    }
    if (pieces[1][6] == '.') {
        if (pieces[2][7] == 'B' || pieces[2][7] == 'Q')
            return false;
    }
    return true;
}



// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
bool Position::blackLegalToQcastle() {

    if (pieces[0][1] != '.' || pieces[0][2] != '.' || pieces[0][3] != '.' || pieces[2][5] == 'N')
        return false;
    if (pieces[1][0] == 'N' || pieces[1][1] == 'N' || pieces[1][2] == 'N' || pieces[1][4] == 'N' || pieces[1][5] == 'N'
     || pieces[1][6] == 'N' || pieces[2][1] == 'N' || pieces[2][2] == 'N' || pieces[2][3] == 'N' || pieces[2][4] == 'N')
        return false;
    if (pieces[1][1] == 'P' || pieces[1][2] == 'P' || pieces[1][3] == 'P' || pieces[1][4] == 'P' || pieces[1][5] == 'P')
        return false;
    if (pieces[1][1] == 'K' || pieces[1][2] == 'K')
        return false;
    if (pieces[0][5] == 'R' || pieces[1][2] == 'R' || pieces[1][3] == 'R' || pieces[1][4] == 'R' || pieces[1][1] == 'B'
     || pieces[1][2] == 'B' || pieces[1][3] == 'B' || pieces[1][4] == 'B' || pieces[1][5] == 'B' || pieces[0][3] == 'Q'
     || pieces[1][1] == 'Q' || pieces[1][2] == 'Q' || pieces[1][3] == 'Q' || pieces[1][4] == 'Q' || pieces[1][5] == 'Q')
        return false;
    int y;  // spans board up and down
    for (y = 1; y != 7; y++) {      // search e file for attacking rooks or queens
        if (pieces[y][4] != '.')
            break;
        else if (pieces[y + 1][4] == 'R' || pieces[y + 1][4] == 'Q')
            return false;
    }
    for (y = 1; y != 7; y++) {      // search d file for attacking rooks or queens
        if (pieces[y][3] != '.')
            break;
        else if (pieces[y + 1][3] == 'R' || pieces[y + 1][3] == 'Q')
            return false;
    }
    for (y = 1; y != 7; y++) {      // search c file for attacking rooks or queens
        if (pieces[y][2] != '.')
            break;
        else if (pieces[y + 1][2] == 'R' || pieces[y + 1][2] == 'Q')
            return false;
    }
    int x;  // spans board left and right
    for (x = 5; x != 7; x++) {      // search home rank for rooks or queens to right of king 
        if (pieces[0][x] != '.')
            break;
        else if (pieces[0][x + 1] == 'R' || pieces[0][x + 1] == 'Q')
            return false;
    }
    x = 3;
    for (y = 1; y != 4; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x - 1] == 'B' || pieces[y + 1][x - 1] == 'Q')
            return false;
        x--;
    }
    x = 2;
    for (y = 1; y != 3; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x - 1] == 'B' || pieces[y + 1][x - 1] == 'Q')
            return false;
        x--;
    }
    if (pieces[1][1] == '.') {
        if (pieces[2][0] == 'B' || pieces[2][0] == 'Q')
            return false;
    }
    x = 5;
    for (y = 1; y != 3; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x + 1] == 'B' || pieces[y + 1][x + 1] == 'Q')
            return false;
        x++;
    }
    x = 4;
    for (y = 1; y != 4; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x + 1] == 'B' || pieces[y + 1][x + 1] == 'Q')
            return false;
        x++;
    }
    x = 3;
    for (y = 1; y != 5; y++) {
        if (pieces[y][x] != '.')
            break;
        else if (pieces[y + 1][x + 1] == 'B' || pieces[y + 1][x + 1] == 'Q')
            return false;
        x++;
    }
    return true;
}




// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
bool Position::squareInBounds(int y, int x) {
    
    if (y > -1 && y < 8 && x > -1 && x < 8)
        return true;
    else
        return false;
}




// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
bool Position::emptySquare(int y, int x) {
    
    if (y < 0 || y > 7 || x < 0 || x > 7)
        return false;
    if (pieces[y][x] == '.')
        return true;
    else
        return false;
}


// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
bool Position::enemyPiece(int y, int x) {
    
    if (y < 0 || y > 7 || x < 0 || x > 7)
        return false;
    char target = pieces[y][x];
    if (whiteToMove) {
        if (target == 'p' || target == 'n' || target == 'b' || target == 'r' ||
            target == 'k' || target == 'q')
            return true;
        else
            return false;
    }
    else {
        if (target == 'P' || target == 'N' || target == 'B' || target == 'R' ||
            target == 'K' || target == 'Q')
            return true;
        else
            return false;
    }
    
}



// -- private-------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::movePiece(int y1, int x1, int y2, int x2) {

    if (pieces[y1][x1] == 'p' && y2 == 7) {
        Position queenPro(pieces,y1, x1, y2, x2, 'q', whiteToMove,
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(queenPro);
        Position rookPro(pieces, y1, x1, y2, x2, 'r', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(rookPro);
        Position bishopPro(pieces, y1, x1, y2, x2, 'b', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(bishopPro);
        Position knightPro(pieces, y1, x1, y2, x2, 'n', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(knightPro);
    }
    else if (pieces[y1][x1] == 'P' && y2 == 0) {
        Position queenPro(pieces, y1, x1, y2, x2, 'Q', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(queenPro);
        Position rookPro(pieces, y1, x1, y2, x2, 'R', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(rookPro);
        Position bishopPro(pieces, y1, x1, y2, x2, 'B', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(bishopPro);
        Position knightPro(pieces, y1, x1, y2, x2, 'N', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(knightPro);
    }
    else {
        Position newPos(pieces, y1, x1, y2, x2, ' ', whiteToMove, 
        whiteKcastlingRights, whiteQcastlingRights, blackKcastlingRights, blackQcastlingRights);
        nextPositions.push_back(newPos);
    }
}

// 24, 78, 78, 128, 236
// white/ black
// 996 / 972 = positive score (greater than 1) = 0.0024 * 400 = 0.96153
// 972 / 996 = negative score (less than 1)





// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::dynamicEval() {

    if (nextPositions.size() == 0)
        return;
    dynamicEvaluation = nextPositions.at(0).dynamicEvaluation;
    if (whiteToMove) {
        for (int i = 1; i < nextPositions.size(); i++) {
            if (nextPositions.at(i).dynamicEvaluation > dynamicEvaluation) {
                dynamicEvaluation = nextPositions.at(i).dynamicEvaluation;
            }
        }
    }
    else {
        for (int i = 1; i < nextPositions.size(); i++) {
            if (nextPositions.at(i).dynamicEvaluation < dynamicEvaluation) {
                dynamicEvaluation = nextPositions.at(i).dynamicEvaluation;
            }
        }
    }
}


// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::sortNextPositions() {

    Position copy;
    if (whiteToMove) {
        for (int i = 0; i < nextPositions.size(); i++) {
            for (int j = 0; j < (nextPositions.size() - i - 1); j++) {
                if (nextPositions.at(j).dynamicEvaluation < nextPositions.at(j + 1).dynamicEvaluation) {
                    copy = nextPositions.at(j + 1);
                    nextPositions.at(j + 1) = nextPositions.at(j);
                    nextPositions.at(j) = copy;
                }
            }
        }
    }
    else {
        for (int i = 0; i < nextPositions.size(); i++) {
            for (int j = 0; j < (nextPositions.size() - i - 1); j++) {
                if (nextPositions.at(j).dynamicEvaluation > nextPositions.at(j + 1).dynamicEvaluation) {
                    copy = nextPositions.at(j + 1);
                    nextPositions.at(j + 1) = nextPositions.at(j);
                    nextPositions.at(j) = copy;
                }
            }
        }
    }
}


// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::twoPlyEval() {

    findLegalMoves();
    for (int i = 0; i < nextPositions.size(); i++) {
        nextPositions.at(i).findLegalMoves();
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            nextPositions.at(i).nextPositions.at(j).staticEval();
        }
    }
    for (int i = 0; i < nextPositions.size(); i++) {
        nextPositions.at(i).dynamicEval();
        if (whiteToMove) {
            nextPositions.at(i).dynamicEvaluation -= nextPositions.at(i).nextPositions.size();
        }
        else {
            nextPositions.at(i).dynamicEvaluation -= nextPositions.at(i).nextPositions.size();
        }
    }
    dynamicEval();
    nextPositions.clear();
}



// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::threePlyEval() {

    findLegalMoves();
    for (int i = 0; i < nextPositions.size(); i++) {
        nextPositions.at(i).findLegalMoves();
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            nextPositions.at(i).nextPositions.at(j).findLegalMoves();     // depth 3
            for (int k = 0; k < nextPositions.at(i).nextPositions.at(j).nextPositions.size(); k++) {
                nextPositions.at(i).nextPositions.at(j).nextPositions.at(k).staticEval();
            }
        }
    }
    for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            nextPositions.at(i).nextPositions.at(j).dynamicEval();        // depth 2
        }
        nextPositions.at(i).dynamicEval();        // depth 1
    }
    dynamicEval();
    nextPositions.clear();
}




// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::fourPlyEval() {

    findLegalMoves();
    for (int i = 0; i < nextPositions.size(); i++) {
        nextPositions.at(i).findLegalMoves();
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            nextPositions.at(i).nextPositions.at(j).findLegalMoves();     // depth 3
            for (int k = 0; k < nextPositions.at(i).nextPositions.at(j).nextPositions.size(); k++) {
                nextPositions.at(i).nextPositions.at(j).nextPositions.at(k).findLegalMoves();     // depth 4
                for (int l = 0; l < nextPositions.at(i).nextPositions.at(j).nextPositions.at(k).nextPositions.size(); l++) {
                    nextPositions.at(i).nextPositions.at(j).nextPositions.at(k).nextPositions.at(l).staticEval();
                }
            }
        }
    }
    for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            for (int k = 0; k < nextPositions.at(i).nextPositions.at(j).nextPositions.size(); k++) {
                nextPositions.at(i).nextPositions.at(j).nextPositions.at(k).dynamicEval();        // depth 3
            }
            nextPositions.at(i).nextPositions.at(j).dynamicEval();        // depth 2
        }
        nextPositions.at(i).dynamicEval();        // depth 1
    }
    dynamicEval();
    nextPositions.clear();
}



// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::staticEval() { // (Position* nextPosition)

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (pieces[i][j] == '.') {      // piece lists or piece counts would speed this up 
                continue;
            }
            if (pieces[i][j] == 'p') {
                dynamicEvaluation -= 24;// - i;//25, 26, 27
                continue;
            }
            if (pieces[i][j] == 'P') {
                dynamicEvaluation += 24;// - i + 7;
                continue;
            }
            if (pieces[i][j] == 'r') {
                dynamicEvaluation -= 128;
                continue;
            }
            if (pieces[i][j] == 'n') {
                dynamicEvaluation -= 78;
                continue;
            }
            if (pieces[i][j] == 'b') {
                dynamicEvaluation -= 78;
                continue;
            }
            if (pieces[i][j] == 'q') {
                dynamicEvaluation -= 236;
                continue;
            }
            if (pieces[i][j] == 'k') {
                dynamicEvaluation -= 8000;
                continue;
            }
            if (pieces[i][j] == 'R') {
                dynamicEvaluation += 128;
                continue;
            }
            if (pieces[i][j] == 'N') {
                dynamicEvaluation += 78;
                continue;
            }
            if (pieces[i][j] == 'B') {
                dynamicEvaluation += 78;
                continue;
            }
            if (pieces[i][j] == 'Q') {
                dynamicEvaluation += 236;
                continue;
            }
            if (pieces[i][j] == 'K') {
                dynamicEvaluation += 8000;
                continue;
            }
        }
    }
    positionEval();
}



// -- public--------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void Position::positionEval() {

    if (pieces[3][3] == 'p')
        dynamicEvaluation -= 2;
    else if (pieces[3][3] == 'P')
        dynamicEvaluation += 2;
    if (pieces[3][4] == 'p')
        dynamicEvaluation -= 2;
    else if (pieces[3][4] == 'P')
        dynamicEvaluation += 2;
    if (pieces[4][3] == 'p')
        dynamicEvaluation -= 2;
    else if (pieces[4][3] == 'P')
        dynamicEvaluation += 2;
    if (pieces[4][4] == 'p')
        dynamicEvaluation -= 2;
    else if (pieces[4][4] == 'P')
        dynamicEvaluation += 2;
    if (pieces[7][6] == 'K')
        dynamicEvaluation += 18;
    if (pieces[0][6] == 'k')
        dynamicEvaluation -= 18;
    if (pieces[7][1] == 'N' || pieces[5][0] == 'N')
        dynamicEvaluation -= 3;
    if (pieces[7][6] == 'N' || pieces[5][7] == 'N')
        dynamicEvaluation -= 3;
    if (pieces[7][2] == 'B')
        dynamicEvaluation -= 2;
    if (pieces[7][5] == 'B')
        dynamicEvaluation -= 2;
    if (pieces[0][1] == 'n' || pieces[2][0] == 'n')
        dynamicEvaluation += 3;
    if (pieces[0][6] == 'n' || pieces[2][7] == 'n')
        dynamicEvaluation += 3;
    if (pieces[0][2] == 'b')
        dynamicEvaluation += 2;
    if (pieces[0][5] == 'b')
        dynamicEvaluation += 2;

}





































