#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit_3->setPlainText("ab");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    obj.putText(ui->plainTextEdit->toPlainText());
    obj.putKey(ui->plainTextEdit_3->toPlainText());
    obj.encipher();
    ui->plainTextEdit_4->setPlainText(obj.Result());
}

void MainWindow::on_pushButton_2_clicked()
{
    obj.putText(ui->plainTextEdit_2->toPlainText());
    //obj.putKey(ui->plainTextEdit_3->toPlainText());
    //哈哈哈哈
    obj.decipher();
    ui->plainTextEdit_5->setPlainText(obj.Result());
}
