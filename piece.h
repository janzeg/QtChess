#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include "chessboard.h"

class Piece : public QObject
{
    Q_OBJECT
public:
    explicit Piece(QObject *parent = nullptr);
    virtual bool moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, ChessBoard* bufferBoard, char color) = 0;

signals:

};

#endif // PIECE_H
