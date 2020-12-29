#include "tar_archive.h"

int TarArchive::add(QString filePath)
{
    if(ntarContents == nullptr)
    {
        ntarContents = new FileNode(filePath, NORMALFILE);
        fileCounts++;
    }
    else
    {
        FileNode *p = ntarContents;
        while(p->get_next() != nullptr) p = p->get_next();
        p->set_next(new FileNode(filePath, NORMALFILE));
        fileCounts++;
    }
    return 0;
}

int TarArchive::save(int mode)
{
    if (mode == 0)  // do not use huffman
    {
        // open a empty tar file
        if (!tarFile.get_file().open(QIODevice::WriteOnly))
        {
            std::cerr << "Cannot open file for writing: "
                      << qPrintable(tarFile.get_file().errorString()) << std::endl;
            return -1;
        }
        // the write stream
        QDataStream out(&tarFile.get_file());

        // 魔数，用于区分不同的tar文件
        char magic[8] = {'N', 'O', 'R', 'M', 'A', 'L', 0, 0};
        out.writeRawData(magic, 8);

        // get the chain of normal files
        FileNode *p = ntarContents;

        // the write buffer for a data block
        char data[BLOCKSIZE];
        memset(data, 0, BLOCKSIZE * sizeof(char));

        // file pointer
        QFile *fp;

        // saves file by file
        while(p != nullptr)
        {
            // output the file header
            memcpy(data, p->get_ptar()->get_header(), BLOCKSIZE);
            out.writeRawData(data, 512);

            //output the file content by data block
            fp = &p->get_ptar()->get_fileHelper().get_file();
            if (!fp->open(QIODevice::ReadOnly))
            {
                std::cerr << "Cannot open file for reading: "
                          << qPrintable(tarFile.get_file().errorString()) << std::endl;
                return -1;
            }
            QDataStream in(fp);
            int dptr = 0;
            memset(data, 0, BLOCKSIZE * sizeof(char));
            while(!fp->atEnd())
            {
                in.readRawData(data+dptr, 1);
                dptr++;
                if(dptr >= 512)
                {
                    dptr = 0;
                    out.writeRawData(data, 512);
                    memset(data, 0, BLOCKSIZE * sizeof(char));
                }
            }

            // if the file is finished, output the data in the buffer
            if(dptr > 0)
            {
                dptr = 0;
                out.writeRawData(data, 512);
                memset(data, 0, BLOCKSIZE * sizeof(char));
            }
            std::cout << "File " << p->get_ptar()->get_header()->fileName << " has been writen." << std::endl;
            fp->close();
            p = p->get_next();
        }
        // output two empty block
        out.writeRawData(data, 512);
        out.writeRawData(data, 512);
        tarFile.get_file().close();
        std::cout << "convert finished." << std::endl << std::endl;
    }
    return 0;
}

int TarArchive::load(QString fileDir)
{
    // read tar file
    QFile *tar = &tarFile.get_file();
    if (!tar->open(QIODevice::ReadOnly))
    {
        std::cerr << "Cannot open file for reading: "
                  << qPrintable(tarFile.get_file().errorString()) << std::endl;
        return -1;
    }
    QDataStream in(tar);

    // prepare a 512-byte size read buffer
    char rbuffer[BLOCKSIZE];
    memset(rbuffer, 0, BLOCKSIZE * sizeof(char));

    //读取魔数判断是否哈夫曼编码压缩
    char magic[8];
    in.readRawData(magic, 8);
    if(checkMagic(magic) == 1)
    {
        in.readRawData(rbuffer, 512);

        // start to make the chain of normal files
        ntarContents = new FileNode(NORMALFILE);
        FileNode *fnode = ntarContents;
        while(!isEmptyBlock(rbuffer, 512))
        {
            // get the header information
            struct TarHeader *fheader = fnode->get_ptar()->get_header();
            memcpy(fheader, rbuffer, 512);
            QString filepath(fheader->fileName);
            QString filename(filepath.split('/').constLast());
            qint64 filesize;
            memcpy(&filesize, fheader->size, 8);

            // fill the file path in filehelper
            QFile *fp = &fnode->get_ptar()->get_fileHelper().get_file();
            fp->setFileName(fileDir + "/" + filename);

            // read file contents and recover files
            qint64 restsize = filesize;
            if (!fp->open(QIODevice::WriteOnly))
            {
                std::cerr << "Cannot open file ["
                          << fileDir.toStdString()
                          << "/"
                          << filename.toStdString()
                          << "] for writing: "
                          << qPrintable(tarFile.get_file().errorString()) << std::endl;

                return -1;
            }
            // the write stream
            QDataStream out(fp);

            while(restsize > 512)
            {
                in.readRawData(rbuffer, 512);
                out.writeRawData(rbuffer, 512);
                restsize -= 512;
            }

            if(restsize > 0)
            {
                in.readRawData(rbuffer, 512);
                out.writeRawData(rbuffer, static_cast<int>(restsize));
            }

            fp->close();
            std::cout << "File " << fileDir.toStdString() << "/" << filename.toStdString() << " has been writen." << std::endl;
            in.readRawData(rbuffer, 512);
            fnode->set_next(new FileNode(NORMALFILE));
            fnode = fnode->get_next();
        }
        // delete the extra filenode
        FileNode *p = ntarContents;
        if(p == fnode) return 0;
        FileNode *q = p->get_next();
        while(q != fnode)
        {
            p = q;
            q = p->get_next();
        }
        delete q;
        p->set_next(nullptr);
        tar->close();
    }
    else if(checkMagic(magic) < 0)
    {
        std::cerr << "unknown magic word..." << std::endl;
        return -1;
    }
    std::cout << "recover finished." << std::endl << std::endl;
    return 0;
}

TarArchive::TarArchive(QString fileDir)
    : ntarContents(nullptr)
    ,tarFile(fileDir, TARFILE)
    ,fileCounts(0)
{}

//TarArchive::TarArchive()
//    : ntarContents(nullptr)
//    , tarFile()
//    ,fileCounts(0)
//{}

TarArchive::~TarArchive(){
    FileNode *p = ntarContents;
    FileNode *q;
    while(p != nullptr)
    {
        q = p->get_next();
        delete p;
        p = q;
    }
}
