#include <iostream>
#include <cstring>

#include "utility.h"
#include "tripledes.h"

using namespace std;

int main(int argc, char *argv[])
{
    char *info;
    int infoSize = 0;
    int extraBytes = 0;

    if (argc == 1)
    {
        message();
    }
    else if (!strcmp(argv[1], "-e"))
    {
        if (argc == 6)
        {
            info = readFile(argv[2], infoSize, extraBytes);
            if (info == NULL)
            {
                return 0;
            }
            char **keys = keysFormat(argv[3], argv[4], argv[5]);
            tripleDESencode(info, infoSize, keys);
            cout << "extra bytes: " << extraBytes << "\n";
            writeFile(argv[2], info, infoSize);
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
            info = readFile(argv[2], infoSize, extraBytes);
            if (info == NULL)
            {
                return 0;
            }
            char **keys = keysFormat(argv[3], argv[4], argv[5]);
            tripleDESdecode(info, infoSize, keys);
            extraBytes = atoi(argv[6]);
            writeFile(argv[2], info, infoSize - extraBytes);
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