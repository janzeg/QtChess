#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>

class ChessBoard : public QObject
{
    Q_OBJECT
    // Sygnały ranksChanged/columnsChanged informują o zmianie wartości ranks/columns
    Q_PROPERTY(int ranks READ ranks NOTIFY ranksChanged)
    Q_PROPERTY(int columns READ columns NOTIFY columnsChanged)

public:
    explicit ChessBoard(QObject *parent = nullptr, int ranks = 0, int columns = 0);
    int ranks() const;
    int columns() const;
    void setData(int column, int rank, char value);
    char data(int column, int rank) const;
    void movePiece(int fromColumn, int fromRank, int toColumn, int toRank);
    void setFen(const QString &fen);
    char getColor(int column, int rank);

signals:
    void ranksChanged(int);
    void columnsChanged(int);
    void boardReset();
    void dataChanged(int c, int r);

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

protected:
    void setRanks(int newRanks);
    void setColumns(int newColumns);
    void initBoard();
    bool setDataInternal(int column, int rank, char value);
};

#endif // CHESSBOARD_H
