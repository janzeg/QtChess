#include "queen.h"

Queen::Queen(QObject *parent)
    : Piece{parent}
{

}

bool Queen::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color) {

    bool validOk = true;

    enum moveType { none,
                    crossTopRight, crossTopLeft, crossDownRight, crossDownLeft,
                    right, left, up, down};

    moveType move = none;

    for (int i = 1; i <= 8; i++) {
        if (rankTo == rankFrom + i && colTo == colFrom + i) {
            move = crossTopRight;
        }
        else if (rankTo == rankFrom - i && colTo == colFrom - i) {
            move = crossDownLeft;
        }
        else if (rankTo == rankFrom + i && colTo == colFrom - i) {
            move = crossTopLeft;
        }
        else if (rankTo == rankFrom - i && colTo == colFrom + i) {
            move = crossDownRight;
        }
    }

    if (rankTo == rankFrom)  {
        if (colTo > colFrom) {
            move = right;
        }
        else if (colTo < colFrom) {
            move = left;
        }
    }
    else if (colTo == colFrom) {
        if (rankTo > rankFrom) {
            move = up;
        }
        else if (rankTo < rankFrom) {
            move = down;
        }
    }


    int j;

    // Sprawdzenie czy coś nie stoi na drodze
    switch (move)
    {
    case none:
        validOk = false;
        break;

    case crossTopRight:
        j = 1;
        for (int i = rankFrom + 1; i < rankTo; i++) {
            //qDebug() << "P(" << colFrom + j << ", " << rankFrom + j << ")";
            if (board->data(colFrom + j, rankFrom + j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;

    case crossTopLeft:
        j = 1;
        for (int i = rankFrom + 1; i < rankTo; i++) {
            if (board->data(colFrom - j, rankFrom + j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;

    case crossDownRight:
        j = 1;
        for (int i = rankTo; i < rankFrom - 1; i++) {
            if (board->data(colFrom + j, rankFrom - j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;

    case crossDownLeft:
        j = 1;
        //qDebug() << "P(" << colFrom - j << ", " << rankFrom - j << ")";
        for (int i = rankTo; i < rankFrom - 1; i++) {
            if (board->data(colFrom - j, rankFrom - j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;

    case up:
        for (int i = rankFrom + 1; i < rankTo; i++) {
            if (board->data(colTo, i) != ' ') {
                validOk = false;
            }
        }
        break;

    case down:
        for (int i = rankTo + 1; i < rankFrom; i++) {
            if (board->data(colTo, i) != ' ') {
                validOk = false;
            }
        }
        break;

    case right:
        for (int i = colFrom + 1; i < colTo; i++) {
            if (board->data(i, rankTo) != ' ') {
                validOk = false;
            }
        }
        break;

    case left:
        for (int i = colTo + 1; i < colFrom; i++) {
            if (board->data(i, rankTo) != ' ') {
                validOk = false;
            }
        }
        break;
    }

    // Bicie
    if (color == board->getColor(colTo, rankTo)) {
        validOk = false;
    }

    return validOk;

}
