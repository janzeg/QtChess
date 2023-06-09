#include "king.h"

King::King(QObject *parent)
    : Piece{parent}
{

}

bool King::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color)
{
    bool validOk = true;

    if (colFrom > 8 || rankFrom > 8 || colFrom < 1 || rankFrom < 1 ||
        colTo > 8 || rankTo > 8 || colTo < 1 || rankTo < 1)
    {
        return false;
    }

    if ((rankTo != rankFrom && rankTo != rankFrom + 1 && rankTo != rankFrom - 1) ||
        (colTo != colFrom && colTo != colFrom + 1 && colTo != colFrom - 1))
    {
        validOk = false;
    }

    // Bicie
    if ((color == board->getColor(colTo, rankTo)) || tolower(board->data(colTo, rankTo)) == 'k') {
        validOk = false;
    }

    // Sprawdzam pozycję króla o przeciwnym kolorze
    char king = 'K';
    if (color == 'w') {
        king = tolower(king);
    }
    int kingCol, kingRank;
    board->getPiecePosition(king, kingCol, kingRank);
    // Król nie może szachować króla przeciwnika
    if (rankTo == kingRank + 1 || rankTo == kingRank || rankTo == kingRank - 1)    {
        if (colTo == kingCol - 1 || colTo == kingCol || colTo == kingCol + 1) {
            validOk = false;
        }
    }

    return validOk;
}
