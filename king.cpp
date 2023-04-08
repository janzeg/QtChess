#include "king.h"

King::King(QObject *parent)
    : Piece{parent}
{

}

bool King::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard *board, char color)
{
    bool validOk = true;


    if ((rankTo != rankFrom && rankTo != rankFrom + 1 && rankTo != rankFrom - 1) ||
        (colTo != colFrom && colTo != colFrom + 1 && colTo != colFrom - 1))
    {
        validOk = false;
    }



    // Bicie
    if (color == board->getColor(colTo, rankTo) || tolower(board->data(colTo, rankTo)) == 'k') {
        validOk = false;
    }

    return validOk;
}
