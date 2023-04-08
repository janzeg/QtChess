#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece
{
public:
    explicit King(QObject *parent = nullptr);
};

#endif // KING_H
