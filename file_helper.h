#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include "tools.h"
#include <QFile>

#define NORMALFILE 0
#define TARFILE 1


class FileHelper
{
public:
    void readFile();
    void writeFile();
    void getFrequency();
    FileHelper(QString filePath, int type);
    FileHelper();
    ~FileHelper();

    QFile &get_file();
    int get_fileType();

private:
    QFile file;
    int fileType;
};

#endif // FILE_HELPER_H
