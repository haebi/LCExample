#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "util/log.h"
#include "util/time.h"

pthread_mutex_t mutexLog = PTHREAD_MUTEX_INITIALIZER;

void writeLog(char* file, char* msg)
{
	FILE *pf;
	char buf[4096];
	char timeStamp[32];

	memset(buf, 0x00, sizeof(buf));

	sprintf(buf, "%s %s", get_current_timestamp(timeStamp), msg);
	
	// lock (support for multi-thread)
	pthread_mutex_lock(&mutexLog);

	// open file + append mode
	pf = fopen(file, "a");
	
	// write log
	fputs(buf, pf);

	// close file
	fclose(pf);

	// unlock
	pthread_mutex_unlock(&mutexLog);

	return;
}
