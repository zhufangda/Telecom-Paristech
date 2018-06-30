#include "mainwindow.h"
#include "paintarea.h"
#include "ui_mainwindow.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
                       QMainWindow(parent)

{
          //PaintArea(new PaintArea(this));
          paintArea=new PaintArea;
          setCentralWidget(paintArea);

          QMenuBar *menuBar=this->menuBar();
          QMenu *fileMenu=menuBar->addMenu(tr("&File"));

          // work for "penColor" button
          QToolBar *toolBar1=this->addToolBar(tr("File"));
          QAction *PenColor=new QAction(QIcon(":new.png"),tr("&penColor"),this);
          PenColor->setShortcut(tr("Ctrl+C"));
          PenColor->setToolTip(tr("Color"));
          PenColor->setStatusTip(tr("Color"));
          fileMenu->addAction(PenColor);
          toolBar1->addAction(PenColor);
          connect(PenColor,SIGNAL(triggered()),this,SLOT(choosepenColor()));

          //work for "penWidth" button
          QToolBar *toolBar2=this->addToolBar(tr("File^ missing"));
          QAction *PenWidth=new QAction(QIcon(":save.png"),tr("&penWidth"),this);
          PenWidth->setShortcut(tr("Ctrl+W"));
          PenWidth->setToolTip(tr("Width"));
          PenWidth->setStatusTip(tr("Width"));
          fileMenu->addAction(PenWidth);
          toolBar2->addAction(PenWidth);
          connect(PenWidth,SIGNAL(triggered()),this,SLOT(choosepenWidth()));



          QActionGroup *group=new QActionGroup(this);


          QAction *line=new QAction(QIcon(":quit.png"),tr("line"),this);
          line->setShortcut(tr("Ctrl+L"));
          line->setToolTip(tr("line"));
          line->setStatusTip(tr("line"));
          line->setCheckable(true);
          group->addAction(line);

          QAction *ellipse=new QAction(QIcon(":quit.png"),tr("ellipse"),this);
          ellipse->setShortcut(tr("Ctrl+E"));
          ellipse->setToolTip(tr("eclipse"));
          ellipse->setStatusTip(tr("eclipse"));
          ellipse->setCheckable(true);
          group->addAction(ellipse);

          QAction *rectangle=new QAction(QIcon(":quit.png"),tr("&rectangle"),this);
          rectangle->setShortcut(tr("Ctrl+R"));
          rectangle->setToolTip(tr("rectangle"));
          rectangle->setStatusTip(tr("rectangle"));
          rectangle->setCheckable(true);
          group->addAction(rectangle);


          connect(group,SIGNAL(triggered(QAction*)),this,SLOT(choosestate(QAction*)));

          QToolBar *toolBar3=this->addToolBar(tr("File"));

          fileMenu->addAction(line);
          toolBar3->addAction(line);

          //work for "quit" button
          QToolBar *toolBar4=this->addToolBar(tr("File"));

          fileMenu->addAction(ellipse);
          toolBar4->addAction(ellipse);
          //connect(newAction3,SIGNAL(triggered()),this,SLOT(quitApp()));

          //work for "rectangle" button
          QToolBar *toolBar5=this->addToolBar(tr("File"));

          fileMenu->addAction(rectangle);
          toolBar5->addAction(rectangle);

    }


void MainWindow::choosepenColor()
{
    QColor newColor = QColorDialog::getColor(paintArea->penColor());
    if (newColor.isValid())
        paintArea->setPenColor(newColor);
}

void MainWindow::choosepenWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        paintArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        paintArea->setPenWidth(newWidth);
}

void MainWindow::choosestate(QAction* sender){
    int choice;
    if(sender->iconText() == "line") {
        choice=1;
        qDebug() << "choice="<<choice;}
    if(sender->iconText() == "ellipse") {
        choice=2;
        qDebug() << "choice="<<choice;}
    else if(sender->iconText()=="rectangle") {
        choice=3;
     qDebug() << "choice="<<choice;}
     paintArea->setState(choice);
    //return choice;
}

MainWindow::~MainWindow()
{
}




