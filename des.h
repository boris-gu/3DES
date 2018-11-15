#ifndef des_h
#define des_h

void DESencode(char *info, int info_size, char *key);

void DESdecode(char *info, int info_size, char *key);

int getBit(char *info, int num);

void setBit(char *info, int num, int value);

void keyGeneration(char *initialKey, char **keys);

void IP(char *info);

void IPinvert(char *info);

#endif