#ifndef des_h
#define des_h

void DESencode(char *info, int infoSize, char *key);

void DESdecode(char *info, int infoSize, char *key);

void keyGeneration(char *initialKey, char **keys);

void IP(char *info);

void f(char *info, char *key);

void IPinvert(char *info);

#endif