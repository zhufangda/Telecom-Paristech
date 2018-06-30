#include<QIODevice>
#include<QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTextEdit>
#include<QFileDialog>
#include<QString>
#include<iostream>
#include<QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    QMenuBar *menuBar = this->menuBar();
    QMenu *fileMenu = menuBar->addMenu(tr("&Fichier"));

    //Open Action
    QAction *openAction = new QAction(QIcon(":images/open.png"),tr("&Open"),this);
    openAction->setShortcut(tr("Ctrl+N"));
    openAction->setToolTip(tr("Opne a file."));
    openAction->setStatusTip(tr("Open a file."));
    fileMenu -> addAction(openAction);

    //Save Action
    QAction *saveAction = new QAction(QIcon(":icone/save.png"),tr("&Save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setToolTip(tr("Save a file."));
    saveAction->setStatusTip(tr("Save a file."));
    fileMenu -> addAction(saveAction);


    //Quit Action
    QAction *quitAction = new QAction(QIcon(":icone/quit.png"),tr("&Quit"),this);
    quitAction->setShortcut(tr("Ctrl+S"));
    quitAction->setToolTip(tr("Quit a file."));
    quitAction->setStatusTip(tr("Quit a file."));
    fileMenu -> addAction(quitAction);

    //Add Toolbar

    QToolBar *openBar = this->addToolBar(tr("Open"));
    openBar->addAction(openAction);

    QToolBar *saveBar = this->addToolBar(tr("Save"));
    saveBar->addAction(saveAction);

    QToolBar *quitBar = this->addToolBar(tr("Quit"));
    quitBar->addAction(quitAction);

    //Add Text Area
    QTextEdit *text = new QTextEdit(this);
    setCentralWidget(text);

    statusBar()->showMessage(tr("Status Bar"));


    //Connection
    connect(openAction, SIGNAL(triggered()),this,SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()),this,SLOT(quitApp()));


}

void MainWindow::openFile()
{
    qDebug() << "Open File";
    QFileDialog dialog(this);
    QStringList fileNames;

    if(dialog.exec() == QDialog::Accepted){
        fileNames = dialog.selectedFiles();
        QString firstName = fileNames[0];
        std::cout << "Open File:" << qPrintable(firstName) << std::endl;

        QFile file(firstName);
        if(!file.open(QIODevice::ReadOnly)){
            return; //TODO give erro information
        }

        QTextStream text = file.readAll();
        file.close();
    }

}

void MainWindow::saveFile()
{
    qDebug() << "Save File";
    QString firstName = QFileDialog::getSaveFileName(this,
                                                     tr("Save File"),
                                                     "/save",
                                                     tr("Text File(*.html,*.xml,*.txx,*)")
                                                     );

     std::cout << "Save File:" << qPrintable(firstName) << std::endl;
}

void MainWindow::quitApp()
{
    qDebug()<< "Quit app";
}

MainWindow::~MainWindow()
{
    delete ui;
}
