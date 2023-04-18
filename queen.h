#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece
{
public:
    explicit Queen(QObject *parent = nullptr);
    bool moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color);
};

#endif // QUEEN_H
