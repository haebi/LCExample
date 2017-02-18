#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "util/log.h"

int main(int argc, char** argv)
{
	char file[255] = "writelog.txt";
	char msg[128] = "write func test!!\n";
	int i, loop;

	if(argc > 1)
	{
		loop = atoi(argv[1]);

		while(loop)
		{
			write_log(file, msg);
			loop--;
		}
	}
	
	write_log(file, msg);

	return 0;
}
