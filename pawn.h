#ifndef PAWN_H
#define PAWN_H

#include "piece.h"
#include <QDebug>

class Pawn : public Piece
{
public:
    explicit Pawn(QObject *parent = nullptr);
    bool moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color);
};

#endif // PAWN_H
