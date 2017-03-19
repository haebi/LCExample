#ifndef __UTIL_INI_H__

#define __UTIL_INI_H__

char* read_ini(char* dest, const char* file, const char* section, const char* key);
int read_ini_int(const char* file, const char* section, const char* key);

#endif
