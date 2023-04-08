#ifndef ROOK_H
#define ROOK_H

#include "piece.h"
#include <QDebug>

class Rook : public Piece
{
public:
    explicit Rook(QObject *parent = nullptr);
    bool moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color);
};

#endif // ROOK_H
