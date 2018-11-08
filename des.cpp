#include "des.h"

void DESencode(char *info, int info_size, char *key)
{
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
    //int move = (8 - (num % 8)) % 8;
    int move = 7 - (num - (num_char * 8));
    // ПРИМЕНЯЕМ СДВИГ И МАСКУ 0000 0001
    rtrn = (rtrn >> move) & 1;
    return rtrn;
}

char *setBit(char *info, int info_size, int num, int value)
{
    // ФОРМИРУЕМ ВОЗВРАЩАЕМЫЙ МАССИВ
    char *rtrn = new char[info_size];
    for (int i = 0; i < info_size; i++)
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
        rtrn[num_char] = info[num_char] | mask;
    }
    else if (value == 0)
    {
        // ИНВЕРТИРУЕМ МАСКУ И ПРИМЕНЯЕМ
        mask = ~mask;
        rtrn[num_char] = info[num_char] & mask;
    }
    return rtrn;
}