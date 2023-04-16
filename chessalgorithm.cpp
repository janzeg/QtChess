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
    if(m_board) {
        m_board->disconnect();
        delete m_board;
    }
    m_board = board;
    if (board) {
        connect(board, SIGNAL(startNewGame()), this, SLOT(newGame())); // testy
    }
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

    // testy
    if (currentPlayer() == PlayerWhite) {
        board()->setCurrentPlayer("BIAŁE");
    }
    else {
        board()->setCurrentPlayer("CZARNE");
    }
}

void ChessAlgorithm::copyBoardToBuffer()
{
    //bufferBoard()->m_boardData = board()->m_boardData;
    bufferBoard()->setBoardData(board()->boardData());
    //m_bufferBoard = m_board;
}

void ChessAlgorithm::copyBufferToBoard()
{
    board()->setBoardData(bufferBoard()->boardData());

}


void ChessAlgorithm::setCastlingCond(char piece, bool value)
{
    if (piece == 'k') {
        m_castlingCond.bKingMoved = value;
    }
    else if (piece == 'K') {
        m_castlingCond.wKingMoved = value;
    }
    else if (piece == 'r') {
        m_castlingCond.bRookMoved = value;
    }
    else if (piece == 'R') {
        m_castlingCond.wRookMoved = value;
    }
}

