#include "tree_node.h"

TreeNode::TreeNode(bool isleaf)
    : isLeaf(isleaf)
    , frequency(0)
    , code("")
    , lchild(nullptr)
    , rchild(nullptr) {}
TreeNode::TreeNode(int freq, TreeNode *lcld, TreeNode *rcld, bool isleaf)
    : isLeaf(isleaf)
    , frequency(freq)
    , code("")

{
    this->lchild = lcld;
    this->rchild = rcld;
}

int TreeNode::getFrequency(){ return this->frequency;}
QString TreeNode::getCode(){ return this->code;}
TreeNode *TreeNode::getLchild(){ return this->lchild;}
TreeNode *TreeNode::getRchild(){ return this->rchild;}
int TreeNode::setFrequency(int freq){ this->frequency = freq; return 0;}
int TreeNode::setCode(QString c){ this->code = c; return 0;}
int TreeNode::setLchild(TreeNode *node){ this->lchild = node; return 0;}
int TreeNode::setRchild(TreeNode *node){ this->rchild = node; return 0;}
int TreeNode::add_1(){ return ++this->frequency;}

bool TreeNode::operator>(TreeNode& node){ return this->frequency > node.frequency;}
bool TreeNode::operator<(TreeNode& node){ return this->frequency < node.frequency;}
bool TreeNode::operator>=(TreeNode& node){ return this->frequency >= node.frequency;}
bool TreeNode::operator<=(TreeNode& node){ return this->frequency <= node.frequency;}
TreeNode& TreeNode::operator+(TreeNode &node)
{
    TreeNode *new_node;
    int freq = this->frequency + node.frequency;
    new_node = new TreeNode(freq, this, &node);
    return *new_node;
}
