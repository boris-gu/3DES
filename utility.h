#ifndef utility_h
#define utility_h

void message();

char *readFile(const char *file_name, int &info_size, int &extra_bytes);

void writeFile(const char *file_name, char *info, int info_size);

#endif