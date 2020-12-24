#ifndef TAR_ARCHIVE_H
#define TAR_ARCHIVE_H

#include "file_node.h"
#include "file_helper.h"
#include <iostream>
#include <QDataStream>

#define BLOCKSIZE 512

#define NORMALFILE 0
#define TARFILE 1

class TarArchive
{
public:
    int load(QString fileDir);
    int save(int mode);
    int add(QString filePath);
    TarArchive(QString fileDir);
//    TarArchive();
    ~TarArchive();

private:
    FileNode *ntarContents;
    FileHelper tarFile;
    int fileCounts;
};

#endif // TAR_ARCHIVE_H
