#include "huffman_tree.h"

HuffmanTree::HuffmanTree()
    : treeHead(nullptr)
{
    nodeTable = new TreeNode[256];
    for(int i = 0; i < 256; i++)
    {
        nodeTable[i].isLeaf = true;
    }
}

HuffmanTree::~HuffmanTree()
{

    delete_tree(treeHead);
    treeHead = nullptr;
    delete[] nodeTable;
    nodeTable = nullptr;
//    std::cout << "here?" << std::endl;
}

int HuffmanTree::delete_tree(TreeNode *head)
{
    if(head == nullptr) return 0;
    TreeNode *lcld = head->getLchild();
    TreeNode *rcld = head->getRchild();
    if(!lcld->isLeaf) delete_tree(lcld);
    if(!rcld->isLeaf) delete_tree(rcld);
    head->setLchild(nullptr);
    head->setRchild(nullptr);
    lcld = nullptr;
    rcld = nullptr;
    delete head;
    head = nullptr;
    return 0;
}

int HuffmanTree::countFrequency(QFile &fp)
{
    char byte;
    if (!fp.open(QIODevice::ReadOnly))
    {
        std::cerr << "Cannot open file for reading: "
                  << qPrintable(fp.errorString()) << std::endl;
        return -1;
    }
    QDataStream in(&fp);
    while(!fp.atEnd())
    {
        in.readRawData(&byte, 1);
        unsigned char ubyte;
        memcpy(&ubyte, &byte, 1);
        this->nodeTable[ubyte].add_1();
    }
    fp.close();
    return 0;
}


int HuffmanTree::createHuffmanTree()
{
    int list[256];
    for(int i = 0; i < 256; i++){ list[i] = i;}
    std::sort(list, list+256,
         [this](int node1, int node2) -> bool{ return this->nodeTable[node1] < this->nodeTable[node2];});
    TreeNodeNode *tchead = new TreeNodeNode();
    TreeNodeNode *p;
    TreeNodeNode *q;
    p = tchead;
    for(int i = 0; i < 256; i++)
    {
        if(this->nodeTable[list[i]].getFrequency() == 0) continue;
        q = new TreeNodeNode(&this->nodeTable[list[i]]);
        p->set_next(q);
        p = q;
        q = nullptr;
    }
    // 开始构建哈夫曼树
    while(true)
    {
        p = merge(tchead);
        if(p == nullptr) std::cerr << "merge error!" << std::endl;
        if(p == tchead) break;
    }
    p = tchead->get_next();
    this->treeHead = p->get_tnode();
    delete tchead;
    delete p;
    tchead = nullptr;
    p = nullptr;
    manageCode(this->treeHead, "");
    return 0;
}

int HuffmanTree::encodeFile(QString file_path, QString target_path)
{
    QProgressDialog progress;
    progress.setFixedWidth(600);
    progress.setMinimumDuration(0);
    progress.setRange(0,100);
    progress.setAutoClose(true);
    progress.setWindowTitle(QString::fromLocal8Bit("压缩进度"));
    progress.setLabel(new QLabel(QString::fromLocal8Bit("正在压缩...")));
    progress.setWindowModality(Qt::WindowModal);
    progress.open();
    progress.setValue(0);
    char byte;
    char bit_buffer[16];

    QFile pack(file_path);
    qint64 pack_size = pack.size();

    // 统计打包文件的词频
    this->countFrequency(pack);
    // 创建哈夫曼树
    this->createHuffmanTree();

    if (!pack.open(QIODevice::ReadOnly))
    {
        std::cerr << "Cannot open file for reading: "
                  << qPrintable(pack.errorString()) << std::endl;
        return -1;
    }
    QDataStream in(&pack);

    QFile tar(target_path);
    // open a empty tar file
    if (!tar.open(QIODevice::WriteOnly))
    {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(tar.errorString()) << std::endl;
        return -1;
    }
    // the write stream
    QDataStream out(&tar);
    // 魔数，用于区分不同的tar文件
    char magic[8] = {'H', 'U', 'F', 'F', 'M', 'A', 'N', 0};
    out.writeRawData(magic, 8);

    for(int i = 0; i < 256; i++)
    {
        if(this->nodeTable[i].getCode().compare("") == 0 && i!=255) continue;
        QString byte_code = "";
        byte_code += static_cast<char>(i);
        byte_code += " " + this->nodeTable[i].getCode() + '\n';
        QByteArray ba = byte_code.toLatin1();
        out.writeRawData(ba, byte_code.length());
    }

    int buffer_ptr = 0;
    qint64 read_size = 0;
    while(!pack.atEnd())
    {
        in.readRawData(&byte, 1);
        read_size++;
        if((read_size*100/pack_size) > ((read_size-1)*100/pack_size))
            progress.setValue((100 * read_size) / pack_size);
        unsigned char ubyte;
        memcpy(&ubyte, &byte, 1);
        QString code = this->nodeTable[ubyte].getCode();
        QByteArray ba = code.toLatin1();
        for(int i = 0; i < code.length(); i++)
        {
            bit_buffer[buffer_ptr] = ba[i];
            buffer_ptr++;
            if(buffer_ptr == 8)
            {
                byte = create_byte(bit_buffer);
                out.writeRawData(&byte, 1);
            }
            if(buffer_ptr == 16)
            {
                byte = create_byte(bit_buffer+8);
                out.writeRawData(&byte, 1);
                buffer_ptr = 0;
            }
        }
    }

    if(buffer_ptr > 0 && buffer_ptr < 8)
    {
        while(buffer_ptr < 8){ bit_buffer[buffer_ptr] = '0'; buffer_ptr++;}
        byte = create_byte(bit_buffer);
        out.writeRawData(&byte, 1);
    }
    if(buffer_ptr > 8 && buffer_ptr < 16)
    {
        while(buffer_ptr < 16){ bit_buffer[buffer_ptr] = '0'; buffer_ptr++;}
        byte = create_byte(bit_buffer+8);
        out.writeRawData(&byte, 1);
    }
    std::cout << "Compress finished." << std::endl;
    progress.setValue(100);
    pack.close();
    tar.close();
    pack.remove();
    return 0;
}

