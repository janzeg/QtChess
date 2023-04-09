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
    if (isupper(source) == true) {
        color = 'w'; // biały
    }
    else {
        color = 'b'; // czarny
    }

    if (currentPiece()->moveValid(colFrom, rankFrom, colTo, rankTo, board(), color)) {
        board()->movePiece(colFrom, rankFrom, colTo, rankTo);
    }

    isCheck();

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

bool ChessAlgorithm::isCheck() {

    // Na razie testy algorytmu wykrywania szacha dla czarnego króla
    // Potem trzeba będzie rozdzielić to w zależności do koloru figury:
    // jeżeli ruch wykonują białe to sprawdzam szacha dla czarnego króla i odwrotnie.
    // Potem też trzeba będzie przy walidacji sprawdzać czy dany ruch nie powoduje szacha (odsłonięcie króla na szach)
    // w tym celu trzeba będzie przekazywać potencjalny stan tablicy do tej funkcji jako parametr
    // i będzie ona weryfikować czy dany potencjalny ruch (powodujący przekazane rozłożenie figur) powoduje szach
    // (tym razem na królu koloru tego samego co figura, która się rusza).

    struct {
        bool topLeft = false;
        bool top = false;
        bool topRight = false;
        bool right = false;
        bool bottomRight = false;
        bool bottom = false;
        bool bottomLeft = false;
        bool left = false;
        bool other = false;
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
    board()->getPiecePosition('k', kingCol, kingRank);

    // ----------- SZACH PRAWO ----------- //
    // Sprawdzenie jaka figura znajduje się po prawej stronie od króla
    for (int i = 1; i <= 7; i++) {
        piece.right = board()->data(kingCol + i, kingRank);
        if (piece.right != ' ') {
            break;
        }
    }
    if (piece.right == 'Q' || piece.right == 'R') {
        check.right = true;
    }
    else {
        check.right = false;
    }

    // ----------- SZACH LEWO ----------- //
    // Sprawdzenie jaka figura znajduje się po lewej stronie od króla
    for (int i = 1; i <= 7; i++) {
        piece.left = board()->data(kingCol - i, kingRank);
        if (piece.left != ' ') {
            break;
        }
    }
    if (piece.left == 'Q' || piece.left == 'R') {
        check.left = true;
    }
    else {
        check.left = false;
    }


    // ----------- SZACH GÓRA ----------- //
    // Sprawdzenie jaka figura znajduje się na górze od króla
    for (int i = 1; i <= 7; i++) {
        piece.top = board()->data(kingCol, kingRank + i);
        if (piece.top != ' ') {
            break;
        }
    }
    if (piece.top == 'Q' || piece.top == 'R') {
        check.top = true;
    }
    else {
        check.top = false;
    }

    // ----------- SZACH DÓŁ ----------- //
    // Sprawdzenie jaka figura znajduje się na dole od króla
    for (int i = 1; i <= 7; i++) {
        piece.bottom = board()->data(kingCol, kingRank - i);
        if (piece.bottom != ' ') {
            break;
        }
    }
    if (piece.bottom == 'Q' || piece.bottom == 'R') {
        piece.bottom = true;
    }
    else {
        piece.bottom = false;
    }



    qDebug() << " !!! checkRight - " << check.right;
    qDebug() << " !!! checkLeft - " << check.left;
    qDebug() << " !!! checkTop - " << check.top;
    qDebug() << " !!! checkBottom - " << check.bottom;

    return false;
}
