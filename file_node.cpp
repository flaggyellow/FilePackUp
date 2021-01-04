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

TreeNode *TreeNodeNode::get_tnode(){ return this->treenode;}
int TreeNodeNode::set_tnode(TreeNode *tnode){ this->treenode = tnode; return 0;}
TreeNodeNode *TreeNodeNode::get_next(){ return this->next;}
int TreeNodeNode::set_next(TreeNodeNode *n){ this->next = n; return 0;}
TreeNodeNode::TreeNodeNode() : treenode(nullptr), next(nullptr){}
TreeNodeNode::TreeNodeNode(TreeNode *tnode) : treenode(tnode),next(nullptr){}

TreeNodeNode::~TreeNodeNode()
{
    this->next = nullptr;
    this->treenode = nullptr;
}

TreeNodeNode *merge(TreeNodeNode* head)
{
    TreeNodeNode *p;
    TreeNodeNode *q;
    TreeNodeNode *r;
    if(head == nullptr){ return nullptr;}
    p = head->get_next();
    if(p == nullptr){ return nullptr;}
    q = p->get_next();
    if(q == nullptr){ return head;}
    r = q->get_next();
    head->set_next(r);
    r = new TreeNodeNode(&(*p->get_tnode() + *q->get_tnode()));
    delete p;
    delete q;
    p = head;
    q = p->get_next();
    while(q != nullptr)
    {
        if(*q->get_tnode() > *r->get_tnode()) break;
        p = q;
        q = p->get_next();
    }
    p->set_next(r);
    r->set_next(q);
    return r;
}
