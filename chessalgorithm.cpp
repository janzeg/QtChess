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

void ChessAlgorithm::setCurrentPiece(char newCurrentPiece)
{
    switch (tolower(newCurrentPiece))
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

    copyBoardToBuffer();

    qDebug() << " --- AKTUALNY GRACZ --- : " << char(currentPlayer());

    // Określenie jaka figura została wybrana
    char piece = board()->data(colFrom, rankFrom);
    //qDebug() << "SOURCE:" << source;

    // Określenie koloru figury
    char color;
    if (isupper(piece) == true) { color = 'w'; }   // biały
    else { color = 'b'; }                          // czarny


    // Gracz może poruszać się tylko swoimi figurami
    if (color != currentPlayer()) {
        return 0;
    }

    // Ustawienie aktualnej figury
    setCurrentPiece(piece);

    // Walidacja nr 1 - sprawdzenie czy ruch jest wykonalny pod kątem możliwości figury i ułożenia innych figur na szachownicy
    if (currentPiece()->moveValid(colFrom, rankFrom, colTo, rankTo, board(), bufferBoard(), color) == true)  {
        // Wykonanie ruchu na buforowej szachownicy
        bufferBoard()->movePiece(colFrom, rankFrom, colTo, rankTo);
    }
    else {
        return 0;
    }

    // Walidacja nr 2 - sprawdzenie czy ruch nie powoduje szacha na graczu, który wykonuje ruch (odsłonięcia króla na szach)
    if (bufferBoard()->isCheck(currentPlayer()) == true) {
        // Wpisanie do szachownicy buforowej z powrotem aktualnego stanu szachownicy
        copyBoardToBuffer();
        return 0;
    }

    // Jeżeli walidacja przeszła pomyślnie to można wykonać ruch już na rzeczywistej szachownicy
    board()->movePiece(colFrom, rankFrom, colTo, rankTo);
    // Wpisanie do szachownicy buforowej z powrotem aktualnego stanu szachownicy
    copyBoardToBuffer();


    char oppositePlayer;
    if (currentPlayer() == 'w') {
        oppositePlayer = 'b';
    }
    else {
         oppositePlayer = 'w';
    }

    // Sprawdzenie czy jest szach
    if (board()->isCheck(oppositePlayer) == true) {
        qDebug() << "SZACH NA " << char(oppositePlayer) << "!!!!!!";
        // Sprawdzenie czy jest mat
        if (isCheckMate(oppositePlayer) == true) {
            qDebug() << "MAT NA " << char(oppositePlayer) << "!!!!!!";
        }
    }

    if (currentPlayer() == PlayerWhite) {
        setCurrentPlayer(PlayerBlack);
    }
    else {
        setCurrentPlayer(PlayerWhite);
    }

    return true;
}

bool ChessAlgorithm::move(const QPoint &from, const QPoint &to)
{
    //qDebug() << "ChessAlgorithm::move";
    return move(from.x(), from.y(), to.x(), to.y());
}

bool ChessAlgorithm::isCheckMate(char color) {

    // Color - kolor gracza, dla którego sprawdzam czy jest mat

    copyBoardToBuffer();

    bool checkMate = true;

    char king = 'k';
    if (color == 'w') {
        king = toupper(king);
    }
    int kingCol, kingRank;


    board()->getPiecePosition(king, kingCol, kingRank);

    // Sprawdzenie czy król może uciec od szacha
    for (int col = kingCol - 1; col <= kingCol + 1; col++) {
        for (int rank = kingRank - 1; rank <= kingRank + 1; rank++) {
            // Walidacja nr 1
            setCurrentPiece(&m_king);
            if (currentPiece()->moveValid(kingCol, kingRank, col, rank, board(), bufferBoard(), color) == true) {
                // Wykonanie ruchu na buforowej szachownicy
                bufferBoard()->movePiece(kingCol, kingRank, col, rank);
                // Walidacja nr 2
                if (bufferBoard()->isCheck(color) == false) {
                    checkMate = false;
                    goto exit;
                }
                // Wpisanie do szachownicy buforowej z powrotem aktualnego stanu szachownicy
                copyBoardToBuffer();
            }
        }
    }

    // Sprawdzenie czy jakikolwiek ruch gracza pozwoli uniknąć szacha
    for (int pieceCol = 1; pieceCol <= 8; pieceCol++) {
        for (int pieceRank = 1; pieceRank <= 8; pieceRank++) {

            char piece = board()->data(pieceCol, pieceRank);

            // Figura nie może być pusta oraz musi należeć do gracza, dla którego sprawdzany jest mat
            if (piece == ' ' || board()->getColor(piece) != color || tolower(piece) == 'k' ) {
                continue;
            }

            // Ustawienie aktualnej figury
            setCurrentPiece(piece);

            for (int col = 1; col <= 8; col++) {
                for (int rank = 1; rank <= 8; rank++) {

                    // Walidacja nr 1
                    if (currentPiece()->moveValid(pieceCol, pieceRank, col, rank, board(), bufferBoard(), color) == true) {
                        // Wykonanie ruchu na buforowej szachownicy
                        bufferBoard()->movePiece(pieceCol, pieceRank, col, rank);

                        // Walidacja nr 2
                        if (bufferBoard()->isCheck(color) == false) {
                            copyBoardToBuffer();
                            checkMate = false;
                            goto exit;
                        }
                        // Wpisanie do szachownicy buforowej z powrotem aktualnego stanu szachownicy
                        copyBoardToBuffer();
                    }
                }
            }
        }
    }
    exit:
    return checkMate;
}
