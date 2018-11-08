#include <iostream>
#include <fstream>
#include <cstring>

#include "utility.h"

using namespace std;

void message()
{
    cout << "./lab2 -e [file] [key1] [key2] [key3]\n";
    cout << "./lab2 -d [file] [extra bytes] [key1] [key2] [key3]\n";
}

char *readFile(const char *file_name, int &info_size, int &extra_bytes)
{
    ifstream file(file_name, ios::binary);
    if (!file)
    {
        cout << "file " << file_name << " not found\n";
    }
    else
    {
        char *info;
        int file_size;
        file.seekg(0, ios::end);
        file_size = file.tellg();                // записываем размер файла
        extra_bytes = (8 - (file_size % 8)) % 8; // узнаем кол-во доп. байтов
        info_size = file_size + extra_bytes;     // записываем размер информации
        cout << info_size << "\n";
        info = new char[info_size](); // создаем пустой  массив данных

        file.seekg(0, ios::beg);
        file.read(info, file_size);
        cout << info << "\n";
        file.close();
        return info;
    }
}

void writeFile(const char *file_name, char *info, int info_size)
{
    char out_file_name[260] = "out_"; // формируем имя выходного файла
    strcat(out_file_name, file_name);

    ofstream file(out_file_name, ios::binary);
    file.write(info, info_size);
    file.close();
}