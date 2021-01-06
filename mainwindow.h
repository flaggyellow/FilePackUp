#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include "tar_archive.h"
#include "file_helper.h"
#include "tools.h"
#include <iostream>
#include "huffman_tree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void fetch_file_path_convert_source();
    void fetch_file_path_convert_target();
    void fetch_file_path_recover_source();
    void fetch_file_path_recover_target();
    void get_fold_contents_source();
    void get_fold_contents_target();
    void convert_start();
    void recover_start();
    void verify_start();
//    void recover_start();
//    void verify_start();

private:
    Ui::MainWindow *ui;
    QString fetch_file_path(int mode);
    QStringList get_fold_contents(QString DirPath);
};
#endif // MAINWINDOW_H
