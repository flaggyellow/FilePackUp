#ifndef TAR_ARCHIVE_H
#define TAR_ARCHIVE_H

#include "file_node.h"
#include "file_helper.h"
#include <iostream>
#include <QDataStream>
#include <QProgressDialog>
#include <QLabel>

#define BLOCKSIZE 512

#define NORMALFILE 0
#define TARFILE 1

class TarArchive
{
public:
    int load(QString fileDir, int mode);
    int save(int mode);
    int add(QString filePath);
    int RLE(QString filePath, QString target_path);
    int deRLE(QString filePath, QString target_path);
    TarArchive(QString fileDir);
//    TarArchive();
    ~TarArchive();

private:
    FileNode *ntarContents;
    FileHelper tarFile;
    int fileCounts;
};

#endif // TAR_ARCHIVE_H
