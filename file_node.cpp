#include "file_node.h"

TarContents *FileNode::get_ptar(){ return this->ptarContents;}
FileNode *FileNode::get_next(){ return this->next;}
int FileNode::set_ptar(TarContents *ptar){ this->ptarContents = ptar; return 0;}
int FileNode::set_next(FileNode *n){ this->next = n; return 0;}

FileNode::FileNode(QString filePath, int type)
    : next(nullptr)
{
    ptarContents = new TarContents(filePath, type);
}

FileNode::FileNode(int type)
    : next(nullptr)
{
    ptarContents = new TarContents(type);
}

FileNode::~FileNode(){
    delete ptarContents;
}
