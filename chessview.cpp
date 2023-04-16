#include "chessview.h"
#include "qlabel.h"
#include <QFontMetrics>
#include <QPainter>
#include <QMouseEvent>

ChessView::ChessView(QWidget *parent)
    : QWidget{parent}
{

}

// Metoda odpowiedzialna za aktualizację szachownnicy
void ChessView::setBoard(ChessBoard *board)
{
    if(m_board == board) return;
    if(m_board) {
        // Rozłączenie wszystkich połączeń (signal-slot) między m_board i tym obiektem (this)
        m_board->disconnect(this);
        //qDebug() << "DISCONNECT";
    }
    m_board = board;
    // Utworzenie połączeń
    if(board){
        //qDebug() << "NOWA PLANSZA ";
        connect(board, SIGNAL(dataChanged(int,int)), this, SLOT(update()));
        connect(board, SIGNAL(boardReset()), this, SLOT(update()));
        connect(board, SIGNAL(currentPlayerChanged()), this, SLOT(updateLabels()));
        connect(board, SIGNAL(gameStateChanged(ChessBoard::GameState)), this, SLOT(updateLabels()));
        connect(board, SIGNAL(promotionChanged()), this, SLOT(showPromotionButtons()));

        connect(queenButton, &QPushButton::clicked, board, [board]{ board->setPromoteTo(ChessBoard::Queen); });
        connect(rookButton, &QPushButton::clicked, board, [board]{ board->setPromoteTo(ChessBoard::Rook); });
        connect(bishopButton, &QPushButton::clicked, board, [board]{ board->setPromoteTo(ChessBoard::Bishop); });
        connect(knightButton, &QPushButton::clicked, board, [board]{ board->setPromoteTo(ChessBoard::Knight); });

        connect(queenButton, SIGNAL(clicked()), this, SLOT(hidePromotionButtons()));
        connect(rookButton, SIGNAL(clicked()), this, SLOT(hidePromotionButtons()));
        connect(bishopButton, SIGNAL(clicked()), this, SLOT(hidePromotionButtons()));
        connect(knightButton, SIGNAL(clicked()), this, SLOT(hidePromotionButtons()));
    }

    setSideBar();
    updateGeometry();
}

void ChessView::setSideBar()
{
    QFont f("Arial", 14, QFont::Bold);
    currentPlayerLabel->setFont(f);
    //currentPlayerLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    currentPlayerLabel->setAlignment(Qt::AlignCenter);
    currentPlayerLabel->setGeometry(QRect(440,10,150,60));

    gameStateLabel->setFont(f);
    //gameStateLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    gameStateLabel->setAlignment(Qt::AlignCenter);
    gameStateLabel->setGeometry(QRect(440,80,150,40));

    newGameButton->setGeometry(QRect(440,360,150,40));
    newGameButton->setFont(f);
    newGameButton->setText("NOWA PARTIA");

    QFont f3("Arial", 12, QFont::Bold);

    promotionLabel->setFont(f3);
    //gameStateLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    promotionLabel->setAlignment(Qt::AlignCenter);
    promotionLabel->setGeometry(QRect(440,120,150,60));
    promotionLabel->setText("PROMOCJA\nWybierz figurę:");

    QFont f2("Arial", 10, QFont::Bold);
    queenButton->setFont(f2);
    queenButton->setGeometry(QRect(463,180,100,30));
    queenButton->setText("Hetman");
    rookButton->setFont(f2);
    rookButton->setGeometry(QRect(463,220,100,30));
    rookButton->setText("Wieża");
    bishopButton->setFont(f2);
    bishopButton->setGeometry(QRect(463,260,100,30));
    bishopButton->setText("Goniec");
    knightButton->setFont(f2);
    knightButton->setGeometry(QRect(463,300,100,30));
    knightButton->setText("Skoczek");

    hidePromotionButtons();
    //showPromotionButtons();

    updateLabels();
}

ChessBoard *ChessView::board() const
{
    return m_board;
}

QSize ChessView::fieldSize() const
{
    return m_fieldSize;
}

void ChessView::setFieldSize(const QSize &newFieldSize)
{
    if (m_fieldSize == newFieldSize)
        return;
    m_fieldSize = newFieldSize;
    emit fieldSizeChanged();
    updateGeometry();
}

QSize ChessView::sizeHint() const
{
    if(!m_board) return QSize(100,100);
    QSize boardSize = QSize(fieldSize().width() * m_board->columns() + 1, m_fieldSize.height() * m_board->ranks() + 1);
    int rankSize = fontMetrics().horizontalAdvance('M') + 200;
    int columnSize = fontMetrics().height() + 4;
    return boardSize+QSize(rankSize, columnSize);
}

QRect ChessView::fieldRect(int column, int rank) const
{
    if(!m_board) return QRect();
    const QSize fs = fieldSize();
    QRect fRect = QRect(QPoint((column-1)*fs.width(), (m_board->ranks()-rank)*fs.height()), fs);
    // offset rect by rank symbols
    int offset = fontMetrics().horizontalAdvance('M');
    // 'M' is the widest letter
    return fRect.translated(offset + 4, 0);
}

