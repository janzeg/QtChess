#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"
#include <QDebug>

class Bishop : public Piece
{
public:
    explicit Bishop(QObject *parent = nullptr);
    bool moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, ChessBoard* bufferBoard, char color);
};

#endif // BISHOP_H
