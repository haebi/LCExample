#include <string.h>
#include <stdio.h>
#include <time.h>

#include <sys/time.h>

// get localtime
// [REF] https://www.joinc.co.kr/w/man/3/localtime
// [REF] https://www.ibm.com/support/knowledgecenter/ko/ssw_ibm_i_73/rtref/localtr.htm
char* getDateTimeFormat(char* dest, int len)
{
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
	sprintf(dest, "%04d-%02d-%02d %02d:%02d:%02d.%06ld"
			, newtime.tm_year+1900
			, newtime.tm_mon+1
			, newtime.tm_mday
			, newtime.tm_hour
			, newtime.tm_min
			, newtime.tm_sec
			, tv.tv_usec);

	return dest;
}
