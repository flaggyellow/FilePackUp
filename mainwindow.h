#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    QString fetch_file_path(int mode);
};
#endif // MAINWINDOW_H
