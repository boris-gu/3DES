#include <iostream>
#include "des.h"

using namespace std;

void DESencode(char *info, int infoSize, char *key)
{
    cout << "key: ";
    for (int i = 0; i < 8; i++)
    {
        cout << key[i];
    }
    cout << "\n";

    // ГЕНЕРАЦИЯ КЛЮЧЕЙ ПО 48 БИТ
    char **keys = new char *[16];
    for (int i = 0; i < 16; i++)
    {
        keys[i] = new char[6];
    }
    keyGeneration(key, keys);

    // РАЗДЕЛЕНИЕ НА БЛОКИ ПО 64 БИТ
    int infoBlocksSize = infoSize / 8;
    char **infoBlocks = new char *[infoBlocksSize];
    for (int i = 0; i < infoBlocksSize; i++)
    {
        infoBlocks[i] = new char[8];
        for (int j = 0; j < 8; j++)
        {
            infoBlocks[i][j] = info[(i * 8) + j];
        }
    }

    // РЕАЛИЗАЦИЯ DES
    char utilRi[4];
    for (int i = 0; i < infoBlocksSize; i++)
    {
        IP(infoBlocks[i]);           // начальная перестановка
        for (int j = 0; j < 16; j++) // 16 проходов функции Фейстеля и XOR
        {
            for (int k = 0; k < 4; k++) // запоминаем R в вспомогательный массив
            {
                utilRi[k] = infoBlocks[i][4 + k];
            }
            f(utilRi, keys[j]);         // функция Фейстеля
            for (int k = 0; k < 4; k++) // L XOR f
            {
                utilRi[k] = utilRi[k] ^ infoBlocks[i][k];
            }
            for (int k = 0; k < 4; k++) // R ставим первым
            {
                infoBlocks[i][k] = infoBlocks[i][k + 4];
            }
            for (int k = 0; k < 4; k++) // L XOR f ставим вторым
            {
                infoBlocks[i][k + 4] = utilRi[k];
            }
        }
        IPinvert(infoBlocks[i]); // конечная перестановка
    }

    // ЗАПИСЬ
    for (int i = 0; i < infoBlocksSize; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            info[(i * 8) + j] = infoBlocks[i][j];
        }
    }

    // ОСВОБОЖДЕНИЕ ПАМЯТИ
    for (int i = 0; i < 16; i++)
    {
        delete[] keys[i];
    }
    delete[] keys;
    for (int i = 0; i < infoBlocksSize; i++)
    {
        delete[] infoBlocks[i];
    }
    delete[] infoBlocks;

    cout << "encryption completed\n";
}

void DESdecode(char *info, int infoSize, char *key)
{
    cout << "key: ";
    for (int i = 0; i < 8; i++)
    {
        cout << key[i];
    }
    cout << "\n";

    // ГЕНЕРАЦИЯ КЛЮЧЕЙ ПО 48 БИТ
    char **keys = new char *[16];
    for (int i = 0; i < 16; i++)
    {
        keys[i] = new char[6];
    }
    keyGeneration(key, keys);

    // РАЗДЕЛЕНИЕ НА БЛОКИ ПО 64 БИТ
    int infoBlocksSize = infoSize / 8;
    char **infoBlocks = new char *[infoBlocksSize];
    for (int i = 0; i < infoBlocksSize; i++)
    {
        infoBlocks[i] = new char[8];
        for (int j = 0; j < 8; j++)
        {
            infoBlocks[i][j] = info[(i * 8) + j];
        }
    }

    // ФОРМИРОВАНИЕ Li+1
    char utilRi[4];
    for (int i = 0; i < infoBlocksSize; i++)
    {
        IP(infoBlocks[i]);           // начальная перестановка
        for (int j = 0; j < 16; j++) // 16 проходов функции Фейстеля и XOR
        {
            for (int k = 0; k < 4; k++) // запоминаем L в вспомогательный массив
            {
                utilRi[k] = infoBlocks[i][k];
            }
            f(utilRi, keys[15 - j]);    // функция Фейстеля
            for (int k = 0; k < 4; k++) // R XOR f
            {
                utilRi[k] = utilRi[k] ^ infoBlocks[i][4 + k];
            }
            for (int k = 0; k < 4; k++) // L ставим вторым
            {
                infoBlocks[i][4 + k] = infoBlocks[i][k];
            }
            for (int k = 0; k < 4; k++) // L XOR f ставим вторым
            {
                infoBlocks[i][k] = utilRi[k];
            }
        }
        IPinvert(infoBlocks[i]); // конечная перестановка
    }

    for (int i = 0; i < infoBlocksSize; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            info[(i * 8) + j] = infoBlocks[i][j];
        }
    }

    // ОСВОБОЖДЕНИЕ ПАМЯТИ
    for (int i = 0; i < 16; i++)
    {
        delete[] keys[i];
    }
    delete[] keys;
    for (int i = 0; i < infoBlocksSize; i++)
    {
        delete[] infoBlocks[i];
    }
    delete[] infoBlocks;

    cout << "decryption complete\n";
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

