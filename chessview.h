#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QWidget>
#include "chessboard.h"
#include <QPointer>
#include <QLabel>

class ChessView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize fieldSize READ fieldSize WRITE setFieldSize NOTIFY fieldSizeChanged)
    //Q_ENUMS(GameState)

public:
    //enum GameState { Normal, Check };
    explicit ChessView(QWidget *parent = nullptr);
    void setBoard(ChessBoard *board);
    ChessBoard* board() const;
    QSize fieldSize() const;
    void setFieldSize(const QSize &newFieldSize);
    void setPiece(char type, const QIcon &icon);
    QIcon piece(char type) const;
    QPoint fieldAt(const QPoint &pt) const; // nie wiem czy na pewno public???
    void mouseReleaseEvent(QMouseEvent *event); // nie wiem czy na pewno public???

    void addLabel(); // TESTY

    QLabel *currentPlayerLabel = new QLabel(this);
    QLabel *gameStateLabel = new QLabel(this);


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
    void drawHighlights(QPainter *painter); // nie wiem czy na pewno public

signals:
    void fieldSizeChanged();
    void clicked(const QPoint &);

private:
    QPointer<ChessBoard> m_board;
    QSize m_fieldSize;
    QMap<char,QIcon> m_pieces;
    QList<Highlight*> m_highlights;

protected:
    QSize sizeHint() const; // nie wiem czy na pewno w protected
    QRect fieldRect(int column, int rank) const;  // nie wiem czy na pewno w protected
    virtual void paintEvent(QPaintEvent *event);
    virtual void drawRank(QPainter *painter, int rank);
    virtual void drawColumn(QPainter *painter, int column);
    virtual void drawField(QPainter *painter, int column, int rank);
    virtual void drawPiece(QPainter *painter, int column, int rank);

public slots:
    void setSideBar(); // TESTY
};



#endif // CHESSVIEW_H
