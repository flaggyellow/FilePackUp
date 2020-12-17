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
// **********************************************************
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fetch_file_path_convert_source(){ui->convert_input_source->setText(fetch_file_path(1));}
void MainWindow::fetch_file_path_convert_target(){ui->convert_input_target->setText(fetch_file_path(0));}
void MainWindow::fetch_file_path_recover_source(){ui->recover_input_source->setText(fetch_file_path(1));}
void MainWindow::fetch_file_path_recover_target(){ui->recover_input_target->setText(fetch_file_path(0));}
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
    if((checkValidLocation(source_path)<0) || (checkValidLocation(target_path)<0))
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("源路径或目标路径有误"));
    QStringList filePaths = source_path.split(';');

    TarArchive tarArchive(target_path, TARFILE);
    // HuffmanTree huffman();

    int fileptr = 0;
    while(fileptr < filePaths.length())
    {
        tarArchive.add(filePaths[fileptr], NORMALFILE);
        fileptr++;
    }
    std::cout << "The file nodes successfully loaded." << std::endl;
    tarArchive.save(0);
}