void f(char *info, char *key)
{
    // РАСШИРЕНИЕ R ДО 48 БИТ
    const int table2[48] = {32, 1, 2, 3, 4, 5,
                            4, 5, 6, 7, 8, 9,
                            8, 9, 10, 11, 12, 13,
                            12, 13, 14, 15, 16, 17,
                            16, 17, 18, 19, 20, 21,
                            20, 21, 22, 23, 24, 25,
                            24, 25, 26, 27, 28, 29,
                            28, 29, 30, 31, 32, 1};
    char extendedR[6];
    for (int i = 1; i <= 48; i++)
    {
        setBit(extendedR, i, getBit(info, table2[i - 1]));
    }

    // СЛОЖЕНИЕ ПО МОДУЛЮ 2 С КЛЮЧОМ
    for (int i = 0; i < 6; i++)
    {
        extendedR[i] = extendedR[i] ^ key[i];
    }

    // ПРЕОБРАЗОВАНИЯ S, 48 БИТ -> 32 БИТ
    const int table3[8][4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                                   0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                                   4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                                   15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
                                  {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                                   3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                                   0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                                   13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
                                  {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                                   13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                                   13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                                   1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
                                  {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                                   13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                                   10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                                   3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
                                  {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                                   14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                                   4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                                   11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
                                  {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                                   10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                                   9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                                   4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
                                  {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                                   13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                                   1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                                   6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
                                  {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                                   1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                                   7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                                   2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};
    char narrowedR[4];
    int utilI = 0; // таблица, 0-3
    int utilJ = 0; // таблица, 0-15
    int util4Bit;  // преобразование до 4 бит

    for (int i = 0; i < 8; i++)
    {
        utilI = getBit(extendedR, (i * 6) + 1);
        utilI = utilI << 1;
        utilI += getBit(extendedR, (i * 6) + 6);

        utilJ = getBit(extendedR, (i * 6) + 2);
        utilJ = utilJ << 1;
        utilJ += getBit(extendedR, (i * 6) + 3);
        utilJ = utilJ << 1;
        utilJ += getBit(extendedR, (i * 6) + 4);
        utilJ = utilJ << 1;
        utilJ += getBit(extendedR, (i * 6) + 5);

        util4Bit = table3[i][utilI][utilJ]; // запоминаем число из таблицы
        setBit(narrowedR, (i * 4) + 1, getBit((char *)&util4Bit, 5));
        setBit(narrowedR, (i * 4) + 2, getBit((char *)&util4Bit, 6));
        setBit(narrowedR, (i * 4) + 3, getBit((char *)&util4Bit, 7));
        setBit(narrowedR, (i * 4) + 4, getBit((char *)&util4Bit, 8));
    }

    // ПЕРЕСТАНОВКА P
    const int table4[32] = {16, 7, 20, 21, 29, 12, 28, 17,
                            1, 15, 23, 26, 5, 18, 31, 10,
                            2, 8, 24, 14, 32, 27, 3, 9,
                            19, 13, 30, 6, 22, 11, 4, 25};
    for (int i = 1; i <= 32; i++)
    {
        setBit(info, i, getBit(narrowedR, table4[i - 1]));
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
