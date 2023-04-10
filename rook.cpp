#include "rook.h"


Rook::Rook(QObject *parent)
    : Piece{parent}
{

}

bool Rook::moveValid(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard* board, ChessBoard* bufferBoard, char color) {

    if (colFrom > 8 || rankFrom > 8 || colFrom < 1 || rankFrom < 1 ||
        colTo > 8 || rankTo > 8 || colTo < 1 || rankTo < 1)
    {
        return false;
    }

    bool validOk = true;

    // Tylko ruchy lewo-prawo
    if ((rankFrom != rankTo && colFrom != colTo)) {
        validOk = false;
    }

    // Sprawdzenie czy coś nie stoi na drodze
    // Ruch w pionie w górę
    if (rankTo > rankFrom) {
        for (int i = rankFrom + 1; i < rankTo; i++) {
            if (board->data(colTo, i) != ' ') {
                validOk = false;
            }
        }
    }
    // Ruch w pionie w dół
    else if (rankTo < rankFrom) {
        for (int i = rankTo + 1; i < rankFrom; i++) {
            if (board->data(colTo, i) != ' ') {
                validOk = false;
            }
        }
    }
    // Ruch w poziomie w prawo
    else if (colTo > colFrom) {
        for (int i = colFrom + 1; i < colTo; i++) {
            if (board->data(i, rankTo) != ' ') {
                validOk = false;
            }
        }
    }
    else if (colTo < colFrom) {
        for (int i = colTo + 1; i < colFrom; i++) {
            if (board->data(i, rankTo) != ' ') {
                validOk = false;
            }
        }
    }

    // Bicie
    if (color == board->getColor(colTo, rankTo) || tolower(board->data(colTo, rankTo)) == 'k') {
        validOk = false;
    }

    return validOk;
}
