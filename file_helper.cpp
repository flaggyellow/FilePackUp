#include "file_helper.h"

void FileHelper::readFile(){}
void FileHelper::writeFile(){}
void FileHelper::getFrequency(){}

QFile &FileHelper::get_file(){ return file;}
int FileHelper::get_fileType(){ return fileType;}

FileHelper::FileHelper(QString filePath, int type)
    : file(filePath)
    , fileType(type)
{}

FileHelper::FileHelper(int type)
    :file()
    ,fileType(type)
{}

FileHelper::~FileHelper(){}
