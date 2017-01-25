#include <stdio.h>
#include <string.h>

#include "util/time.h"

int main (void)
{
	char buff[26 +1];

	printf("rev1.1\n");
	
	printf("time num %s\n", get_current_time(buff, sizeof(buff)));

	printf("time str %s\n", time_to_timestamp(buff, sizeof(buff)));

	return 0;
}

