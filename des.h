#ifndef des_h
#define des_h

void DESencode(char *info, int info_size, char *key);

void DESdecode(char *info, int info_size, char *key);

int getBit(char *info, int num);

char *setBit(char *info, int info_size, int num, int value);

void IP(char *info, int info_size);

void IPinvert(char *info, int info_size);

#endif