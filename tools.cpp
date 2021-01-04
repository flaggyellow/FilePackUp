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

int checkMagic(char *magic)
{
    char normal[8] = {'N','O','R','M','A','L',0,0};
    char huffman[8] = {'H','U','F','F','M','A','N',0};
    if(memcmp(magic, normal, 8) == 0) return 1;
    else if(memcmp(magic, huffman, 8) == 0) return 2;
    else return -1;
}

int checkMagic(QString file_path)
{
    QFile tar(file_path);
    if (!tar.open(QIODevice::ReadOnly))
    {
        std::cerr << "Cannot open file for reading: "
                  << qPrintable(tar.errorString()) << std::endl;
        return -1;
    }
    QDataStream in(&tar);
    //读取魔数判断是否哈夫曼编码压缩
    char magic[8];
    in.readRawData(magic, 8);
    tar.close();
    char normal[8] = {'N','O','R','M','A','L',0,0};
    char huffman[8] = {'H','U','F','F','M','A','N',0};
    if(memcmp(magic, normal, 8) == 0) return 1;
    else if(memcmp(magic, huffman, 8) == 0) return 2;
    else return -1;
}

char create_byte(char* buffer)
{
    char byte = 0;
    for(int i = 0; i < 8; i++)
    {
        byte *= 2;
        int flag;
        if(buffer[i] == '0') flag = 0;
        else if(buffer[i] == '1') flag = 1;
        else{
            std::cerr << "failed to create a byte..." << std::endl;
            return byte;
        }
        byte += flag;
    }
    return byte;
}

char get_bit(char *buffer, int ptr)
{
    unsigned char byte;
    if(ptr >= 0 && ptr <= 7) memcpy(&byte, buffer, 1);
    else if(ptr >=8 && ptr <= 15) memcpy(&byte, buffer+1, 1);
    else {std::cerr << "ptr out of range!" << std::endl; return EOF;}
    unsigned char mask = 0x80;
    for(int i = 0; i < (ptr%8); i++) mask /= 2;
    if((byte & mask) == mask) return '1';
    else if((byte & mask) == 0x00) return '0';
    else {std::cerr << "get_bit error!" << std::endl; return '\0';}
}
