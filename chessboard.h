#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>
#include <QDebug>

class ChessBoard : public QObject
{
    Q_OBJECT
    // Sygnały ranksChanged/columnsChanged informują o zmianie wartości ranks/columns
    Q_PROPERTY(int ranks READ ranks NOTIFY ranksChanged)
    Q_PROPERTY(int columns READ columns NOTIFY columnsChanged)
    Q_ENUMS(GameState Piece)

public:
    enum GameState { Normal, Check, CheckMate };
    enum Piece { Queen, Rook, Bishop, Knight};
    explicit ChessBoard(QObject *parent = nullptr, int ranks = 0, int columns = 0);
    int ranks() const;
    int columns() const;
    void setData(int column, int rank, char value);
    char data(int column, int rank) const;
    void movePiece(int fromColumn, int fromRank, int toColumn, int toRank);
    void setFen(const QString &fen);
    char getColor(int column, int rank);
    void getPiecePosition(char piece, int &column, int &rank) const;
    //QVector<char> m_boardData;
    bool isCheck(char color);

    QVector<char> boardData() const;

    void setBoardData(const QVector<char> &newBoardData);

    char getColor(char piece);

    //void setMessage1(const QString &newMessage1);
    //QString getMessage1() const;

    void setCurrentPlayer(const QString &newCurrentPlayer);

    QString currentPlayer() const;

    void setGameState(GameState newGameState);

    GameState gameState() const;

    void setPromotion(bool newPromotion);

    bool promotion() const;

    Piece promoteTo() const;

    // TESTY
    int promCol;
    int promRank;
    // TESTY

signals:
    void ranksChanged(int);
    void columnsChanged(int);
    void boardReset();
    void dataChanged(int c, int r);
    void currentPlayerChanged();
    void gameStateChanged(ChessBoard::GameState);
    void startNewGame();
    void promotionChanged();

public slots:
    void setPromoteTo(ChessBoard::Piece newPromoteTo);

private:
    int m_ranks;
    int m_columns;
    /* Wektor znaków reprezentujący układ figur na szachownicy
     *            biała     czarna
     * Król     - K         k
     * Hetman   - Q         q
     * Wieża    - R         r
     * Goniec   - B         b
     * Skoczek  - N         n
     * Pionek   - P         p */
    QVector<char> m_boardData;
    QString m_currentPlayer;
    GameState m_gameState;
    bool m_promotion;
    Piece m_promoteTo;



protected:
    void setRanks(int newRanks);
    void setColumns(int newColumns);
    void initBoard();
    bool setDataInternal(int column, int rank, char value);
};

#endif // CHESSBOARD_H
