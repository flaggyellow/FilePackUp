#include "tools.h"


int checkValidLocation(QString filepath, int mode)
{
    // mode 0: check files /mode 1: check dir
    if(mode == 1){
        QStringList dir = filepath.split(';');
        if (dir.length() < 0) return -1;
        for(int i = 0; i < dir.length(); i++) if( (!isFileExist(dir[i])) || !(isFile(dir[i])) ) return -2;
        return 0;
    }else if(mode == 0){
        if (filepath.length() < 0) return -1;
        if ( (!isFileExist(filepath)) || !(isDir(filepath)) ) return -2;
        return 0;
    }else{
        std::cerr << "checkValidLocation: uncorrect use." << std::endl;
        return -3;
    }
}

bool isFileExist(QString fullFilePath)
{
    QFile file(fullFilePath);
    if(file.exists()) return true;
    return false;
}

bool isFile(QString filepath)
{
    QFileInfo file(filepath);
    if(file.isFile()) return true;
    return false;
}
bool isDir(QString filepath)
{
    QFileInfo file(filepath);
    if(file.isDir()) return true;
    return false;
}

int cpQs2Ca(char *to, QString qs)
{
    std::string str = qs.toStdString();
    std::strcpy(to, str.c_str());
    return 0;
}

int fillOctCa(char *to, long long num)
{
    std::vector<int> ivec;
    long long Num = num;
    while (num != 0)
    {
        ivec.push_back(num % 8);
        num = num / 8;
    }
    std::vector<int>::size_type sz = ivec.size();
    for (std::vector<int>::size_type index = 0; index != sz; ++index)
        to[index] = numbers[ivec[sz - 1 - index]];
    return 0;
}

bool isEmptyBlock(char *datablock, int size)
{
    for(int ptr = 0; ptr < size; ptr++)
    {
        if(datablock[ptr] != 0x00) return false;
    }
    return true;
}
