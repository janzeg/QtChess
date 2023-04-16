#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessalgorithm.h"
#include "chessview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void viewClicked(const QPoint &field);
    void newGamePushBtn();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void newGame();




private:
    Ui::MainWindow *ui;
    ChessView *m_view;
    ChessAlgorithm *m_algorithm;
    QPoint m_clickPoint;
    ChessView::FieldHighlight *m_selectedField;

};
#endif // MAINWINDOW_H
