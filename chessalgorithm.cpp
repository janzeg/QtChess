#include "chessalgorithm.h"
#include <QPoint>
#include <QDebug>

ChessAlgorithm::ChessAlgorithm(QObject *parent)
    : QObject{parent}
{
    m_board = nullptr;
    m_bufferBoard = nullptr; // ?????
    m_currentPlayer = PlayerWhite;
    m_result = NoResult;
}

ChessBoard *ChessAlgorithm::board() const
{
    return m_board;
}

ChessBoard *ChessAlgorithm::bufferBoard() const
{
    return m_bufferBoard;
}

void ChessAlgorithm::setBoard(ChessBoard *board)
{
    if(board == m_board) return;
    if(m_board) delete m_board;
    m_board = board;
    emit boardChanged(m_board);
}

void ChessAlgorithm::setBufferBoard(ChessBoard *bufferBoard)    // ??????
{
    if(bufferBoard == m_bufferBoard) return;
    if(m_bufferBoard) delete m_bufferBoard;
    m_bufferBoard = bufferBoard;
    emit boardChanged(m_bufferBoard);
}

// Utworzenie klasycznej szachownicy 8x8
void ChessAlgorithm::setupBoard()
{
    setBoard(new ChessBoard(this, 8,8));
    setBufferBoard(new ChessBoard(this, 8,8));
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

void ChessAlgorithm::copyBoardToBuffer()
{
    //bufferBoard()->m_boardData = board()->m_boardData;
    bufferBoard()->setBoardData(board()->boardData());
    //m_bufferBoard = m_board;
}

void ChessAlgorithm::copyBufferToBoard()
{
    //board()->m_boardData = bufferBoard()->m_boardData;
    board()->setBoardData(bufferBoard()->boardData());
    //m_board = m_bufferBoard;
}

/*
Pawn ChessAlgorithm::pawn() const
{
    return m_pawn;
}*/

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

    qDebug() << "Aktualny gracz - " << char(currentPlayer());

    // Określenie jaka figura została wybrana
    char source = board()->data(colFrom, rankFrom);
    //qDebug() << "SOURCE:" << source;

    // Określenie koloru figury
    char color;
    if (isupper(source) == true) { color = 'w'; }   // biały
    else { color = 'b'; }                           // czarny

    // Gracz może poruszać się tylko swoimi figurami
    if (color != currentPlayer()) {
        return 0;
    }

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

    if (currentPiece()->moveValid(colFrom, rankFrom, colTo, rankTo, board(), color)) {
        board()->movePiece(colFrom, rankFrom, colTo, rankTo);
        if (currentPlayer() == PlayerWhite) {
            setCurrentPlayer(PlayerBlack);
        }
        else {
            setCurrentPlayer(PlayerWhite);
        }
    }

    board()->isCheck('w');   // sprawdzenie czy jest szach na

    //m_bufferBoard = m_board;

    //qDebug() << "NORMAL BOARD: " << m_board->m_boardData;
    //qDebug() << "BUFFER BOARD: " << m_bufferBoard->m_boardData;

    static int cnt = 0;
    cnt++;

    /*
    if (cnt == 2) {
        copyBoardToBuffer();
        qDebug() << "copyBoardToBuffer...";
    }

    if (cnt == 4) {
        copyBufferToBoard();
        qDebug() << "copyBufferToBoard...";
    }*/


    return true;
}

bool ChessAlgorithm::move(const QPoint &from, const QPoint &to)
{
    qDebug() << "ChessAlgorithm::move";
    return move(from.x(), from.y(), to.x(), to.y());
}


