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
    if (column > 8 || rank > 8 || column < 1 || rank < 1 ) {
        return 0;
    }

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

char ChessBoard::getColor(int column, int rank)
{
    if (this->data(column, rank) == ' ') {
        return 'e'; // empty
    }

    if (isupper(this->data(column, rank)) == true) {
        return 'w'; // white
    }
    else {
        return 'b'; // black
    }
}

char ChessBoard::getColor(char piece)
{
    if (isupper(piece) == true) {
        return 'w'; // white
    }
    else {
        return 'b'; // black
    }
}

void ChessBoard::getPiecePosition(char piece, int &column, int &rank) const
{
    int index = m_boardData.indexOf(piece);
    //qDebug() << "INDEX - " << index;

    column = (index % 8) + 1;
    //qDebug() << "COLUMN - " << column;

    // Potem to zrobić w pętli for
    if (index < 7) {
        rank = 1;
    }
    else if (index >= 8 && index < 15) {
        rank = 2;
    }
    else if (index >= 16 && index <= 23) {
        rank = 3;
    }
    else if (index >= 24 && index <= 31) {
        rank = 4;
    }
    else if (index >= 32 && index <= 39) {
        rank = 5;
    }
    else if (index >= 40 && index <= 47) {
        rank = 6;
    }
    else if (index >= 48 && index <= 55) {
        rank = 7;
    }
    else if (index >= 56 && index <= 63) {
        rank = 8;
    }
    //qDebug() << "RANK - " << rank;
}

void ChessBoard::setBoardData(const QVector<char> &newBoardData)
{
    m_boardData = newBoardData;
}

QVector<char> ChessBoard::boardData() const
{
    return m_boardData;
}

bool ChessBoard::isCheck(char color) {

    // Parametr color - mówi czy sprawdzamy szach na czarnych czy na białych
    char king = 'k';
    char queen = 'Q';
    char rook = 'R';
    char bishop = 'B';
    char knight = 'N';
    char pawn = 'P';

    // Jeżeli sprawdzamy czy jest szach na białych to zmieniamy figury na czarne
    if (color == 'w') {
        king = toupper(king);
        queen = tolower(queen);
        rook = tolower(rook);
        bishop = tolower(bishop);
        knight = tolower(knight);
        pawn = tolower(pawn);
    }

    struct {
        bool topLeft = false;
        bool top = false;
        bool topRight = false;
        bool right = false;
        bool bottomRight = false;
        bool bottom = false;
        bool bottomLeft = false;
        bool left = false;
        bool knight = false;
    } check;

    struct {
        char topLeft;
        char top;
        char topRight;
        char right;
        char bottomRight;
        char bottom;
        char bottomLeft;
        char left;
    } piece;

    int kingCol, kingRank;
    this->getPiecePosition(king, kingCol, kingRank);


    // ----------------- SZACH PRAWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        piece.right = this->data(kingCol + i, kingRank);
        if (piece.right != ' ') {
            break;
        }
    }
    if (piece.right == queen || piece.right == rook) {
        check.right = true;
    }
    else {
        check.right = false;
    }

    // ----------------- SZACH LEWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        piece.left = this->data(kingCol - i, kingRank);
        if (piece.left != ' ') {
            break;
        }
    }
    if (piece.left == queen || piece.left == rook) {
        check.left = true;
    }
    else {
        check.left = false;
    }

    // ----------------- SZACH GÓRA ----------------- //
    for (int i = 1; i <= 7; i++) {
        piece.top = this->data(kingCol, kingRank + i);
        if (piece.top != ' ') {
            break;
        }
    }
    if (piece.top == queen || piece.top == rook) {
        check.top = true;
    }
    else {
        check.top = false;
    }

    // ----------------- SZACH DÓŁ ----------------- //
    for (int i = 1; i <= 7; i++) {
        piece.bottom = this->data(kingCol, kingRank - i);
        if (piece.bottom != ' ') {
            break;
        }
    }
    if (piece.bottom == queen || piece.bottom == rook) {
        check.bottom = true;
    }
    else {
        check.bottom = false;
    }

    int j;

    // ----------------- SZACH GÓRA-LEWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = kingRank + 1; i < 7; i++) {
            piece.topLeft = this->data(kingCol - j, kingRank + j);
            if (piece.topLeft != ' ') {
                break;
            }
            j++;
        }
    }
    if ((piece.topLeft == queen || piece.topLeft == bishop) ||
        (color == 'w' && piece.topLeft == pawn && j == 1))
    {
        check.topLeft = true;
    }
    else {
        check.topLeft = false;
    }

    // ----------------- SZACH GÓRA-PRAWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = kingRank + 1; i < 7; i++) {
            piece.topRight = this->data(kingCol + j, kingRank + j);
            if (piece.topRight != ' ') {

                //qDebug() << "ALALALA" << piece.topRight;

                break;
            }
            j++;
        }
    }
    if ((piece.topRight == queen || piece.topRight == bishop) ||
        (color == 'w' && piece.topRight == pawn && j == 1))
    {
        check.topRight = true;
    }
    else {
        check.topRight = false;
    }

    // ----------------- SZACH DÓŁ-LEWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = 0; i < kingRank - 1; i++) {
            piece.bottomLeft = this->data(kingCol - j, kingRank - j);
            if (piece.bottomLeft != ' ') {
                break;
            }
            j++;
        }
    }
    if ((piece.bottomLeft == queen || piece.bottomLeft == bishop) ||
        (color == 'b' && piece.bottomLeft == pawn && j == 1))
    {
        check.bottomLeft = true;
    }
    else {
        check.bottomLeft = false;
    }

    // ----------------- SZACH DÓŁ-PRAWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = 0; i < kingRank - 1; i++) {
            piece.bottomRight = this->data(kingCol + j, kingRank - j);
            if (piece.bottomRight != ' ') {
                break;
            }
            j++;
        }
    }
    if ((piece.bottomRight == queen || piece.bottomRight == bishop) ||
        (color == 'b' && piece.bottomRight == pawn && j == 1))
    {
        check.bottomRight = true;
    }
    else {
        check.bottomRight = false;
    }

    // ----------------- SZACH SKOCZEK ----------------- //
    if ((this->data(kingCol + 1, kingRank + 2) == knight) ||
        (this->data(kingCol - 1, kingRank + 2) == knight) ||
        (this->data(kingCol + 1, kingRank - 2) == knight) ||
        (this->data(kingCol - 1, kingRank - 2) == knight) ||
        (this->data(kingCol + 2, kingRank + 1) == knight) ||
        (this->data(kingCol + 2, kingRank - 1) == knight) ||
        (this->data(kingCol - 2, kingRank + 1) == knight) ||
        (this->data(kingCol - 2, kingRank - 1) == knight))
    {
        check.knight = true;
    }
    else {
        check.knight = false;
    }
    /*
    qDebug() << " CHECK Right - " << check.right;
    qDebug() << " CHECK Left - " << check.left;
    qDebug() << " CHECK Top - " << check.top;
    qDebug() << " CHECK Bottom - " << check.bottom;
    qDebug() << " CHECK TopLeft - " << check.topLeft;
    qDebug() << " CHECK TopRight - " << check.topRight;
    qDebug() << " CHECK BottomLeft - " << check.bottomLeft;
    qDebug() << " CHECK BottomRight - " << check.bottomRight;
    qDebug() << " CHECK Knight - " << check.knight;*/

    if (check.right || check.left || check.top || check.bottom || check.topLeft ||
        check.topRight || check.bottomLeft || check.bottomRight || check.knight)
    {
        return true;
    }
    else {
        return false;
    }

}

