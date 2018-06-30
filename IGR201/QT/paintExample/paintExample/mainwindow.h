#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QColorDialog>
#include <QInputDialog>
#include "paintarea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QAction *rectangle, *ellipse;

private slots:
void choosepenColor();
void choosepenWidth();
void choosestate(QAction* sender);

signals:
   // QAction *rectangle, *ellipse;

private:
    PaintArea *paintArea;



};

#endif // MAINWINDOW_H
