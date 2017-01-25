#include <string.h>
#include <stdio.h>
#include <time.h>

#include <sys/time.h>

// get localtime
// [REF] https://www.joinc.co.kr/w/man/3/localtime
// [REF] https://www.ibm.com/support/knowledgecenter/ko/ssw_ibm_i_73/rtref/localtr.htm
char* get_current_timestamp(char* dest, int length)
{
	char buf[32];

	memset(buf, 0x00, sizeof(buf));

	struct timeval tv; // for micro seconds
	struct tm newtime; // current system timezone
	time_t ltime;

	// get current time
	ltime=time(&ltime);

	// convert current time to time of local timezone.
	localtime_r(&ltime, &newtime);

	// get micro second
	gettimeofday(&tv, NULL);

	// make timestamp
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d.%06ld"
			, newtime.tm_year+1900
			, newtime.tm_mon+1
			, newtime.tm_mday
			, newtime.tm_hour
			, newtime.tm_min
			, newtime.tm_sec
			, tv.tv_usec);

	memcpy(dest, buf, length);
	memset(dest + length -1, 0x00, 1);

	return dest;
}

char* get_current_time(char* dest, int length)
{
	char buf[32];

	memset(buf, 0x00, sizeof(buf));

	struct timeval tv; // for micro seconds
	struct tm newtime; // current system timezone
	time_t ltime;

	// get current time
	ltime=time(&ltime);

	// convert current time to time of local timezone.
	localtime_r(&ltime, &newtime);

	// get micro second
	gettimeofday(&tv, NULL);

	// make timestamp
	sprintf(buf, "%04d%02d%02d%02d%02d%02d%06ld"
			, newtime.tm_year+1900
			, newtime.tm_mon+1
			, newtime.tm_mday
			, newtime.tm_hour
			, newtime.tm_min
			, newtime.tm_sec
			, tv.tv_usec);

	memcpy(dest, buf, length);
	memset(dest + length -1, 0x00, 1);

	return dest;
}

char* time_to_timestamp(char* dest, int length)
{
	char buf[32];

	memset(buf, 0x00, sizeof(buf));

	// make timestamp from time number
	// 0         1         2
	// 01234567890123456789012345
	// 2017-01-01 00:00:00.000000
	memcpy(buf  +0, dest  +0, 4);
	buf[4] = '-';
	memcpy(buf  +5, dest  +4, 2);
	buf[7] = '-';
	memcpy(buf  +8, dest  +6, 2);
	buf[10] = ' ';
	memcpy(buf +11, dest  +8, 2);
	buf[13] = ':';
	memcpy(buf +14, dest +10, 2);
	buf[16] = ':';
	memcpy(buf +17, dest +12, 2);
	buf[19] = '.';
	memcpy(buf +20, dest +14, 6);

	// export converted data
	memcpy(dest, buf, length);
	memset(dest + length -1, 0x00, 1);

	return dest;
}
