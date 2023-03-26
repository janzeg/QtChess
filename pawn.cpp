#include "pawn.h"

Pawn::Pawn(QObject *parent)
    : Piece{parent}
{

}

bool Pawn::moveValid(int colFrom, int rankFrom, int colTo, int rankTo)
{
    if (colTo == colFrom && rankTo == rankFrom + 1) {
        return true;
    }

    return false;
}
