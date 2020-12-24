#ifndef TAR_CONTENTS_H
#define TAR_CONTENTS_H

#define NORTYPE  '0'            /* normal file */
#define LNKTYPE  '1'            /* hard link */
#define SYMTYPE  '2'            /* symbolic link */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */

#include "file_helper.h"
#include <QFileInfo>
#include "tools.h"
#include <QDateTime>

struct TarHeader
{
    char fileName[100];
    char fileMode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
};

class TarContents
{
public:
    TarContents(QString filePath, int type);
    TarContents(int type);
    ~TarContents();

    void get_filename();
    void get_size();
    void get_mtime();
    void get_chksum();

    void set_filename();
    void set_size();
    void set_mtime();
    void set_chksum();

    TarHeader *get_header();
    FileHelper &get_fileHelper();

private:
    struct TarHeader *header;
    FileHelper file;
};

#endif // TAR_CONTENTS_H