ChessAlgorithm::castlingCondType ChessAlgorithm::castlingCond() const
{
    return m_castlingCond;
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


    // Przypadek specjalny - próba wykonania roszady (oddzielna walidacja)
    bool castlingTry = false;
    if ((piece == 'K' && colFrom == 5 && rankFrom == 1 && (colTo == 7 || colTo == 3)) ||
        (piece == 'k' && colFrom == 5 && rankFrom == 8 && (colTo == 7 || colTo == 3)))
    {
        //qDebug() << "PRÓBA ROSZADY";
        castlingTry = true;
    }

    // Walidacja nr 1 - sprawdzenie czy ruch jest wykonalny pod kątem możliwości figury i ułożenia innych figur na szachownicy
    if (currentPiece()->moveValid(colFrom, rankFrom, colTo, rankTo, board(), bufferBoard(), color) == true)  {
        // Wykonanie ruchu na buforowej szachownicy
        bufferBoard()->movePiece(colFrom, rankFrom, colTo, rankTo);
    }
    else if (castlingTry == true) {
        // Walidacja roszady
        if (validCastling(colFrom, rankFrom, colTo, rankTo, color) == true) {
            // Wykonanie ruchu na buforowej szachownicy
            bufferBoard()->movePiece(colFrom, rankFrom, colTo, rankTo);
        }
        else {
            return 0;
        }
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

    // Przy roszadzie również ruch wieżą
    if (castlingTry == true) {
        // Krótka
        if (colTo == 7) {
            board()->movePiece(8, rankFrom, colTo - 1, rankTo);
        }
        // Długa
        else if (colTo == 3) {
            board()->movePiece(1, rankFrom, colTo + 1, rankTo);
        }
    }

    // Wpisanie do szachownicy buforowej z powrotem aktualnego stanu szachownicy
    copyBoardToBuffer();

    // Zapamiętujemy ruch królem lub wieżą (konieczne do roszady)
    if (tolower(piece) == 'k' || tolower(piece) == 'K') {
        setCastlingCond(piece, true);
    }

    char oppositePlayer;
    if (currentPlayer() == 'w') {
        oppositePlayer = 'b';
    }
    else {
         oppositePlayer = 'w';
    }

    // Sprawdzenie czy jest szach na graczu przeciwnym
    if (board()->isCheck(oppositePlayer) == true) {
        qDebug() << "SZACH NA " << char(oppositePlayer) << "!!!!!!";
        board()->setGameState(ChessBoard::Check);
        // Sprawdzenie czy jest mat
        if (isCheckMate(oppositePlayer) == true) {
            qDebug() << "MAT NA " << char(oppositePlayer) << "!!!!!!";
            board()->setGameState(ChessBoard::CheckMate);
            return 0;
        }
    }
    else {
        board()->setGameState(ChessBoard::Normal);
    }

    // Sprawdzenie czy któryś z graczy jest w pacie
    if (isDeadLock(PlayerWhite)) {
        qDebug() << "PAT BIAŁE";
        return 0;
    }
    if (isDeadLock(PlayerBlack)) {
        qDebug() << "PAT CZARNE";
        return 0;
    }

    // Zmiana gracza wykonującego ruch
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

bool ChessAlgorithm::validCastling(int colFrom, int rankFrom, int colTo, int rankTo, char color) {

    // Color - kolor gracza, dla którego sprawdzam czy może wykonać roszadę

    bool castlingOk = true;

    copyBoardToBuffer();

    enum castlingType {shortC = 1, longC =  2} castlingVariant;

    // Określenie typu roszady (krótka/długa)
    if (colTo == 7) {
        castlingVariant = shortC;
    }
    else if (colTo == 3) {
        castlingVariant = longC;
    }

    if (color == 'w') {
        // Sprawdzenie czy król lub wieża wykonały już ruch w tej partii
        if (castlingCond().wKingMoved == true || castlingCond().wRookMoved == true) {
            return false;
        }
    }
    else if (color == 'b') {
        // Sprawdzenie czy król lub wieża wykonały już ruch w tej partii
        if (castlingCond().bKingMoved == true || castlingCond().bRookMoved == true) {
            return false;
        }
    }

    if (castlingVariant == shortC) {
        // Sprawdzenie czy między królem a wieżą nie ma innych figur oraz ...
        // ... sprawdzenie czy pola przez, które przejdzie król nie są atakowane przez przeciwnika
        for (int i = 1; i <= 2; i++) {
            if (board()->data(colFrom + i, rankFrom) != ' ') {
                //qDebug() << "!!!!! KRÓTKA !!!!!";
                //qDebug() << "!!!!! ROSZADA - COŚ STOI POMIĘDZY !!!!!";
                return false;
            }
            bufferBoard()->movePiece(colFrom, rankFrom, colFrom + i, rankFrom);
            if (bufferBoard()->isCheck(color) == true) {
                copyBoardToBuffer();
                //qDebug() << "!!!!! ROSZADA - SZACH !!!!!";
                return false;
            }
            else {
                copyBoardToBuffer();
            }
        }
    }
    else if (castlingVariant == longC) {
        // Sprawdzenie czy między królem a wieżą nie ma innych figur oraz ...
        // ... sprawdzenie czy pola przez, które przejdzie król nie są atakowane przez przeciwnika
        for (int i = 1; i <= 3; i++) {
            if (board()->data(colFrom - i, rankFrom) != ' ') {
                //qDebug() << "!!!!! DŁUGA !!!!!";
                //qDebug() << "!!!!! ROSZADA - COŚ STOI POMIĘDZY !!!!!";
                return false;
            }
            bufferBoard()->movePiece(colFrom, rankFrom, colFrom + i, rankFrom);
            if (bufferBoard()->isCheck(color) == true) {
                copyBoardToBuffer();
                //qDebug() << "!!!!! ROSZADA - SZACH !!!!!";
                return false;
            }
            else {
                copyBoardToBuffer();
            }
        }
    }

    return castlingOk;
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

bool ChessAlgorithm::isDeadLock(char color) {

    // Color - kolor gracza, dla którego sprawdzam czy jest pat

    copyBoardToBuffer();

    bool deadLock = true;

    // Sprawdzenie czy istnieje jakiś ruch gracza, który jest wykonalny
    for (int pieceCol = 1; pieceCol <= 8; pieceCol++) {
        for (int pieceRank = 1; pieceRank <= 8; pieceRank++) {

            char piece = board()->data(pieceCol, pieceRank);

            // Figura nie może być pusta oraz musi należeć do gracza, dla którego sprawdzany jest pat
            if (piece == ' ' || board()->getColor(piece) != color) {
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

                            //qDebug() << "pieceCol - " << pieceCol;
                            //qDebug() << "pieceRank - " << pieceRank;
                            //qDebug() << "piece - " << piece;
                            //qDebug() << "col - " << col;
                            //qDebug() << "rank - " << rank;

                            deadLock = false;
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
    return deadLock;
}
