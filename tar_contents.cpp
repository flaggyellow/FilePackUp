#include "tar_contents.h"

TarHeader &TarContents::get_header(){return *(this->header);}
FileHelper &TarContents::get_fileHelper(){return this->file;}


TarContents::TarContents(QString filePath, int type)
    : file(filePath, type)
{
    header = static_cast<struct TarHeader*>(malloc(sizeof(struct TarHeader)));
    std::memset(header, 0, sizeof(TarHeader));
    QFileInfo info(filePath);
    cpQs2Ca(header->fileName, filePath);
    qint64 file_size = info.size();
    // fillOctCa(header->size, file_size);
    memcpy(header->size, &file_size, 8);
    qint64 modified_time = info.lastModified().toSecsSinceEpoch();
    memcpy(header->mtime, &modified_time, 8);
}

//TarContents::TarContents()
//    : file()
//{
//    header = static_cast<struct TarHeader*>(malloc(sizeof(struct TarHeader)));
//    std::memset(header, 0, sizeof(TarHeader));
//}

TarContents::~TarContents()
{
    free(header);
}
