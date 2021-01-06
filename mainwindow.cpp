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
    connect(ui->verify_source_button, SIGNAL(clicked()), this, SLOT(get_fold_contents_source()));
    connect(ui->verify_target_button, SIGNAL(clicked()), this, SLOT(get_fold_contents_target()));
    //start button
    connect(ui->convert_start, SIGNAL(clicked()), this, SLOT(convert_start()));
    connect(ui->recover_start, SIGNAL(clicked()), this, SLOT(recover_start()));
    connect(ui->verify_start, SIGNAL(clicked()), this, SLOT(verify_start()));
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
void MainWindow::get_fold_contents_source()
{
    ui->source_list->clear();
    QString fold_path = fetch_file_path(0);
    if((checkValidLocation(fold_path, 0)<0))
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("源路径选择错误 "));
        ui->label->setText(QString::fromLocal8Bit("请重新选择源路径 "));
        return;
    }
    ui->label_source->setText(fold_path);
    QStringList filelist = get_fold_contents(fold_path);
    for(QString filePath : filelist)
    {
        ui->source_list->addItem(new QListWidgetItem(filePath));
    }
}

void MainWindow::get_fold_contents_target()
{
    ui->target_list->clear();
    QString fold_path = fetch_file_path(0);
    if((checkValidLocation(fold_path, 0)<0))
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("目标路径选择错误"));
        ui->label->setText(QString::fromLocal8Bit("请重新选择目标路径"));
        return;
    }
    ui->label_target->setText(fold_path);
    QStringList filelist = get_fold_contents(fold_path);
    for(QString filePath : filelist)
    {
        ui->target_list->addItem(new QListWidgetItem(filePath));
    }
}

QStringList MainWindow::get_fold_contents(QString DirPath)
{
    QDir *dir = new QDir(DirPath);
    QStringList list;
    dir->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList filelist = dir->entryInfoList();
    dir->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList dirlist = dir->entryInfoList();
    for(QFileInfo info: filelist) list.append(info.filePath());
    for(QFileInfo info: dirlist) list += get_fold_contents(info.filePath());
    return list;
}

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
    bool useHuffman = ui->convert_radioButton->isChecked();
    bool useRLE = ui->convert_radioButton_2->isChecked();
    if((checkValidLocation(source_path, 1)<0) || (checkValidLocation(target_path, 0)<0))
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("源路径或目标路径有误"));
        ui->convert_status_label->setText(QString::fromLocal8Bit("请重新选择源路径或目标路径"));
        return;
    }
    QStringList filePaths = source_path.split(';');

    TarArchive tarArchive(target_path + "/pack.tar");

    HuffmanTree huffman;

    int fileptr = 0;
    while(fileptr < filePaths.length())
    {
        tarArchive.add(filePaths[fileptr]);
        fileptr++;
    }
    std::cout << "The file nodes successfully loaded." << std::endl;
    if(!useHuffman && !useRLE)
    {
        tarArchive.save(0);
        ui->convert_status_label->setText(QString::fromLocal8Bit("已打包完成！"));
    }
    else if(useHuffman)
    {
        tarArchive.save(1);
        huffman.encodeFile(target_path + "/pack.tar", target_path + "/compress.tar");
        ui->convert_status_label->setText(QString::fromLocal8Bit("已压缩完成！"));
    }
    else if(useRLE)
    {
        tarArchive.save(2);
        tarArchive.RLE(target_path + "/pack.tar", target_path + "/compress.tar");
        ui->convert_status_label->setText(QString::fromLocal8Bit("已压缩完成！"));
    }
    else
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("出现未知原因错误：模式选择"));
    }

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
    if(checkMagic(source_path) == 1)
    {
        TarArchive tarArchive(source_path);
        tarArchive.load(target_path, 1);
        ui->recover_status_label->setText(QString::fromLocal8Bit("解包已完成！"));
    }
    else if(checkMagic(source_path) == 2)
    {
        HuffmanTree Huffman;
        QString pack_path = source_path + ".tmp";
        Huffman.decodeFile(source_path, pack_path);
        TarArchive tarArchive(pack_path);
        tarArchive.load(target_path, 2);
        ui->recover_status_label->setText(QString::fromLocal8Bit("解压已完成！"));
    }
    else if(checkMagic(source_path) == 3)
    {
        QString pack_path = source_path + ".tmp";
        TarArchive tarArchive(pack_path);
        tarArchive.deRLE(source_path, pack_path);
        tarArchive.load(target_path, 3);
        ui->recover_status_label->setText(QString::fromLocal8Bit("解压已完成！"));
    }
    else
    {
        std::cerr << "unknown magic word..." << std::endl;
    }
    return;
}

void MainWindow::verify_start()
{
    QString source_dir = ui->label_source->text();
    QString target_dir = ui->label_target->text();
    QStringList source_list;
    QStringList target_list;
    for(int i = 0; i < ui->source_list->count(); i++) source_list.append(ui->source_list->item(i)->text());
    for(int i = 0; i < ui->target_list->count(); i++) target_list.append(ui->target_list->item(i)->text());
    if(ui->source_list->count() == ui->target_list->count())
    {
        for(int i = 0; i < ui->source_list->count(); i++)
        {
            if(source_list[i].mid(source_dir.length(), -1).compare(target_list[i].mid(target_dir.length(), -1)) != 0)
            {
                ui->label->setText(QString::fromLocal8Bit("目录树结构不同！"));
                return;
            }
        }
        for(int i = 0; i < source_list.count(); i++)
        {
            QFile *source, *target;
            source = new QFile(source_list[i]);
            target = new QFile(target_list[i]);
            if (!source->open(QIODevice::ReadOnly))
            {
                std::cerr << "Cannot open file for reading: "
                          << qPrintable(source->errorString()) << std::endl;
                return;
            }
            QDataStream source_in(source);
            if (!target->open(QIODevice::ReadOnly))
            {
                std::cerr << "Cannot open file for reading: "
                          << qPrintable(target->errorString()) << std::endl;
                return;
            }
            QDataStream target_in(target);
            while(!source->atEnd() && !target->atEnd())
            {
                char source_buffer;
                char target_buffer;
                source_in.readRawData(&source_buffer,1);
                target_in.readRawData(&target_buffer,1);
                if(source_buffer != target_buffer)
                {
                    QString filename = source_list[i].mid(source_dir.length(), -1);
                    QString report = "源路径和目标路径中的 \"" + filename + "\" 文件内容不同";
                    ui->label->setText(report);
                    return;
                }
            }
            if(!source->atEnd() || !target->atEnd())
            {
                QString filename = source_list[i].mid(source_dir.length(), -1);
                QString report = "源路径和目标路径中的 \"" + filename + "\" 文件长度不同";
                ui->label->setText(report);
                return;
            }
        }
        ui->label->setText(QString::fromLocal8Bit("验证完毕，源目录与目标目录相同！"));
    }
    else
    {
        ui->label->setText(QString::fromLocal8Bit("文件的数量不同！"));
        return;
    }
}
