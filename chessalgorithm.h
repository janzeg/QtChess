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

signals:
    void boardChanged(ChessBoard*);
    void currentPlayerChanged(ChessBoard::Color);

public slots:
    virtual void newGame();
    virtual bool move(int colFrom, int rankFrom, int colTo, int rankTo);
    bool move(const QPoint &from, const QPoint &to);

protected:
    void setBoard(ChessBoard *board);
    virtual void setupBoard();

    ChessBoard* bufferBoard() const;
    void setBufferBoard(ChessBoard *bufferBoard);
    void copyBoardToBuffer();

    inline ChessBoard::Color currentPlayer() const { return m_currentPlayer; }
    void setCurrentPlayer(ChessBoard::Color value);

    Piece *currentPiece() const;
    void setCurrentPiece(Piece *newCurrentPiece);
    void setCurrentPiece(char newCurrentPiece);

    bool isCheckMate(ChessBoard::Color playerColor);
    bool isDeadLock(ChessBoard::Color playerColor);

    struct castlingCondType {
        bool wKingMoved = false;
        bool wRookMoved = false;
        bool bKingMoved = false;
        bool bRookMoved = false;
    };
    castlingCondType castlingCond() const;
    void setCastlingCond(char piece, bool value);
    bool validCastling(int colFrom, int rankFrom, int colTo, int rankTo, ChessBoard::Color playerColor);


private:
    ChessBoard* m_board;
    ChessBoard* m_bufferBoard;
    ChessBoard::Color m_currentPlayer;
    Piece* m_currentPiece;
    Pawn m_pawn;
    Rook m_rook;
    Bishop m_bishop;
    Knight m_knight;
    Queen m_queen;
    King m_king;
    castlingCondType m_castlingCond;
};

#endif // CHESSALGORITHM_H
