#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "util/log.h"

int main(int argc, char** argv)
{
	char file[255] = "writelog.txt";
	char msg[128] = "write func test!!\n";

	writeLog(file, msg);

	return 0;
}
