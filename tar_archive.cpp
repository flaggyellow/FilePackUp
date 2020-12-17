#include "tar_archive.h"

int TarArchive::add(QString filePath, int type)
{
    if(ntarContents == nullptr)
    {
        ntarContents = new FileNode(filePath, type);
        fileCounts++;
    }
    else
    {
        FileNode *p = ntarContents;
        while(p->get_next() != nullptr) p = p->get_next();
        p->set_next(new FileNode(filePath, type));
        fileCounts++;
    }
    return 0;
}

int TarArchive::save(int mode)
{
    if (mode == 0)  // do not use huffman
    {
        if (!tarFile.get_file().open(QIODevice::WriteOnly))
        {
            std::cerr << "Cannot open file for writing: "
                      << qPrintable(tarFile.get_file().errorString()) << std::endl;
            return -1;
        }
        QDataStream out(&tarFile.get_file());
        FileNode *p = ntarContents;
        char data[BLOCKSIZE];
        memset(data, 0, BLOCKSIZE * sizeof(char));
        QFile *fp;
        while(p != nullptr)
        {
            memcpy(data, &p->get_ptar()->get_header(), BLOCKSIZE);
            out.writeRawData(data, 512);
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
            if(dptr > 0)
            {
                dptr = 0;
                out.writeRawData(data, 512);
                memset(data, 0, BLOCKSIZE * sizeof(char));
            }
            std::cout << "File " << &p->get_ptar()->get_header().fileName << " has been writen." << std::endl;
            p = p->get_next();
        }
        out.writeRawData(data, 512);
        out.writeRawData(data, 512);
    }
    return 0;
}

TarArchive::TarArchive(QString fileDir, int type)
    : ntarContents(nullptr)
    , tarFile(fileDir + "/pack.tar", type)
    ,fileCounts(0)
{}

TarArchive::TarArchive()
    : ntarContents(nullptr)
    , tarFile()
    ,fileCounts(0)
{}

TarArchive::~TarArchive(){}
