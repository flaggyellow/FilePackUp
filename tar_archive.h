#ifndef TAR_ARCHIVE_H
#define TAR_ARCHIVE_H

#include "file_node.h"
#include "file_helper.h"
#include <iostream>
#include <QDataStream>

#define BLOCKSIZE 512

class TarArchive
{
public:
    int load();
    int save(int mode);
    int add(QString filePath, int type);
    int get();
    TarArchive(QString fileDir, int type);
//    TarArchive();
    ~TarArchive();

private:
    FileNode *ntarContents;
    FileHelper tarFile;
    int fileCounts;
};

#endif // TAR_ARCHIVE_H
