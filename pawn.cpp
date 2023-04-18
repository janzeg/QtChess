#include "pawn.h"

Pawn::Pawn(QObject *parent)
    : Piece{parent}
{

}

bool Pawn::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color) {

    bool validOk = false;


    if (colFrom > 8 || rankFrom > 8 || colFrom < 1 || rankFrom < 1 ||
        colTo > 8 || rankTo > 8 || colTo < 1 || rankTo < 1)
    {
        return false;
    }

    switch (color)
    {

    case 'w':
        // Ruch do przodu
        if ((rankFrom == 2) && (colTo == colFrom && rankTo == rankFrom + 2) &&
            (board->data(colTo, rankTo) == ' ') && (board->data(colTo, rankTo - 1) == ' '))
        {
            validOk = true;
        }
        if ((colTo == colFrom && rankTo == rankFrom + 1) && (board->data(colTo, rankTo) == ' ')) {
            validOk = true;
        }
        // Bicie
        if (((colTo == colFrom + 1 || colTo == colFrom - 1) && rankTo == rankFrom + 1) &&
                (board->getColor(colTo, rankTo) == 'b') && (tolower(board->data(colTo, rankTo)) != 'k')) {
            validOk = true;
        }
        break;

    case 'b':
        // Ruch do przodu
        if ((rankFrom == 7) && (colTo == colFrom && rankTo == rankFrom - 2) &&
            (board->data(colTo, rankTo) == ' ') && (board->data(colTo, rankTo + 1) == ' ')) {
            validOk = true;
        }
        if ((colTo == colFrom && rankTo == rankFrom - 1) && (board->data(colTo, rankTo) == ' ')) {
            validOk = true;
        }
        // Bicie
        if (((colTo == colFrom + 1 || colTo == colFrom - 1) && rankTo == rankFrom - 1) &&
                (board->getColor(colTo, rankTo) == 'w') && (tolower(board->data(colTo, rankTo)) != 'k')) {
            validOk = true;
        }
        break;
    }

    return validOk;
}
