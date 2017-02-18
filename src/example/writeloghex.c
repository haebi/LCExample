#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "util/log.h"

// sample struct
struct box
{
	int a[2];
	long b[2];
	char c[32];
	char d[64];
	unsigned long long e[2];
	char f[16];
};

// main
int main(int argc, char** argv)
{
	// vars
	struct box data;
	char file[255] = "writelog.txt";
	int i, loop;

	// init
	memset(&data, 0x00, sizeof(struct box));

	// setting data
	data.a[0] = 123;
	data.b[0] = 456;
	sprintf(data.c, "Hello World");
	sprintf(data.d, "Sample Text Message Data to HEX Codes...");
	data.e[0] = 1234567890;
	sprintf(data.f, "last data...ZXC");

	if(argc > 1)
	{
		loop = atoi(argv[1]);

		while(loop)
		{
			// write HEX log N times
			write_log_hex(file, (char *)&data, sizeof(struct box));
			loop--;
		}
	}
	else
	{
		// write HEX log 1 time
		write_log_hex(file, (char *)&data, sizeof(struct box));
	}

	return 0;
}