int manageCode(TreeNode *head, QString head_code)
{
    TreeNode *lcld = head->getLchild();
    TreeNode *rcld = head->getRchild();
    int leaf_count = 0;
    if(lcld->isLeaf){ lcld->setCode(head_code + "0"); leaf_count++;}
    else leaf_count += manageCode(lcld, head_code + "0");
    if(rcld->isLeaf){ rcld->setCode(head_code + "1"); leaf_count++;}
    else leaf_count += manageCode(rcld, head_code + "1");
    return leaf_count;
}

int HuffmanTree::decodeFile(QString file_path, QString target_path)
{
    QProgressDialog progress;
    progress.setFixedWidth(600);
    progress.setMinimumDuration(0);
    progress.setRange(0,100);
    progress.setWindowModality(Qt::WindowModal);
    progress.setAutoClose(true);
    progress.setWindowTitle(QString::fromLocal8Bit("解压进度"));
    progress.setLabel(new QLabel(QString::fromLocal8Bit("正在解压...")));
    progress.open();
    progress.setValue(0);
    QFile tar(file_path);
    if (!tar.open(QIODevice::ReadOnly))
    {
        std::cerr << "Cannot open file for reading: "
                  << qPrintable(tar.errorString()) << std::endl;
        return -1;
    }
    QDataStream in(&tar);

    qint64 tar_size = tar.size();

    QFile pack(target_path);
    // open a empty tar file
    if (!pack.open(QIODevice::WriteOnly))
    {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(pack.errorString()) << std::endl;
        return -1;
    }
    // the write stream
    QDataStream out(&pack);

    // 读取魔数
    char magic[8];
    in.readRawData(magic, 8);

    char huffman_buffer[64];
    int hbptr = 0;
    while(static_cast<unsigned char>(huffman_buffer[0]) != 0xff)
    {
        memset(huffman_buffer, 0, 64 * sizeof(char));
        while(huffman_buffer[hbptr-1] != '\n' || hbptr == 1)
        {
            if(in.atEnd()) std::cerr << "the file has been finished." << std::endl;
            in.readRawData(huffman_buffer+hbptr, 1);
            hbptr++;
            if(hbptr > 63) std::cerr << "out of range!" << std::endl;
        }
        QString code = "";
        for(int i = 2; i < hbptr-1; i++) code += huffman_buffer[i];
        this->nodeTable[static_cast<unsigned char>(huffman_buffer[0])].setCode(code);
        hbptr = 0;
    }
    TreeNode *tchead = new TreeNode();
    this->treeHead = tchead;
    make_child(tchead);

    char dbuffer[2];
    int dbptr = 0;
    TreeNode *treeptr = tchead;
    in.readRawData(dbuffer, 1);
    qint64 read_size = 1;
    while(!in.atEnd())
    {
        char bit = get_bit(dbuffer, dbptr);
        if(bit == '0') treeptr = treeptr->getLchild();
        else treeptr = treeptr->getRchild();
        if(treeptr->isLeaf)
        {
            char index;
            unsigned char uindex = static_cast<unsigned char>(treeptr-&nodeTable[0]);
            memcpy(&index, &uindex, 1);
            out.writeRawData(&index, 1);
            treeptr = tchead;
        }
        dbptr++;
        if(dbptr == 8)
        {
            in.readRawData(dbuffer+1, 1);
            read_size++;
            if((read_size*100/tar_size) > ((read_size-1)*100/tar_size))
                progress.setValue(((read_size*100) / tar_size));
        }
        if(dbptr == 16)
        {
            in.readRawData(dbuffer, 1);
            dbptr = 0; read_size++;
            if((read_size*100/tar_size) > ((read_size-1)*100/tar_size))
                progress.setValue(((100*read_size) / tar_size));
        }
    }
    progress.setValue(100);
    pack.close();
    tar.close();
    return 0;
}

int HuffmanTree::make_child(TreeNode *head)
{
    QString head_code = head->getCode();
    TreeNode *lcld = find_code(head_code + '0');
    TreeNode *rcld = find_code(head_code + '1');
    head->setLchild(lcld);
    head->setRchild(rcld);
    if(!lcld->isLeaf) make_child(lcld);
    if(!rcld->isLeaf) make_child(rcld);
    return 0;
}

TreeNode *HuffmanTree::find_code(QString code)
{
    for(int i = 0; i < 256; i++)
        if(this->nodeTable[i].getCode().compare(code) == 0) return &nodeTable[i];
    TreeNode *new_node = new TreeNode();
    new_node->setCode(code);
    return new_node;
}

