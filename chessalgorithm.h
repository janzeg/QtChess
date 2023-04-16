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
    Q_ENUMS(Result Player)
    Q_PROPERTY(Result result READ result CONSTANT)
    Q_PROPERTY(Player currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)



public:
    enum Result { NoResult, PlayerWhiteWins, Draw, PlayerBlackWins };
    //enum GameState { Normal, Check};
    enum Player { NoPlayer = ' ', PlayerWhite = 'w', PlayerBlack = 'b' };
    struct castlingCondType {
        bool wKingMoved = false;
        bool wRookMoved = false;
        bool bKingMoved = false;
        bool bRookMoved = false;
    };


    explicit ChessAlgorithm(QObject *parent = 0);
    ChessBoard* board() const;
    inline Result result() const { return m_result; }
    inline Player currentPlayer() const { return m_currentPlayer; }


    Piece *currentPiece() const;
    void setCurrentPiece(Piece *newCurrentPiece);
    Pawn pawn() const;
    Rook rook() const;
    ChessBoard* bufferBoard() const; // ???????

    bool isCheckMate(char color);
    void setCurrentPiece(char newCurrentPiece);
    bool isDeadLock(char color);

    bool validCastling(int colFrom, int rankFrom, int colTo, int rankTo, char color);

    castlingCondType castlingCond() const;

    void setCastlingCond(char piece, bool value);



signals:
    void boardChanged(ChessBoard*);
    void gameOver(ChessAlgorithm::Result);
    void currentPlayerChanged(ChessAlgorithm::Player);

public slots:
    virtual void newGame();
    virtual bool move(int colFrom, int rankFrom, int colTo, int rankTo);
    bool move(const QPoint &from, const QPoint &to);

protected:
    virtual void setupBoard();
    void setBoard(ChessBoard *board);
    void setResult(Result);
    void setCurrentPlayer(Player);
    void setBufferBoard(ChessBoard *bufferBoard); // ???????

private:
    ChessBoard* m_board;
    ChessBoard* m_bufferBoard;
    Result m_result;
    Player m_currentPlayer;
    Piece* m_currentPiece;
    Pawn m_pawn;
    Rook m_rook;
    Bishop m_bishop;
    Knight m_knight;
    Queen m_queen;
    King m_king;
    void copyBoardToBuffer();
    void copyBufferToBoard();
    castlingCondType m_castlingCond;

};

#endif // CHESSALGORITHM_H
