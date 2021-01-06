#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "tree_node.h"
#include <QDataStream>
#include <QFile>
#include "file_node.h"
#include <QByteArray>
#include "tools.h"
#include <QProgressDialog>
#include <QLabel>

#define BLOCKSIZE 512

class HuffmanTree
{
public:
    HuffmanTree();
    ~HuffmanTree();
    int countFrequency(QFile &fp);
    int createHuffmanTree();
    int encodeFile(QString file_path, QString target_path);
    int decodeFile(QString file_path, QString target_path);
    int delete_tree(TreeNode *head);
    int make_child(TreeNode *head);
    TreeNode *find_code(QString code);

private:
    TreeNode *nodeTable;
    TreeNode *treeHead;
};

int manageCode(TreeNode *head, QString head_code);


#endif // HUFFMAN_TREE_H
