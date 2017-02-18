#ifndef __UTIL_LOG_H__

#define __UTIL_LOG_H__

#include <pthread.h>

void write_log(const char* file, const char* msg);
void write_log_hex(const char* file, const char* data, const int data_length);

#endif
