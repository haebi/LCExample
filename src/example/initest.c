#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util/ini.h"

int main(int argc, char** argv)
{
	char msg_help[1024] = {0x00, };

	if(argc < 4)
	{
		sprintf(msg_help,
				"[USAGE]\n"
				"%s [ini file] [section] [key]"
				, argv[0]);

		printf("%s\n", msg_help);

		exit(0);
	}

	char val1[64] = {0x00, };

	printf("%s\n", read_ini(val1, argv[1], argv[2], argv[3]));

	return 0;
}
