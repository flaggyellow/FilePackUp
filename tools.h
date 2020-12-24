#ifndef TOOLS_H
#define TOOLS_H

#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <cstring>
#include <vector>
#include <cmath>
#include <iostream>

static char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int checkValidLocation(QString filepath, int mode);
bool isFileExist(QString fullFilePath);
bool isFile(QString filepath);
bool isDir(QString filepath);
int cpQs2Ca(char *to, QString qs);
int fillOctCa(char *to, long long num);
bool isEmptyBlock(char *datablock, int size);

#endif // TOOLS_H
