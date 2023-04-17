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
}

void ChessBoard::setColumns(int newColumns)
{
    if(columns() == newColumns) return;
    m_columns = newColumns;
    initBoard();
}

void ChessBoard::initBoard()
{
    m_boardData.fill(' ', ranks()*columns());
    char pieceTab[8] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

    for (int col = 1; col <= 8; col++) {
        setData(col, 1, pieceTab[col-1]);
    }
    for (int col = 1; col <= 8; col++) {
        setData(col, 2, 'P');
    }
    for (int rank = 3; rank <= 6; rank++) {
        for (int col = 1; col <= 8; col++) {
            setData(col, rank, ' ');
        }
    }
    for (int col = 1; col <= 8; col++) {
        setData(col, 7, 'p');
    }
    for (int col = 1; col <= 8; col++) {
        setData(col, 8, tolower(pieceTab[col-1]));
    }
}

// Określenie jaka figura znajduje się w danym polu (kolumna, wiersz) szachownicy
char ChessBoard::data(int column, int rank) const
{
    if (column > 8 || rank > 8 || column < 1 || rank < 1 ) {
        return 0;
    }
    return m_boardData.at((rank-1)*columns()+(column-1));
}

// Ustawienie figury w danym polu szachownicy (wpisanie odpowiedniego znaku do wektora m_boardData)
void ChessBoard::setData(int column, int rank, char value)
{
    if(setDataInternal(column, rank, value))
    emit dataChanged(column, rank);
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
    // Wyczyszczenie pola, z którego został wykonany ruch
    setData(fromColumn, fromRank, ' ');
}

char ChessBoard::getColor(int column, int rank)
{
    if (data(column, rank) == ' ') {
        return 'e'; // empty
    }

    if (isupper(data(column, rank)) == true) {
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

void ChessBoard::setCurrentPlayer(const ChessBoard::Color &newCurrentPlayer)
{
    if (m_currentPlayer == newCurrentPlayer)
        return;
    m_currentPlayer = newCurrentPlayer;
    emit currentPlayerChanged();
}

ChessBoard::Color ChessBoard::currentPlayer() const
{
    return m_currentPlayer;
}

void ChessBoard::setGameState(GameState newGameState)
{
    if (m_gameState == newGameState)
        return;
    m_gameState = newGameState;
    emit gameStateChanged(newGameState);
}

ChessBoard::GameState ChessBoard::gameState() const
{
    return m_gameState;
}

void ChessBoard::setPromotion(bool newPromotion)
{
    if (m_promotion == newPromotion)
        return;
    m_promotion = newPromotion;
    emit promotionChanged();
}

bool ChessBoard::promotion() const
{
    return m_promotion;
}

void ChessBoard::setPromoteTo(Piece newPromoteTo)
{
    m_promoteTo = newPromoteTo;

    char piece;
    switch(newPromoteTo) {
    case Queen:
        piece = 'Q';
        break;
    case Rook:
        piece = 'R';
        break;
    case Bishop:
        piece = 'B';
        break;
    case Knight:
        piece = 'N';
        break;
    }

    // W ten sposób, bo po wykonaniu ruchu aktualnym graczem będzie
    // przeciwny niż ten, dla którego ma miejsce promocja piona
    if (currentPlayer() == White) {
        piece = tolower(piece);
    }

    setData(promCol, promRank, piece);
    setPromotion(false);
}

ChessBoard::Piece ChessBoard::promoteTo() const
{
    return m_promoteTo;
}

void ChessBoard::getPiecePosition(char piece, int &column, int &rank) const
{
    int index = m_boardData.indexOf(piece);

    column = (index % 8) + 1;

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
    //qDebug() << "BOARD DATA - " << m_boardData;
    return m_boardData;
}

bool ChessBoard::isCheck(Color playerColor) {

    // Parametr color - mówi czy sprawdzamy szach na czarnych czy na białych
    char king = 'k';
    char queen = 'Q';
    char rook = 'R';
    char bishop = 'B';
    char knight = 'N';
    char pawn = 'P';

    // Jeżeli sprawdzamy czy jest szach na białych to zmieniamy figury na czarne
    if (playerColor == White) {
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
        piece.right = data(kingCol + i, kingRank);
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
        piece.left = data(kingCol - i, kingRank);
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
        piece.top = data(kingCol, kingRank + i);
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
        piece.bottom = data(kingCol, kingRank - i);
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
            piece.topLeft = data(kingCol - j, kingRank + j);
            if (piece.topLeft != ' ') {
                break;
            }
            j++;
        }
    }
    if ((piece.topLeft == queen || piece.topLeft == bishop) ||
        (playerColor == White && piece.topLeft == pawn && j == 1))
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
            piece.topRight = data(kingCol + j, kingRank + j);
            if (piece.topRight != ' ') {

                //qDebug() << "ALALALA" << piece.topRight;

                break;
            }
            j++;
        }
    }
    if ((piece.topRight == queen || piece.topRight == bishop) ||
        (playerColor == White && piece.topRight == pawn && j == 1))
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
            piece.bottomLeft = data(kingCol - j, kingRank - j);
            if (piece.bottomLeft != ' ') {
                break;
            }
            j++;
        }
    }
    if ((piece.bottomLeft == queen || piece.bottomLeft == bishop) ||
        (playerColor == Black && piece.bottomLeft == pawn && j == 1))
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
            piece.bottomRight = data(kingCol + j, kingRank - j);
            if (piece.bottomRight != ' ') {
                break;
            }
            j++;
        }
    }
    if ((piece.bottomRight == queen || piece.bottomRight == bishop) ||
        (playerColor == Black && piece.bottomRight == pawn && j == 1))
    {
        check.bottomRight = true;
    }
    else {
        check.bottomRight = false;
    }

    // ----------------- SZACH SKOCZEK ----------------- //
    if ((data(kingCol + 1, kingRank + 2) == knight) ||
        (data(kingCol - 1, kingRank + 2) == knight) ||
        (data(kingCol + 1, kingRank - 2) == knight) ||
        (data(kingCol - 1, kingRank - 2) == knight) ||
        (data(kingCol + 2, kingRank + 1) == knight) ||
        (data(kingCol + 2, kingRank - 1) == knight) ||
        (data(kingCol - 2, kingRank + 1) == knight) ||
        (data(kingCol - 2, kingRank - 1) == knight))
    {
        check.knight = true;
    }
    else {
        check.knight = false;
    }

    if (check.right || check.left || check.top || check.bottom || check.topLeft ||
        check.topRight || check.bottomLeft || check.bottomRight || check.knight)
    {
        return true;
    }
    else {
        return false;
    }
}
