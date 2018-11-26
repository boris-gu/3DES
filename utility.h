#ifndef utility_h
#define utility_h

void message();

char *readFile(const char *fileName, int &infoSize, bool encode);

void writeFile(const char *fileName, char *info, int infoSize, bool decode);

char **keysFormat(char *key1, char *key2, char *key3);

#endif