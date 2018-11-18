#include <iostream>
#include <fstream>
#include <cstring>

#include "utility.h"

using namespace std;

void message()
{
    cout << "./lab2 -e [file] [key1] [key2] [key3]\n";
    cout << "./lab2 -d [file] [key1] [key2] [key3] [extra bytes]\n";
}

char *readFile(const char *fileName, int &infoSize, int &extraBytes)
{
    ifstream file(fileName, ios::binary);
    if (!file)
    {
        cout << "file " << fileName << " not found\n";
        return NULL;
    }
    else
    {
        char *info;
        int fileSize;
        file.seekg(0, ios::end);
        fileSize = file.tellg();               // записываем размер файла
        extraBytes = (8 - (fileSize % 8)) % 8; // узнаем кол-во доп. байтов
        infoSize = fileSize + extraBytes;      // записываем размер информации
        info = new char[infoSize]();           // создаем пустой  массив данных

        file.seekg(0, ios::beg);
        file.read(info, fileSize);
        file.close();
        return info;
    }
}

void writeFile(const char *fileName, char *info, int infoSize)
{
    char outFileName[260] = "out_"; // формируем имя выходного файла
    strcat(outFileName, fileName);

    ofstream file(outFileName, ios::binary);
    file.write(info, infoSize);
    file.close();
}

char **keysFormat(char *key1, char *key2, char *key3)
{
    char **rtrn = new char *[3]; // массив ключей
    for (int i = 0; i < 3; i++)
    {
        rtrn[i] = new char[8];
    }

    int i;
    for (i = 0; i < 8 && key1[i] != '\0'; i++) // 1-й ключ
    {
        rtrn[0][i] = key1[i];
    }
    for (; i < 8; i++)
    {
        rtrn[0][i] = 'A';
    }

    for (i = 0; i < 8 && key2[i] != '\0'; i++) // 2-й ключ
    {
        rtrn[1][i] = key2[i];
    }
    for (; i < 8; i++)
    {
        rtrn[1][i] = 'B';
    }

    for (i = 0; i < 8 && key3[i] != '\0'; i++) // 3-й ключ
    {
        rtrn[2][i] = key3[i];
    }
    for (; i < 8; i++)
    {
        rtrn[2][i] = 'C';
    }
    return rtrn;
}