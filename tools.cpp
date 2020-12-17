#include "tools.h"


int checkValidLocation(QString filepath)
{
    QStringList dir = filepath.split(';');
    if (dir.length() <= 0) return -1;
    for(int i = 0; i < dir.length(); i++) if(!isFileExist(dir[i])) return -2;
    return 0;
}

bool isFileExist(QString fullFilePath)
{
    QFile file(fullFilePath);
    if(file.exists()) return true;
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
