#include <iostream>
#include <fstream>
#include <cstring>

#include "utility.h"

using namespace std;

void message()
{
    cout << "./lab2 -e [file] [key1] [key2] [key3]\n";
    cout << "./lab2 -d [file] [key1] [key2] [key3]\n";
}

char *readFile(const char *fileName, int &infoSize, bool encode)
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
        fileSize = file.tellg(); // записываем размер файла
        if (encode)
        {
            int extraBytes = 8 - (fileSize % 8); // узнаем кол-во доп. байтов
            infoSize = fileSize + extraBytes;    // записываем размер информации
            info = new char[infoSize]();         // создаем пустой  массив данных
            info[fileSize] = 1;
        }
        else
        {
            infoSize = fileSize;
            info = new char[infoSize]();
        }

        file.seekg(0, ios::beg);
        file.read(info, fileSize);
        file.close();
        return info;
    }
}

void writeFile(const char *fileName, char *info, int infoSize, bool decode)
{
    char outFileName[260] = "out_"; // формируем имя выходного файла
    strcat(outFileName, fileName);

    if (decode) // избавляемся от лишний байт
    {
        for (int i = infoSize - 1; i >= 0; i--)
        {
            if (info[i] == 1)
            {
                infoSize = i;
                break;
            }
        }
    }

    ofstream file(outFileName, ios::binary);
    file.write(info, infoSize);
    file.close();
}

char **keysFormat(char *key1, char *key2, char *key3)
{
    char **rtrn = new char *[3]; // массив ключей
    for (int i = 0; i < 3; i++)
    {
        rtrn[i] = new char[7];
    }

    int i;
    for (i = 0; i < 7 && key1[i] != '\0'; i++) // 1-й ключ
    {
        rtrn[0][i] = key1[i];
    }
    for (; i < 7; i++)
    {
        rtrn[0][i] = 'A';
    }

    for (i = 0; i < 7 && key2[i] != '\0'; i++) // 2-й ключ
    {
        rtrn[1][i] = key2[i];
    }
    for (; i < 7; i++)
    {
        rtrn[1][i] = 'A';
    }

    for (i = 0; i < 7 && key3[i] != '\0'; i++) // 3-й ключ
    {
        rtrn[2][i] = key3[i];
    }
    for (; i < 7; i++)
    {
        rtrn[2][i] = 'A';
    }
    return rtrn;
}

int getBit(char *info, int num)
{
    --num;                    // переходим к счету с 0
    int numChar = num / 8;    // номер символа
    int rtrn = info[numChar]; // нужный символ
    // ФОРМИРУЕМ СДВИГ
    int move = 7 - (num - (numChar * 8));
    // ПРИМЕНЯЕМ СДВИГ И МАСКУ 0000 0001
    rtrn = (rtrn >> move) & 1;
    return rtrn;
}

void setBit(char *info, int num, int value)
{
    // ФОРМИРУЕМ ВОЗВРАЩАЕМЫЙ МАССИВ
    char rtrn[8];
    for (int i = 0; i < 8; i++)
    {
        rtrn[i] = info[i];
    }

    --num;                 // переходим к счету с 0
    int numChar = num / 8; // номер символа
    // ФОРМИРУЕМ МАСКУ
    int mask = 1 << (7 - (num - (numChar * 8)));
    if (value == 1)
    {
        // ПРИМЕНЯЕМ МАСКУ
        info[numChar] = rtrn[numChar] | mask;
    }
    else if (value == 0)
    {
        // ИНВЕРТИРУЕМ МАСКУ И ПРИМЕНЯЕМ
        mask = ~mask;
        info[numChar] = rtrn[numChar] & mask;
    }
}