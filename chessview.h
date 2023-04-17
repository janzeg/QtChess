#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QWidget>
#include "chessboard.h"
#include <QPointer>
#include <QLabel>
#include <QPushButton>

class ChessView : public QWidget
{
    Q_OBJECT

public:
    explicit ChessView(QWidget *parent = nullptr);
    void setBoard(ChessBoard *board);
    ChessBoard* board() const;
    QSize fieldSize() const;
    void setFieldSize(const QSize &newFieldSize);
    void setPiece(char type, const QIcon &icon);
    QIcon piece(char type) const;
    QPoint fieldAt(const QPoint &pt) const;
    void mouseReleaseEvent(QMouseEvent *event);
    void setSideBar();

    QLabel *currentPlayerLabel = new QLabel(this);
    QLabel *gameStateLabel = new QLabel(this);
    QPushButton *newGameButton = new QPushButton(this);

    QPushButton *queenButton = new QPushButton(this);
    QPushButton *rookButton = new QPushButton(this);
    QPushButton *bishopButton = new QPushButton(this);
    QPushButton *knightButton = new QPushButton(this);
    QLabel *promotionLabel = new QLabel(this);


    class Highlight {
    public:
        Highlight() {}
        virtual ~Highlight() {}
        virtual int type() const { return 0; }
    };

    class FieldHighlight : public Highlight {
    public:
        enum { Type = 1 };
        FieldHighlight(int column, int rank, QColor color)
            : m_field(column, rank), m_color(color) {}
        inline int column() const { return m_field.x(); }
        inline int rank() const { return m_field.y(); }
        inline QColor color() const { return m_color; }
        int type() const { return Type; }
    private:
        QPoint m_field;
        QColor m_color;
    };

    void addHighlight(Highlight *hl);
    void removeHighlight(Highlight *hl);
    inline Highlight *highlight(int index)
    const {return m_highlights.at(index); }
    inline int highlightCount() const { return m_highlights.size(); }
    void drawHighlights(QPainter *painter);

signals:
    void fieldSizeChanged();
    void clicked(const QPoint &);

private:
    QPointer<ChessBoard> m_board;
    QSize m_fieldSize;
    QMap<char,QIcon> m_pieces;
    QList<Highlight*> m_highlights;

protected:
    QSize sizeHint() const;
    QRect fieldRect(int column, int rank) const;
    virtual void paintEvent(QPaintEvent *event);
    virtual void drawRank(QPainter *painter, int rank);
    virtual void drawColumn(QPainter *painter, int column);
    virtual void drawField(QPainter *painter, int column, int rank);
    virtual void drawPiece(QPainter *painter, int column, int rank);

public slots:
    void updateLabels();
    void showPromotionButtons();
    void hidePromotionButtons();
};

#endif // CHESSVIEW_H
