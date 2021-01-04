#ifndef FILE_NODE_H
#define FILE_NODE_H

#include "tar_contents.h"
#include "tree_node.h"

class FileNode
{
public:
    TarContents *get_ptar();
    int set_ptar(TarContents *ptar);
    FileNode *get_next();
    int set_next(FileNode *n);
    FileNode(QString filePath, int type);
    FileNode(int type);
    ~FileNode();
private:
    TarContents *ptarContents;
    FileNode *next;
};

class TreeNodeNode
{
public:
    TreeNode *get_tnode();
    int set_tnode(TreeNode *tnode);
    TreeNodeNode *get_next();
    int set_next(TreeNodeNode *n);
    TreeNodeNode();
    TreeNodeNode(TreeNode *tnode);
    ~TreeNodeNode();

private:
    TreeNode *treenode;
    TreeNodeNode *next;
};

TreeNodeNode *merge(TreeNodeNode* head);

#endif // FILE_NODE_H
