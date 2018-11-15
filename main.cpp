#include <iostream>
#include <cstring>

#include "utility.h"
#include "des.h"

using namespace std;

int main(int argc, char *argv[])
{
    char *info;
    int info_size = 0;
    int extra_bytes = 0;

    if (argc == 1)
    {
        message();
    }
    else if (!strcmp(argv[1], "-e"))
    {
        if (argc == 6)
        {
            info = readFile(argv[2], info_size, extra_bytes);
            /* for (int i = 0; i < info_size; i++)
            {
                cout << (int)info[i] << "\n";
            } */
            writeFile(argv[2], info, info_size);
            DESencode(info, info_size, argv[3]);
        }
        else
        {
            message();
        }
    }
    else if (!strcmp(argv[1], "-d"))
    {
        if (argc == 7)
        {
            info = readFile(argv[2], info_size, extra_bytes);
            writeFile(argv[2], info, info_size);
        }
        else
        {
            message();
        }
    }
    else
    {
        cout << argv[1];
        message();
    }
    return 0;
}