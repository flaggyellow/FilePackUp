#ifndef FILE_NODE_H
#define FILE_NODE_H

#include "tar_contents.h"

class FileNode
{
public:
    TarContents *get_ptar();
    int set_ptar(TarContents *ptar);
    FileNode *get_next();
    int set_next(FileNode *n);
    FileNode(QString filePath, int type);
    FileNode();
    ~FileNode();
private:
    TarContents *ptarContents;
    FileNode *next;
};

#endif // FILE_NODE_H
