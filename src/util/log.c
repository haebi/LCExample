#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "util/log.h"
#include "util/time.h"

pthread_mutex_t mutexLog = PTHREAD_MUTEX_INITIALIZER;

// write log
// [REF] http://stackoverflow.com/questions/7136416/opening-file-in-append-mode-using-open-api
void writeLog(const char* file, const char* msg)
{
	int fd, n;
	char buf[4096];
	char timeStamp[32];

	memset(buf, 0x00, sizeof(buf));

	sprintf(buf, "%s %s", get_current_timestamp(timeStamp, sizeof(timeStamp)), msg);
	
	// lock (support for multi-thread)
	pthread_mutex_lock(&mutexLog);

	// open(or create, if not exists) file + append mode
	fd = open(file, O_WRONLY|O_APPEND|O_CREAT,0644);
	
	// write log
	n = write(fd, buf, strlen(buf));

	// close file
	close(fd);

	// unlock
	pthread_mutex_unlock(&mutexLog);

	return;
}
