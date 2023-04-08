#include "pawn.h"

Pawn::Pawn(QObject *parent)
    : Piece{parent}
{

}

bool Pawn::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color) {

    bool validOk = false;

    switch (color)
    {

    case 'w':
        // Ruch do przodu
        if ((rankFrom == 2) && (colTo == colFrom && rankTo == rankFrom + 2) && (board->data(colTo, rankTo) == ' ')) {
            validOk = true;
        }
        if ((colTo == colFrom && rankTo == rankFrom + 1) && (board->data(colTo, rankTo) == ' ')) {
            validOk = true;
        }
        // Bicie
        if (((colTo == colFrom + 1 || colTo == colFrom - 1) && rankTo == rankFrom + 1) && board->getColor(colTo, rankTo) == 'b') {
            validOk = true;
        }
        break;

    case 'b':
        // Ruch do przodu
        if ((rankFrom == 7) && (colTo == colFrom && rankTo == rankFrom - 2) && (board->data(colTo, rankTo) == ' ')) {
            validOk = true;
        }
        if ((colTo == colFrom && rankTo == rankFrom - 1) && (board->data(colTo, rankTo) == ' ')) {
            validOk = true;
        }
        // Bicie
        if (((colTo == colFrom + 1 || colTo == colFrom - 1) && rankTo == rankFrom - 1) && board->getColor(colTo, rankTo) == 'b') {
            validOk = true;
        }
        break;
    }


    return validOk;
}
