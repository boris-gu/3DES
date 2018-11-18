#ifndef tripledes_h
#define tripledes_h

void tripleDESencode(char *info, int infoSize, char **keys);

void tripleDESdecode(char *info, int infoSize, char **keys);

#endif