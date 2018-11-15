#include <iostream>
#include "des.h"

using namespace std;

void DESencode(char *info, int info_size, char *key)
{
    // РАЗДЕЛЕНИЕ НА БЛОКИ ПО 64 БИТ
    int info_blocks_size = info_size / 8;
    char **info_blocks = new char *[info_blocks_size];
    for (int i = 0; i < info_blocks_size; i++)
    {
        info_blocks[i] = new char[8];
        for (int j = 0; j < 8; j++)
        {
            info_blocks[i][j] = info[(i * 8) + j];
        }
    }

    // ВЫВОД
    for (int i = 0; i < info_blocks_size; i++)
    {
        cout << "\"";
        for (int j = 0; j < 8; j++)
        {
            cout << info_blocks[i][j];
        }
        cout << "\"\n";
    }
    cout << "\n";

    for (int i = 0; i < info_blocks_size; i++)
    {
        IP(info_blocks[i]);
    }

    // ВЫВОД
    for (int i = 0; i < info_blocks_size; i++)
    {
        cout << "\"";
        for (int j = 0; j < 8; j++)
        {
            cout << info_blocks[i][j];
        }
        cout << "\"\n";
    }
    cout << "\n";

    for (int i = 0; i < info_blocks_size; i++)
    {
        IPinvert(info_blocks[i]);
    }

    // ВЫВОД
    for (int i = 0; i < info_blocks_size; i++)
    {
        cout << "\"";
        for (int j = 0; j < 8; j++)
        {
            cout << info_blocks[i][j];
        }
        cout << "\"\n";
    }

    // ОСВОБОЖДЕНИЕ ПАМЯТИ
    for (int i = 0; i < info_blocks_size; i++)
    {
        delete[] info_blocks[i];
    }
    delete[] info_blocks;
}

void DESdecode(char *info, int info_size, char *key)
{
}

int getBit(char *info, int num)
{
    //cout << num << "\n";
    --num;                     // переходим к счету с 0
    int num_char = num / 8;    // номер символа
    int rtrn = info[num_char]; // нужный символ
    // ФОРМИРУЕМ СДВИГ
    //int move = (8 - (num % 8)) % 8;
    int move = 7 - (num - (num_char * 8));
    // ПРИМЕНЯЕМ СДВИГ И МАСКУ 0000 0001
    rtrn = (rtrn >> move) & 1;
    //cout << "OK\n";
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

    --num;                  // переходим к счету с 0
    int num_char = num / 8; // номер символа
    // ФОРМИРУЕМ МАСКУ
    int mask = 1 << (7 - (num - (num_char * 8)));
    if (value == 1)
    {
        // ПРИМЕНЯЕМ МАСКУ
        info[num_char] = rtrn[num_char] | mask;
    }
    else if (value == 0)
    {
        // ИНВЕРТИРУЕМ МАСКУ И ПРИМЕНЯЕМ
        mask = ~mask;
        info[num_char] = rtrn[num_char] & mask;
    }
}

void IP(char *info) // РАЗБИТЬ ПО ПАКЕТАМ!!!!!!
{
    const int table1[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
                            62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
                            57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
                            61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
    // ФОРМИРУЕМ ВОЗВРАЩАЕМЫЙ МАССИВ
    char rtrn[8];
    for (int i = 0; i < 8; i++)
    {
        rtrn[i] = info[i];
    }

    //char *util = new char[8];

    for (int i = 1; i <= 64; i++)
    {
        //cout << "YES1\n";
        setBit(info, i, getBit(rtrn, table1[i - 1]));
        //cout << "YES\n";
    }
}

void IPinvert(char *info)
{
    const int table8[64] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
                            38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 60, 29,
                            36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
                            34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
    // ФОРМИРУЕМ ВОЗВРАЩАЕМЫЙ МАССИВ
    char rtrn[8];
    for (int i = 0; i < 8; i++)
    {
        rtrn[i] = info[i];
    }

    for (int i = 1; i <= 64; i++)
    {
        setBit(info, i, getBit(rtrn, table8[i - 1]));
    }
}
