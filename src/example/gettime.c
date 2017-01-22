#include <stdio.h>
#include <string.h>

#include "util/time.h"

int main (void)
{
/*
	char buff[32];

	char buff2[32];
	
//	memcpy(buff2, getDateTimeFormat(buff, sizeof(buff)), 10);
	strcpy(buff2, getDateTimeFormat(buff, sizeof(buff)));
//	sprintf(buff2, "%s", getDateTimeFormat(buff, sizeof(buff)));

	printf("%s\n", buff);
	printf("%s\n", buff2);
*/
	
	char buff[32];

	getDateTimeFormat(buff, sizeof(buff));

	printf("%s\n", buff);

	return 0;
}

