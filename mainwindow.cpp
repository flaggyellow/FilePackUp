#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

// ******************* connects *****************************
    //select button
    connect(ui->convert_source_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_convert_source()));
    connect(ui->convert_target_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_convert_target()));
    connect(ui->recover_source_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_recover_source()));
    connect(ui->recover_target_select_button, SIGNAL(clicked()), this, SLOT(fetch_file_path_recover_target()));
    //start button
    connect(ui->convert_start, SIGNAL(clicked()), this, SLOT(convert_start()));
    connect(ui->recover_start, SIGNAL(clicked()), this, SLOT(recover_start()));
// **********************************************************
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fetch_file_path_convert_source(){ui->convert_input_source->setText(fetch_file_path(1));}
void MainWindow::fetch_file_path_convert_target(){ui->convert_input_target->setText(fetch_file_path(0));}
void MainWindow::fetch_file_path_recover_source(){ui->recover_input_source->setText(fetch_file_path(2));}
void MainWindow::fetch_file_path_recover_target(){ui->recover_input_target->setText(fetch_file_path(0));}
QString MainWindow::fetch_file_path(int mode)   //mode 0:directory only / 1:file only
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QString::fromLocal8Bit("选择文件路径..."));
    if(mode == 0){
        fileDialog->setFileMode(QFileDialog::Directory);
    }else if(mode == 1){
        fileDialog->setFileMode(QFileDialog::ExistingFiles);
    }else{
        fileDialog->setFileMode(QFileDialog::ExistingFile);
    }
    fileDialog->setDirectory("C:/Users/11251/Desktop/test");
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList filePath;
    if (fileDialog->exec()) filePath = fileDialog->selectedFiles();
    QString paths = "";
    for(int i = 0; i < filePath.length(); i++)
    {
        if(i > 0) paths += ";";
        paths += (filePath[i]);
    }
    return paths;
}
void MainWindow::convert_start()
{
    QString source_path = ui->convert_input_source->text();
    QString target_path = ui->convert_input_target->text();
    if((checkValidLocation(source_path, 1)<0) || (checkValidLocation(target_path, 0)<0))
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("源路径或目标路径有误"));
        ui->convert_status_label->setText(QString::fromLocal8Bit("请重新选择源路径或目标路径"));
        return;
    }
    QStringList filePaths = source_path.split(';');

    TarArchive tarArchive(target_path + "/pack.tar");
    // HuffmanTree huffman();

    int fileptr = 0;
    while(fileptr < filePaths.length())
    {
        tarArchive.add(filePaths[fileptr]);
        fileptr++;
    }
    std::cout << "The file nodes successfully loaded." << std::endl;
    tarArchive.save(0);
    ui->convert_status_label->setText(QString::fromLocal8Bit("已打包完成！"));
    return;
}

void MainWindow::recover_start(){
    QString source_path = ui->recover_input_source->text();
    QString target_path = ui->recover_input_target->text();
    if((checkValidLocation(source_path, 1)<0) || (checkValidLocation(target_path, 0)<0))
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("源路径或目标路径有误"));
        ui->recover_status_label->setText(QString::fromLocal8Bit("请重新选择源路径或目标路径"));
        return;
    }
    if(source_path.split(';').length() > 1 || QString::compare( source_path.split('.').constLast(), "tar") != 0)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("源路径只能选择一个tar文件"));
        ui->recover_status_label->setText(QString::fromLocal8Bit("请重新选择源路径 "));
        return;
    }
    TarArchive tarArchive(source_path);
    tarArchive.load(target_path);
    ui->recover_status_label->setText(QString::fromLocal8Bit("解包已完成！"));
    return;
}
