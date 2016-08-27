// -----------------------------------------------------------------------------
// Position.cpp
// ChessPuter
// Author: Miles Bright
// -----------------------------------------------------------------------------
#include "Position.hpp"

#include <math.h>
#include <iostream>




// 24, 78, 78, 128, 236
// white/ black
// 996 / 972 = positive score (greater than 1) = 0.0024 * 400 = 0.96153
// 972 / 996 = negative score (less than 1)

// -- private-------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Position::kingOnBoard(bool wTm, Position *home) {

    if (wTm) {
        for (int i = 7; i > -1; i--) {
            for (int j = 7; j > -1; j--) {
                if (pieces[i][j] == 'K') {
                    return;
                }
            }
        }
        home->dynamicEvaluation = -20000;
        return;
    }
    else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (pieces[i][j] == 'k') {
                    return;
                }
            }
        }
        home->dynamicEvaluation = 20000;
        return;
    }
}




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



void Position::onePlyEval() {

  findLegalMoves();
  for (int i = 0; i < nextPositions.size(); i++) {
      nextPositions.at(i).staticEval();
  }
  dynamicEval();
  if (whiteToMove) {
      dynamicEvaluation += nextPositions.size();
  }
  else {
      dynamicEvaluation -= nextPositions.size();
  }
  nextPositions.clear();
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
        /*if (whiteToMove) {
            nextPositions.at(i).dynamicEvaluation -= nextPositions.at(i).nextPositions.size();
        }
        else {
            nextPositions.at(i).dynamicEvaluation += nextPositions.at(i).nextPositions.size();
        }*/
    }
    dynamicEval();
    if (whiteToMove) {
        dynamicEvaluation += nextPositions.size();
    }
    else {
        dynamicEvaluation -= nextPositions.size();
    }
    nextPositions.clear();
}



//
//
//
void Position::threePlyEval() {
    findLegalMoves();
    for (int i = 0; i < nextPositions.size(); i++) {
        nextPositions.at(i).findLegalMoves();
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            nextPositions.at(i).nextPositions.at(j).findLegalMoves();     // depth 3
            for (int k = 0; k < nextPositions.at(i).nextPositions.at(j).nextPositions.size(); k++)
                nextPositions.at(i).nextPositions.at(j).nextPositions.at(k).staticEval();
        }
    }
    for (int i = 0; i < nextPositions.size(); i++) {
        for (int j = 0; j < nextPositions.at(i).nextPositions.size(); j++) {
            nextPositions.at(i).nextPositions.at(j).dynamicEval();        // depth 2
            if (whiteToMove) {
                dynamicEvaluation += nextPositions.at(i).nextPositions.at(j).nextPositions.size();
            }
            else {
                dynamicEvaluation -= nextPositions.at(i).nextPositions.at(j).nextPositions.size();
            }
        }
        nextPositions.at(i).dynamicEval();        // depth 1
        /*if (whiteToMove) {
            dynamicEvaluation += nextPositions.at(i).nextPositions.size();
        }
        else {
            dynamicEvaluation -= nextPositions.at(i).nextPositions.size();
        }*/
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
void Position::staticEval() {

    double whiteMaterial = 0;
    double blackMaterial = 0;
    int whiteBishops = 0;
    int blackBishops = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (pieces[i][j] == '.')
                continue;
            if (pieces[i][j] == 'p') {
                blackMaterial += 120 - i;
                continue;
            }
            if (pieces[i][j] == 'P') {
                whiteMaterial += 120 - i + 7;
                continue;
            }
            if (pieces[i][j] == 'r') {
                blackMaterial += 640;
                continue;
            }
            if (pieces[i][j] == 'n') {
                blackMaterial += 390;
                if (i == 0 || j == 0 || j == 7)
                    blackMaterial -= 18;
                continue;
            }
            if (pieces[i][j] == 'b') {
                blackMaterial += 390;
                if (i == 0)
                    blackMaterial -= 10;
                blackBishops ++;
                continue;
            }
            if (pieces[i][j] == 'q') {
                blackMaterial += 1180;
                continue;
            }
            if (pieces[i][j] == 'k') {
                blackMaterial += 12000;
                if (i == 0 && j == 6 && pieces[0][7] != 'r')
                    blackMaterial += 84;
                else if (i == 0 && j == 2 && pieces[0][0] != 'r')
                    blackMaterial += 60;
                continue;
            }
            if (pieces[i][j] == 'R') {
                whiteMaterial += 640;
                continue;
            }
            if (pieces[i][j] == 'N') {
                whiteMaterial += 390;
                if (i == 7 || j == 0 || j == 7)
                    whiteMaterial -= 18;
                continue;
            }
            if (pieces[i][j] == 'B') {
                whiteMaterial += 390;
                if (i == 0)
                    whiteMaterial -= 10;
                whiteBishops ++;
                continue;
            }
            if (pieces[i][j] == 'Q') {
                whiteMaterial += 1180;
                continue;
            }
            if (pieces[i][j] == 'K') {
                whiteMaterial += 12000;
                if (i == 7 && j == 6 && pieces[7][7] != 'R')
                    whiteMaterial += 84;
                else if (i == 7 && j == 2 && pieces[7][0] != 'R')
                    whiteMaterial += 60;
                continue;
            }
        }
    }
    if (whiteBishops > 1 && blackBishops < 2)
        whiteMaterial += 72;
    else if (blackBishops > 1 && whiteBishops < 2)
        blackMaterial += 72;
    if (pieces[3][3] == 'p')// || pieces[3][3] == 'n' || pieces[3][3] == 'b')
        blackMaterial += 12;
    else if (pieces[3][3] == 'P')// || pieces[3][3] == 'N' || pieces[3][3] == 'B')
        whiteMaterial += 12;
    if (pieces[3][4] == 'p')// || pieces[3][4] == 'n' || pieces[3][4] == 'b')
        blackMaterial += 12;
    else if (pieces[3][4] == 'P')// || pieces[3][4] == 'N' || pieces[3][4] == 'B')
        whiteMaterial += 12;
    if (pieces[4][3] == 'P')// || pieces[4][3] == 'N' || pieces[4][3] == 'B')
        whiteMaterial += 12;
    else if (pieces[4][3] == 'p')// || pieces[4][3] == 'n' || pieces[4][3] == 'b')
        blackMaterial += 12;
    if (pieces[4][4] == 'P')// || pieces[4][4] == 'N' || pieces[4][4] == 'B')
        whiteMaterial += 12;
    else if (pieces[4][4] == 'p')// || pieces[4][4] == 'n' || pieces[4][4] == 'b')
        blackMaterial += 12;
    // whiteMaterial = 16980    blackMaterial = 16860
    // dynamicEvaluation = log(whiteMaterial / blackMaterial) [negative when white less than black] [optionally * 300] = 0.92403436858
    // whiteMaterial = 12520     blackMaterial = 12400
    // dynamicEvaluation = greater than previous @ 1.254793114
    dynamicEvaluation = log(whiteMaterial / blackMaterial) * 30000;
}
