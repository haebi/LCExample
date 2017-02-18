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
void write_log(const char* file, const char* msg)
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

void write_log_hex(const char* file, const char* data, const int data_length)
{
	int fd, n, length, ext_pos;
	char buf[65535];
	char ext[131070];
	char buf_txt[1024];
	char timeStamp[32];
	char char_check;

	// check if data_length size over 65535
	length = data_length > 65535 ? 65535 : data_length;

	// init & copy
	memset(buf, 0x00, sizeof(buf));
	memset(ext, 0x00, sizeof(ext));
	memset(buf_txt, 0x00, sizeof(buf_txt));
	memcpy(buf, data, length);
	ext_pos = 0;
	char_check = 0x00;
	
	// print
	// print:ext size of data
	sprintf(buf_txt, "%s [HEX DUMP : %d Bytes]\n", get_current_timestamp(timeStamp, sizeof(timeStamp)), data_length);
	memcpy(ext, buf_txt, strlen(buf_txt));
	ext_pos += strlen(buf_txt);

	int i, j, line_count;

	line_count = 0;
	for(i = 0; i < length; i++)
	{
		if(i % 16 == 0)
		{
			// write new line
			if(i != 0)
			{
				sprintf(buf_txt, "\n");
				memcpy(&ext[ext_pos], buf_txt, strlen(buf_txt));
				ext_pos += 1;
				line_count++;
			}

			// write address
			sprintf(buf_txt, "%s %04X:%04X ", get_current_timestamp(timeStamp, sizeof(timeStamp)), i, i+15);
			memcpy(&ext[ext_pos], buf_txt, strlen(buf_txt));
			ext_pos += strlen(buf_txt);
		}

		// write HEX
		sprintf(buf_txt, " %02X", 0xff & buf[i]);
		memcpy(&ext[ext_pos], buf_txt, strlen(buf_txt));
		ext_pos += strlen(buf_txt);

		// write char
		if(i % 16 == 15)
		{
			// seprate blank
			memset(&ext[ext_pos], 0x20, 2);
			ext_pos += 2;

			// write char
			for(j = 0; j < 16; j++)
			{
				char_check = buf[(i / 16) * 16 + j];
				memset(&ext[ext_pos], char_check == 0x00 ? 0x20 : char_check, 1);
				ext_pos++;
			}
		}
	}

	// write empty HEX space (not full)
	for(i = 0; i < length % 16; i++)
	{
		memset(&ext[ext_pos], 0x20, 3);
		ext_pos += 3;
	}

	// seprate blank
	memset(&ext[ext_pos], 0x20, 2);
	ext_pos += 2;

	// write char (not full)
	for(i = 0; i < length % 16; i++)
	{
		char_check = buf[line_count * 16 + i];
		memset(&ext[ext_pos], char_check == 0x00 ? 0x20 : char_check, 1);
		ext_pos++;
	}

	// end of new line
	sprintf(buf_txt, "\n");
	memcpy(&ext[ext_pos], buf_txt, strlen(buf_txt));
	ext_pos += 1;
	
	// lock (support for multi-thread)
	pthread_mutex_lock(&mutexLog);

	// open(or create, if not exists) file + append mode
	fd = open(file, O_WRONLY|O_APPEND|O_CREAT,0644);
	
	// write log
	n = write(fd, ext, ext_pos);

	// close file
	close(fd);

	// unlock
	pthread_mutex_unlock(&mutexLog);

	return;
}
