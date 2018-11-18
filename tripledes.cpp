#include <iostream>
#include "tripledes.h"
#include "des.h"

using namespace std;

void tripleDESencode(char *info, int infoSize, char **keys)
{
    cout << "STEP 1\n";
    DESencode(info, infoSize, keys[0]);
    cout << "-------\n";
    cout << "STEP 2\n";
    DESencode(info, infoSize, keys[1]);
    cout << "-------\n";
    cout << "STEP 3\n";
    DESencode(info, infoSize, keys[2]);
}

void tripleDESdecode(char *info, int infoSize, char **keys)
{
    cout << "STEP 1\n";
    DESdecode(info, infoSize, keys[0]);
    cout << "-------\n";
    cout << "STEP 2\n";
    DESdecode(info, infoSize, keys[1]);
    cout << "-------\n";
    cout << "STEP 3\n";
    DESdecode(info, infoSize, keys[2]);
}