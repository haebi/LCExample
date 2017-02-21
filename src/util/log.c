#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "util/log.h"
#include "util/time.h"

pthread_mutex_t mutexLog = PTHREAD_MUTEX_INITIALIZER;

static void data_append_cpy(char* dest, int* dest_pos, const char* src, const int length);
static void data_append_set(char* dest, int* dest_pos, const char char_fill, const int length);

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

// write log hex
void write_log_hex(const char* file, const char* data, const int data_length)
{
	int fd, n, length, buf_write_pos, i, j, line_count;
	char buf[65535];
	char buf_write[131070];
	char buf_txt[1024];
	char timeStamp[32];
	char char_check;

	// limit max size 65535
	length = data_length > 65535 ? 65535 : data_length;

	// init
	memset(buf	, 0x00, sizeof(buf));
	memset(buf_write, 0x00, sizeof(buf_write));
	memset(buf_txt	, 0x00, sizeof(buf_txt));
	buf_write_pos	= 0;
	char_check	= 0x00;
	line_count	= 0;

	// copy
	memcpy(buf, data, length);
	
	// print
	// print:buf_write size of data
	sprintf(buf_txt, "%s [HEX DUMP : %d Bytes]\n", get_current_timestamp(timeStamp, sizeof(timeStamp)), data_length);
	data_append_cpy(buf_write, &buf_write_pos, buf_txt, strlen(buf_txt));

	// loop i - each char byte
	for(i = 0; i < length; i++)
	{
		// each 16 bytes -> new line
		if(i % 16 == 0)
		{
			// write new line
			if(i != 0)
			{
				data_append_set(buf_write, &buf_write_pos, '\n', 1);

				line_count++;
			}

			// write address
			sprintf(buf_txt, "%s %04X:%04X ", get_current_timestamp(timeStamp, sizeof(timeStamp)), i, i+15);
			data_append_cpy(buf_write, &buf_write_pos, buf_txt, strlen(buf_txt));
		}

		// write HEX
		sprintf(buf_txt, " %02X", 0xff & buf[i]);
		data_append_cpy(buf_write, &buf_write_pos, buf_txt, strlen(buf_txt));

		// write char
		if(i % 16 == 15)
		{
			// seprate blank
			data_append_set(buf_write, &buf_write_pos, 0x20, 2);

			// write char
			for(j = 0; j < 16; j++)
			{
				char_check = buf[line_count * 16 + j];
				data_append_set(buf_write, &buf_write_pos, char_check == 0x00 ? 0x20 : char_check, 1);
			}
		}
	}

	// complete last line
	// [CLL STT]--------------------------------------------------
	// write empty HEX space (not full)
	for(i = 0; i < length % 16; i++)
	{
		data_append_set(buf_write, &buf_write_pos, 0x20, 3);
	}

	// seprate blank
	data_append_set(buf_write, &buf_write_pos, 0x20, 2);

	// write char (not full)
	for(i = 0; i < length % 16; i++)
	{
		char_check = buf[line_count * 16 + i];
		data_append_set(buf_write, &buf_write_pos, char_check == 0x00 ? 0x20 : char_check, 1);
	}
	// [CLL END]--------------------------------------------------

	// end of new line
	data_append_set(buf_write, &buf_write_pos, '\n', 1);
	
	// lock (support for multi-thread)
	pthread_mutex_lock(&mutexLog);

	// open(or create, if not exists) file + append mode
	fd = open(file, O_WRONLY|O_APPEND|O_CREAT,0644);
	
	// write log
	n = write(fd, buf_write, buf_write_pos);

	// close file
	close(fd);

	// unlock
	pthread_mutex_unlock(&mutexLog);

	return;
}

/*
 * dest		: destination address
 * dest_pos	: position of append start
 * src		: source address
 * length	: append data length
 * */
static void data_append_cpy(char* dest, int* dest_pos, const char* src, const int length)
{
	// start append data from dest_pos
	memcpy(dest + (*dest_pos), src, length);

	// calculate nbuf_write append position
	*dest_pos += length;

	return;
}

static void data_append_set(char* dest, int* dest_pos, const char char_fill, const int length)
{
	// start append fill from dest_pos
	memset(dest + (*dest_pos), char_fill, length);

	// calculate nbuf_write append position
	*dest_pos += length;

	return;
}
