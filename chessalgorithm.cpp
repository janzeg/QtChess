#include "chessalgorithm.h"
#include <QPoint>
#include <QDebug>

ChessAlgorithm::ChessAlgorithm(QObject *parent)
    : QObject{parent}
{
    m_board = nullptr;
    m_currentPlayer = NoPlayer;
    m_result = NoResult;
}

ChessBoard *ChessAlgorithm::board() const
{
    return m_board;
}

void ChessAlgorithm::setBoard(ChessBoard *board)
{
    if(board == m_board) return;
    if(m_board) delete m_board;
    m_board = board;
    emit boardChanged(m_board);
}

// Utworzenie klasycznej szachownicy 8x8
void ChessAlgorithm::setupBoard()
{
    setBoard(new ChessBoard(this, 8,8));
}

void ChessAlgorithm::newGame()
{
    // Utworzenie szachownicy
    setupBoard();
    // Rozstawienie figur
    board()->setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void ChessAlgorithm::setResult(Result value)
{
    if(result() == value) return;
    if(result() == NoResult) {
        m_result = value;
        emit gameOver(m_result);
    } else { m_result = value; }
}

void ChessAlgorithm::setCurrentPlayer(Player value)
{
    if(currentPlayer() == value) return;
    m_currentPlayer = value;
    emit currentPlayerChanged(m_currentPlayer);
}

/*
Rook ChessAlgorithm::rook() const
{
    return m_rook;
}
*/

/*
Pawn ChessAlgorithm::pawn() const
{
    return m_pawn;
}
*/

void ChessAlgorithm::setCurrentPiece(Piece *newCurrentPiece)
{
    m_currentPiece = newCurrentPiece;
}

Piece *ChessAlgorithm::currentPiece() const
{
    return m_currentPiece;
}

bool ChessAlgorithm::move(int colFrom, int rankFrom,
                          int colTo, int rankTo)
{
    //Q_UNUSED(colFrom)
    //Q_UNUSED(rankFrom)
    //Q_UNUSED(colTo)
    //Q_UNUSED(rankTo)
    //return false;

    // Określenie jaka figura została wybrana
    char source = board()->data(colFrom, rankFrom);
    qDebug() << "SOURCE:" << source;

    switch (tolower(source))
    {
    case 'p':
        setCurrentPiece(&m_pawn);
        break;
    case 'r':
        setCurrentPiece(&m_rook);
        break;
    case 'b':
        setCurrentPiece(&m_bishop);
        break;
    case 'n':
        setCurrentPiece(&m_knight);
        break;
    case 'q':
        setCurrentPiece(&m_queen);
        break;
    case 'k':
        setCurrentPiece(&m_king);
        break;
    }

    // Określenie koloru figury
    char color;
    if (isupper(source) == true) { color = 'w'; }   // biały
    else { color = 'b'; }                           // czarny

    if (currentPiece()->moveValid(colFrom, rankFrom, colTo, rankTo, board(), color)) {
        board()->movePiece(colFrom, rankFrom, colTo, rankTo);
    }

    isCheck('w');   // sprawdzenie czy jest szach na

    /*
    int col, rank;
    board()->getPiecePosition('r', col, rank);
    qDebug() << "col: " << col;
    qDebug() << "rank: " << rank;
    qDebug() << "DATA - " << board()->data(col, rank); */

    return true;
}

bool ChessAlgorithm::move(const QPoint &from, const QPoint &to)
{
    qDebug() << "ChessAlgorithm::move";
    return move(from.x(), from.y(), to.x(), to.y());
}

bool ChessAlgorithm::isCheck(char color) {

    // Na razie testy algorytmu wykrywania szacha dla czarnego króla
    // Potem trzeba będzie rozdzielić to w zależności do koloru figury:
    // jeżeli ruch wykonują białe to sprawdzam szacha dla czarnego króla i odwrotnie.
    // Potem też trzeba będzie przy walidacji sprawdzać czy dany ruch nie powoduje szacha (odsłonięcie króla na szach)
    // w tym celu trzeba będzie przekazywać potencjalny stan tablicy do tej funkcji jako parametr
    // i będzie ona weryfikować czy dany potencjalny ruch (powodujący przekazane rozłożenie figur) powoduje szach
    // (tym razem na królu koloru tego samego co figura, która się rusza).


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
    board()->getPiecePosition('K', kingCol, kingRank);


    // ----------------- SZACH PRAWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        piece.right = board()->data(kingCol + i, kingRank);
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
        piece.left = board()->data(kingCol - i, kingRank);
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
        piece.top = board()->data(kingCol, kingRank + i);
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
        piece.bottom = board()->data(kingCol, kingRank - i);
        if (piece.bottom != ' ') {
            break;
        }
    }
    if (piece.bottom == queen || piece.bottom == rook) {
        piece.bottom = true;
    }
    else {
        piece.bottom = false;
    }


    int j;

    // ----------------- SZACH GÓRA-LEWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = kingRank + 1; i < 7; i++) {
            piece.topLeft = board()->data(kingCol - j, kingRank + j);
            if (piece.topLeft != ' ') {
                break;
            }
            j++;
        }
    }
    if (piece.topLeft == queen || piece.topLeft == bishop) {
        check.topLeft = true;
    }
    else {
        check.topLeft = false;
    }

    // ----------------- SZACH GÓRA-PRAWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = kingRank + 1; i < 7; i++) {
            piece.topRight = board()->data(kingCol + j, kingRank + j);
            if (piece.topRight != ' ') {
                break;
            }
            j++;
        }
    }
    if (piece.topRight == queen || piece.topRight == bishop) {
        check.topRight = true;
    }
    else {
        check.topRight = false;
    }

    // ----------------- SZACH DÓŁ-LEWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = 0; i < kingRank - 1; i++) {
            piece.bottomLeft = board()->data(kingCol - j, kingRank - j);
            if (piece.bottomLeft != ' ') {
                break;
            }
            j++;
        }
    }
    if (piece.bottomLeft == queen || piece.bottomLeft == bishop) {
        check.bottomLeft = true;
    }
    else {
        check.bottomLeft = false;
    }

    // ----------------- SZACH DÓŁ-PRAWO ----------------- //
    for (int i = 1; i <= 7; i++) {
        j = 1;
        for (int i = 0; i < kingRank - 1; i++) {
            piece.bottomRight = board()->data(kingCol + j, kingRank - j);
            if (piece.bottomRight != ' ') {
                break;
            }
            j++;
        }
    }
    if (piece.bottomRight == queen || piece.bottomRight == bishop) {
        check.bottomRight = true;
    }
    else {
        check.bottomRight = false;
    }

    // ----------------- SZACH SKOCZEK ----------------- //
    if ((board()->data(kingCol + 1, kingRank + 2) == knight) ||
        (board()->data(kingCol - 1, kingRank + 2) == knight) ||
        (board()->data(kingCol + 1, kingRank - 2) == knight) ||
        (board()->data(kingCol - 1, kingRank - 2) == knight) ||
        (board()->data(kingCol + 2, kingRank + 1) == knight) ||
        (board()->data(kingCol + 2, kingRank - 1) == knight) ||
        (board()->data(kingCol - 2, kingRank + 1) == knight) ||
        (board()->data(kingCol - 2, kingRank - 1) == knight))
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

    return false;
}