void ChessView::paintEvent(QPaintEvent *event)
{
    if(!m_board) return;
    QPainter painter(this);
    for (int r = m_board->ranks(); r > 0; --r) {
        painter.save();
        drawRank(&painter, r);
        painter.restore();
    }
    for (int c = 1; c <= m_board->columns(); ++c) {
        painter.save();
        drawColumn(&painter, c);
        painter.restore();
    }
    for(int r = 1; r <= m_board->ranks(); ++r) {
        for(int c = 1; c <= m_board->columns(); ++c) {
            painter.save();
            drawField(&painter, c, r);
            painter.restore();
        }
    }
    for(int r = m_board->ranks(); r > 0; --r) {
        for(int c = 1; c <= m_board->columns(); ++c) {
            drawPiece(&painter, c, r);
        }
    }
    drawHighlights(&painter);

}

void ChessView::drawRank(QPainter *painter, int rank)
{
    QRect r = fieldRect(1, rank);
    QRect rankRect = QRect(0, r.top(), r.left(), r.height()).adjusted(2, 0, -2, 0);
    QString rankText = QString::number(rank);
    painter->drawText(rankRect, Qt::AlignVCenter | Qt::AlignRight, rankText);
}

void ChessView::drawColumn(QPainter *painter, int column)
{
    QRect r = fieldRect(column, 1);
    QRect columnRect = QRect(r.left(), r.bottom(), r.width(), height()-r.bottom()).adjusted(0, 2, 0, -2);
    painter->drawText(columnRect, Qt::AlignHCenter | Qt::AlignTop, QChar('a'+column-1));
}

void ChessView::drawField(QPainter *painter, int column, int rank)
{
    QRect rect = fieldRect(column, rank);
    QColor fillColor = (column+rank) % 2 ? palette().color(QPalette::Light) : palette().color(QPalette::Mid);
    painter->setPen(palette().color(QPalette::Dark));
    painter->setBrush(fillColor);
    painter->drawRect(rect);
}

void ChessView::drawPiece(QPainter *painter, int column, int rank)
{
    QRect rect = fieldRect(column, rank);
    char value = m_board->data(column, rank);
    if(value != ' ') {
        QIcon icon = piece(value);
        if(!icon.isNull()) {
            icon.paint(painter, rect, Qt::AlignCenter);
        }
    }
}

void ChessView::setPiece(char type, const QIcon &icon)
{
    m_pieces.insert(type, icon);
    update();
}

QIcon ChessView::piece(char type) const
{
    return m_pieces.value(type, QIcon());
}

QPoint ChessView::fieldAt(const QPoint &pt) const
{
    if(!m_board) return QPoint();
    const QSize fs = fieldSize();
    int offset = fontMetrics().horizontalAdvance('M')+4;
    // 'M' is the widest letter
    if(pt.x() < offset) return QPoint();
    int c = (pt.x()-offset) / fs.width();
    int r = pt.y()/fs.height();
    if(c < 0 || c >= m_board->columns() || r<0 ||
            r >= m_board->ranks())
        return QPoint();
    return QPoint(c+1, m_board->ranks() - r);
    // max rank - r
}


void ChessView::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pt = fieldAt(event->pos());
    qDebug() << "CLICKED: " << pt;
    if(pt.isNull()) return;
    emit clicked(pt);
}

void ChessView::addHighlight(ChessView::Highlight *hl)
{
    m_highlights.append(hl);
    update();
}

void ChessView::removeHighlight(ChessView::Highlight *hl)
{
    m_highlights.removeOne(hl);
    update();
}

void ChessView::drawHighlights(QPainter *painter)
{
    for(int idx=0; idx < highlightCount(); ++idx) {
        Highlight *hl = highlight(idx);
        if(hl->type() == FieldHighlight::Type) {
            FieldHighlight *fhl = static_cast<FieldHighlight*>(hl);
            QRect rect = fieldRect(fhl->column(), fhl->rank());
            painter->fillRect(rect, fhl->color());
        }
    }
}

void ChessView::updateLabels()
{
    QString currentPlayer = board()->currentPlayer();
    if (currentPlayer == "") {
        currentPlayer = "RUCH - BIAŁE";
    }

    ChessBoard::GameState currentGameState = board()->gameState();
    QString currentGameStateText;
    if (currentGameState == ChessBoard::Check) {
        currentGameStateText = "SZACH!";
    }
    else if (currentGameState == ChessBoard::CheckMate) {
        currentGameStateText = "SZACH MAT!";
        if (currentPlayer == "RUCH - BIAŁE") {
            currentPlayer = "BIAŁE\nWYGRYWAJĄ!";
        }
        else {
            currentPlayer = "CZARNE\nWYGRYWAJĄ!";
        }
    }
    else {
        currentGameStateText = "";
    }

    currentPlayerLabel->setText(currentPlayer);
    gameStateLabel->setText(currentGameStateText);
}

void ChessView::showPromotionButtons()
{
    promotionLabel->setVisible(true);
    queenButton->setVisible(true);
    rookButton->setVisible(true);
    bishopButton->setVisible(true);
    knightButton->setVisible(true);
    currentPlayerLabel->setVisible(false);
}

void ChessView::hidePromotionButtons()
{
    promotionLabel->setVisible(false);
    queenButton->setVisible(false);
    rookButton->setVisible(false);
    bishopButton->setVisible(false);
    knightButton->setVisible(false);
    currentPlayerLabel->setVisible(true);
}
