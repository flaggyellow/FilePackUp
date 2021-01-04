#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <QString>

class TreeNode
{
public:
    TreeNode(bool isleaf=false);
    TreeNode(int freq, TreeNode *lcld, TreeNode *rcld, bool isleaf=false);
    bool isLeaf;
    int getFrequency();
    QString getCode();
    TreeNode *getLchild();
    TreeNode *getRchild();
    int setFrequency(int frequency);
    int setCode(QString code);
    int setLchild(TreeNode *node);
    int setRchild(TreeNode *node);

    int add_1();
    bool operator> (TreeNode& node);
    bool operator< (TreeNode& node);
    bool operator>= (TreeNode& node);
    bool operator<= (TreeNode& node);
    TreeNode& operator+ (TreeNode& node);

private:
    int frequency;
    QString code;
    TreeNode *lchild;
    TreeNode *rchild;
};

#endif // TREE_NODE_H
