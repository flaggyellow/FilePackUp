#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connects
    connect(ui->convert_source_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_convert_source()));
    connect(ui->convert_target_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_convert_target()));
    connect(ui->recover_source_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_recover_source()));
    connect(ui->recover_target_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_recover_target()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fetch_file_path_convert_source()
{
    ui->convert_input_source->setText(fetch_file_path(0));
}
void MainWindow::fetch_file_path_convert_target()
{
    ui->convert_input_target->setText(fetch_file_path(0));
}
void MainWindow::fetch_file_path_recover_source()
{
    ui->recover_input_source->setText(fetch_file_path(1));
}
void MainWindow::fetch_file_path_recover_target()
{
    ui->recover_input_target->setText(fetch_file_path(0));
}
QString MainWindow::fetch_file_path(int mode)   //mode 0:directory only / 1:file only
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QString::fromLocal8Bit("选择文件路径..."));
    if(mode == 0)
    {
        fileDialog->setFileMode(QFileDialog::Directory);
    }else{
        fileDialog->setFileMode(QFileDialog::ExistingFiles);
    }
    fileDialog->setDirectory("C:/");
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList filePath;
    if (fileDialog->exec())
    {
        filePath = fileDialog->selectedFiles();
    }
    if (filePath.length() > 0) return filePath[0];
    else return "";
}
