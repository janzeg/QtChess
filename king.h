#ifndef KING_H
#define KING_H

#include "piece.h"
#include "QDebug"

class King : public Piece
{
public:
    explicit King(QObject *parent = nullptr);
    bool moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color);
};

#endif // KING_H
