#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_view = new ChessView;

    m_view->setPiece('P', QIcon(":/chessIcons/Chess_plt45.png")); // pawn
    m_view->setPiece('K', QIcon(":/chessIcons/Chess_klt45.png")); // king
    m_view->setPiece('Q', QIcon(":/chessIcons/Chess_qlt45.png")); // queen
    m_view->setPiece('R', QIcon(":/chessIcons/Chess_rlt45.png")); // rook
    m_view->setPiece('N', QIcon(":/chessIcons/Chess_nlt45.png")); // knight
    m_view->setPiece('B', QIcon(":/chessIcons/Chess_blt45.png")); // bishop

    m_view->setPiece('p', QIcon(":/chessIcons/Chess_pdt45.png")); // pawn
    m_view->setPiece('k', QIcon(":/chessIcons/Chess_kdt45.png")); // king
    m_view->setPiece('q', QIcon(":/chessIcons/Chess_qdt45.png")); // queen
    m_view->setPiece('r', QIcon(":/chessIcons/Chess_rdt45.png")); // rook
    m_view->setPiece('n', QIcon(":/chessIcons/Chess_ndt45.png")); // knight
    m_view->setPiece('b', QIcon(":/chessIcons/Chess_bdt45.png")); // bishop

    m_algorithm = new ChessAlgorithm(this);
    //m_algorithm = new FoxAndHounds(this); // F&H

    m_algorithm->newGame();
    m_view->setBoard(m_algorithm->board());
    setCentralWidget(m_view);
    m_view->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_view->setFieldSize(QSize(50,50));
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    connect(m_view, SIGNAL(clicked(QPoint)), this, SLOT(viewClicked(QPoint)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::viewClicked(const QPoint &field)
{
    if(m_clickPoint.isNull()) {
        if(m_view->board()->data(field.x(), field.y()) != ' ') {
            m_clickPoint = field;
            m_selectedField = new ChessView::FieldHighlight(field.x(), field.y(), QColor(255, 0, 0, 50));
            m_view->addHighlight(m_selectedField);
        }
    } else {
        if(field != m_clickPoint){
            m_algorithm->move(m_clickPoint, field); // F&H
            //m_view->board()->movePiece(m_clickPoint.x(), m_clickPoint.y(), field.x(), field.y());
        }
        m_clickPoint = QPoint();
        m_view->removeHighlight(m_selectedField);
        delete m_selectedField;
        m_selectedField = 0;
    }
}
