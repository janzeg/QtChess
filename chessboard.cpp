#include "chessboard.h"
#include <QDebug>

ChessBoard::ChessBoard(QObject *parent, int ranks, int columns)
    : QObject{parent}, m_ranks(ranks), m_columns(columns)
{
    initBoard();
}

int ChessBoard::ranks() const
{
    return m_ranks;
}

int ChessBoard::columns() const
{
    return m_columns;
}

void ChessBoard::setRanks(int newRanks)
{
    if(ranks() == newRanks) return;
    m_ranks = newRanks;
    initBoard();
    emit ranksChanged(m_ranks);
}

void ChessBoard::setColumns(int newColumns)
{
    if(columns() == newColumns) return;
    m_columns = newColumns;
    initBoard();
    emit columnsChanged(m_columns);
}

// Reset szachownicy (wyczyszczenie wektora przechowującego rozmieszczenie figur)
void ChessBoard::initBoard()
{
    m_boardData.fill(' ', ranks()*columns());
    emit boardReset();
}

// Określenie jaka figura znajduje się w danym polu (kolumna, wiersz) szachownicy
char ChessBoard::data(int column, int rank) const
{
    //qDebug() << m_boardData.at((rank-1)*columns()+(column-1));
    return m_boardData.at((rank-1)*columns()+(column-1));
}

// Ustawienie figury w danym polu szachownicy (wpisanie odpowiedniego znaku do wektora m_boardData)
void ChessBoard::setData(int column, int rank, char value)
{
    // Jeżeli zmieni się ułożenie figur na szachownicy, to emitowany jest sygnał dataChanged
    if(setDataInternal(column, rank, value))    
    emit dataChanged(column, rank);
    //qDebug() << m_boardData;
}

bool ChessBoard::setDataInternal(int column, int rank, char value)
{
    int index = (rank-1)*columns()+(column-1);
    if(m_boardData.at(index) == value) return false;
    m_boardData[index] = value;
    return true;
}

// Przesunięcie figury z jednego pola na drugie
void ChessBoard::movePiece(int fromColumn, int fromRank, int toColumn, int toRank)
{
    setData(toColumn, toRank, data(fromColumn, fromRank));

    //qDebug() << data(fromColumn, fromRank);
    // Wyczyszczenie pola, z którego został wykonany ruch
    setData(fromColumn, fromRank, ' ');
}

void ChessBoard::setFen(const QString &fen)
{
    int index = 0;
    int skip = 0;
    const int columnCount = columns();
    QChar ch;
    for(int rank = ranks(); rank > 0; --rank) {
        for(int column = 1; column <= columnCount; ++column) {
            if(skip > 0) {
                ch = ' ';
                skip--;
            } else {
                ch = fen.at(index++);
                if(ch.isDigit()) {
                    skip = ch.toLatin1()-'0';
                    ch = ' ';
                    skip--;
                }
            }
            setDataInternal(column, rank, ch.toLatin1());
        }
        QChar next = fen.at(index++);
        if(next != '/' && next != ' ') {
            initBoard();
            return; // fail on error
        }
    }
    emit boardReset();
}
