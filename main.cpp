#include <iostream>
#include <cstring>

#include "utility.h"
#include "tripledes.h"

using namespace std;

int main(int argc, char *argv[])
{
    char *info;
    int infoSize = 0;

    if (argc == 1)
    {
        message();
    }
    else if (!strcmp(argv[1], "-e"))
    {
        if (argc == 6)
        {
            info = readFile(argv[2], infoSize, true);
            if (info == NULL)
            {
                return 0;
            }
            char **keys = keysFormat(argv[3], argv[4], argv[5]);
            tripleDESencode(info, infoSize, keys);
            writeFile(argv[2], info, infoSize, false);
        }
        else
        {
            message();
        }
    }
    else if (!strcmp(argv[1], "-d"))
    {
        if (argc == 6)
        {
            info = readFile(argv[2], infoSize, false);
            if (info == NULL)
            {
                return 0;
            }
            char **keys = keysFormat(argv[3], argv[4], argv[5]);
            tripleDESdecode(info, infoSize, keys);
            writeFile(argv[2], info, infoSize, true);
        }
        else
        {
            message();
        }
    }
    else
    {
        message();
    }
    return 0;
}