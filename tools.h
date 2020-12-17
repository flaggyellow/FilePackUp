#ifndef TOOLS_H
#define TOOLS_H

#include <QStringList>
#include <QFile>
#include <cstring>
#include <vector>
#include <cmath>

static char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int checkValidLocation(QString filepath);
bool isFileExist(QString fullFilePath);
int cpQs2Ca(char *to, QString qs);
int fillOctCa(char *to, long long num);

#endif // TOOLS_H
