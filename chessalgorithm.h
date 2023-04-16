#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include <QObject>
#include "chessboard.h"
#include "piece.h"
#include "pawn.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "king.h"

class ChessAlgorithm : public QObject
{
    Q_OBJECT

public:
    explicit ChessAlgorithm(QObject *parent = 0);
    ChessBoard* board() const;
    ChessBoard* bufferBoard() const;

    inline ChessBoard::Player currentPlayer() const { return m_currentPlayer; }

    Piece *currentPiece() const;
    void setCurrentPiece(Piece *newCurrentPiece);
    void setCurrentPiece(char newCurrentPiece);

    bool isCheckMate(char color);
    bool isDeadLock(char color);

    struct castlingCondType {
        bool wKingMoved = false;
        bool wRookMoved = false;
        bool bKingMoved = false;
        bool bRookMoved = false;
    };
    castlingCondType castlingCond() const;
    void setCastlingCond(char piece, bool value);
    bool validCastling(int colFrom, int rankFrom, int colTo, int rankTo, char color);

signals:
    void boardChanged(ChessBoard*);
    void currentPlayerChanged(ChessBoard::Player);

public slots:
    virtual void newGame();
    virtual bool move(int colFrom, int rankFrom, int colTo, int rankTo);
    bool move(const QPoint &from, const QPoint &to);

protected:
    virtual void setupBoard();

private:
    ChessBoard* m_board;
    ChessBoard* m_bufferBoard;
    ChessBoard::Player m_currentPlayer;
    Piece* m_currentPiece;
    Pawn m_pawn;
    Rook m_rook;
    Bishop m_bishop;
    Knight m_knight;
    Queen m_queen;
    King m_king;
    castlingCondType m_castlingCond;
    void setBufferBoard(ChessBoard *bufferBoard);
    void copyBoardToBuffer();
    void copyBufferToBoard();
    void setBoard(ChessBoard *board);
    void setCurrentPlayer(ChessBoard::Player);

};

#endif // CHESSALGORITHM_H
