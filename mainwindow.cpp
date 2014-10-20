#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pMainCtrl = new CMainCtrl();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    dlgSetParams dlg(pMainCtrl, this);
    dlg.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    dlgSetParams2 dlg(this);
    dlg.exec();
}
