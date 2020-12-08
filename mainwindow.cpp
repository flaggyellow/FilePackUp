#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ui->input_source->text();
    // connect(ui->convert_source_select_button, SIGNAL(clicked()), this, SLOT(send_text()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::send_text(){
//    ui->convert_input_source->setText("Hello World!");
//    QFileDialog *win1 = new QFileDialog(this);
//    win1->show();
//}

