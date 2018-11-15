#include <iostream>
#include "des.h"

using namespace std;

void DESencode(char *info, int info_size, char *key)
{
    // ГЕНЕРАЦИЯ КЛЮЧЕЙ ПО 48 БИТ
    char **keys = new char *[16];
    for (int i = 0; i < 16; i++)
    {
        keys[i] = new char[6];
    }
    keyGeneration(key, keys);

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
    --num;                     // переходим к счету с 0
    int num_char = num / 8;    // номер символа
    int rtrn = info[num_char]; // нужный символ
    // ФОРМИРУЕМ СДВИГ
    int move = 7 - (num - (num_char * 8));
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

void keyGeneration(char *initialKey, char **rtrnKeys)
{
    // ФОРМИРОВАНИЕ РАСШИРЕННОГО КЛЮЧА
    char extendedKey[8];                                 // расширенный ключ
    for (int i = 1, j = 1, count = 0; i <= 56; i++, j++) // i - initialKey, j - extendedKey
    {
        setBit(extendedKey, j, getBit(initialKey, i));
        if (getBit(initialKey, i) == 1) // счетчик единиц
        {
            ++count;
        }
        if ((i % 7) == 0) // каждый 8-ой бит
        {
            ++j;
            setBit(extendedKey, j, (count % 2));
            count = 0;
        }
    }

    // ПЕРЕСТАНОВКА И ИЗБАВЛЕНИЕ ОТ ЛИШНИХ БИТ
    const int table5[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
                            10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
                            63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
                            14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
    char utilKey[7];
    for (int i = 1; i <= 56; i++)
    {
        setBit(utilKey, i, getBit(extendedKey, table5[i - 1]));
    }

    for (int i = 1; i <= 56; i++)
    {
        cout << getBit(utilKey, i);
        if (i % 7 == 0)
            cout << "  ";
    }
    cout << "\n";

    // СДВИГИ ДВУХ ПОЛОВИН КЛЮЧА, ФОРМИРОВАНИЕ ПОДКЛЮЧЕЙ
    const int table6[16] = {1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28};
    char utilKeys[16][7]; // вспомогательная, запоминаем сдвиги
    for (int i = 0; i < 16; i++)
    {
        for (int j = 1, getUtility; j <= 28; j++) //28
        {
            getUtility = j + table6[i];
            if (getUtility > 28)
            {
                getUtility -= 28;
            }
            setBit(utilKeys[i], j, getBit(utilKey, getUtility));
            setBit(utilKeys[i], j + 28, getBit(utilKey, getUtility + 28));
        }
    }

    // ВЫБОРКА БИТ, ФОРМИРОВАНИЕ ПОДКЛЮЧЕЙ
    const int table7[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
                            26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
                            51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
    for (int i = 0; i < 16; i++)
    {
        for (int j = 1; j <= 48; j++)
        {
            setBit(rtrnKeys[i], j, getBit(utilKeys[i], table7[j - 1]));
        }
    }

    /* cout << '\n';
    for (int j = 0; j < 16; j++)
    {
        for (int i = 1; i <= 48; i++)
        {
            cout << getBit(rtrnKeys[j], i);
            if (i % 7 == 0)
                cout << "  ";
        }
        cout << "\n";
    } */
}

void IP(char *info)
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

    for (int i = 1; i <= 64; i++)
    {
        setBit(info, i, getBit(rtrn, table1[i - 1]));
    }
}

void IPinvert(char *info)
{
    const int table8[64] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
                            38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
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
