#include "knight.h"
#include <QDebug>

Knight::Knight(QObject *parent)
    : Piece{parent}
{

}

bool Knight::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color) {

    bool validOk = true;

    if (colFrom > 8 || rankFrom > 8 || colFrom < 1 || rankFrom < 1 ||
        colTo > 8 || rankTo > 8 || colTo < 1 || rankTo < 1)
    {
        return false;
    }

    // Tylko ruchy w kształcie L
    if (colTo == colFrom + 1 || colTo == colFrom - 1) {
        if (rankTo != rankFrom + 2 && rankTo != rankFrom - 2) {
            validOk = false;
        }
    }
    else if (colTo == colFrom + 2 || colTo == colFrom - 2) {
        if (rankTo != rankFrom + 1 && rankTo != rankFrom - 1) {
            validOk = false;
        }
    }
    else {
        validOk = false;
    }

    // Bicie
    if (color == board->getColor(colTo, rankTo) || tolower(board->data(colTo, rankTo)) == 'k') {
        validOk = false;
    }

    return validOk;
}
