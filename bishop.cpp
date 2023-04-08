#include "bishop.h"

Bishop::Bishop(QObject *parent)
    : Piece{parent}
{

}

bool Bishop::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, char color) {

    bool validOk = true;

    enum moveType { none, topRight, topLeft, downRight, downLeft };
    moveType crossMove = none;

    for (int i = 1; i <= 8; i++) {
        if (rankTo == rankFrom + i && colTo == colFrom + i) {
            crossMove = topRight;
        }
        else if (rankTo == rankFrom - i && colTo == colFrom - i) {
            crossMove = downLeft;
        }
        else if (rankTo == rankFrom + i && colTo == colFrom - i) {
            crossMove = topLeft;
        }
        else if (rankTo == rankFrom - i && colTo == colFrom + i) {
            crossMove = downRight;
        }
    }

    //qDebug() << crossMove;

    int j;

    // Sprawdzenie czy coś nie stoi na drodze
    switch (crossMove)
    {
    case none:
        validOk = false;
        break;

    case topRight:
        j = 1;
        for (int i = rankFrom + 1; i < rankTo; i++) {
            //qDebug() << "P(" << colFrom + j << ", " << rankFrom + j << ")";
            if (board->data(colFrom + j, rankFrom + j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;

    case topLeft:
        j = 1;
        for (int i = rankFrom + 1; i < rankTo; i++) {
            qDebug() << "P(" << colFrom - j << ", " << rankFrom + j << ")";
            if (board->data(colFrom - j, rankFrom + j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;

    case downRight:
        j = 1;
        for (int i = rankTo; i < rankFrom - 1; i++) {
            if (board->data(colFrom + j, rankFrom - j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;

    case downLeft:
        j = 1;
        //qDebug() << "P(" << colFrom - j << ", " << rankFrom - j << ")";
        for (int i = rankTo; i < rankFrom - 1; i++) {
            if (board->data(colFrom - j, rankFrom - j) != ' ') {
                validOk = false;
            }
            j++;
        }
        break;
    }

    // Bicie
    if (color == board->getColor(colTo, rankTo) || tolower(board->data(colTo, rankTo)) == 'k') {
        validOk = false;
    }

    return validOk;
}
