#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "util/ini.h"

// define macro function
#define flag_set(FLAG, MASK)   FLAG |= (MASK)				// set
#define flag_clear(FLAG, MASK) FLAG &= ~(MASK)				// clear
#define flag_check(FLAG, MASK) (FLAG & (MASK)) == (MASK) ? 1 : 0	// check

// define flags
#define IS_REMARK        0x01
#define IS_SECTION       0x02
#define IS_KEY           0x04
#define IS_VALUE         0x08
#define IS_LINE_FIRST    0x10
#define IS_FOUND_SECTION 0x20
#define IS_FOUND_KEY     0x40
#define IS_FOUND_VALUE   0x80

// define default value
#define BUF_SIZE 8192

char* read_ini(char* dest, const char* file, const char* section, const char* key)
{
	char buf[BUF_SIZE] = {0x00, };
	char _section[64];
	char _key[64];
	char _value[64];

	int fd, n, i, pos;
	
	int flag;

	flag = 0;

	// open file
	fd = open(file, O_RDONLY);

	// read file
	n = read(fd, buf, BUF_SIZE);

	// read bytes
	for(i = 0; i < BUF_SIZE; i++)
	{
		// process
		// not remark state
		if(!flag_check(flag, IS_REMARK))
		{
			// set state [remark]
			// when encounted character '#'
			if(buf[i] == '#')
			{
				flag_set(flag, IS_REMARK);
				continue;
			}

			// set state [section]
			// '[', newline state
			if(buf[i] == '[' && flag_check(flag, IS_LINE_FIRST))
			{
				flag_clear(flag, IS_FOUND_SECTION);
				flag_set(flag, IS_SECTION);
				pos = 0;
			}

			// set state [key]
			// no [, newline state
			if(buf[i] != '[' && flag_check(flag, IS_LINE_FIRST | IS_FOUND_SECTION))
			{
				flag_set(flag, IS_KEY);
				pos = 0;
			}

			// set state [found section]
			// equals from parameter section and now section(_section)
			if(buf[i] == ']' && flag_check(flag, IS_SECTION))
			{
				flag_clear(flag, IS_SECTION);

				if(memcmp(_section, section, strlen(_section)) == 0)
				{
					flag_set(flag, IS_FOUND_SECTION);
				}
			}

			// set state [found key]
			// when encounted character '=' in [key] state
			if(buf[i] == '=' && flag_check(flag, IS_KEY))
			{
				flag_clear(flag, IS_KEY);
				flag_set(flag, IS_VALUE);
				pos = 0;

				if(memcmp(_key, key, strlen(_key)) == 0)
				{
					flag_set(flag, IS_FOUND_KEY);
				}
			}

			// set state [found value]
			// when encounted character \n , space or tab
			if(buf[i] == '\n' || buf[i] == '\t' || buf[i] == ' ')
			{
				if(flag_check(flag, IS_VALUE))
				{
					if(flag_check(flag, IS_FOUND_SECTION | IS_FOUND_KEY))
					{
						flag_set(flag, IS_FOUND_VALUE);
						break;
					}
				}
			}

			// parse [section]
			if(flag_check(flag, IS_SECTION))
			{
				if(buf[i] != '[')
				{
					memcpy(_section+pos, &buf[i], 1);
					pos++;
				}
			}

			// parse [key]
			if(flag_check(flag, IS_KEY))
			{
				memcpy(_key+pos, &buf[i], 1);
				pos++;
			}

			// parse [value]
			if(flag_check(flag, IS_VALUE))
			{
				if(buf[i] != '=')
				{
					memcpy(_value+pos, &buf[i], 1);
					pos++;
				}
			}

			// clear state [line first]
			flag_clear(flag, IS_LINE_FIRST);
		}

		// set state [line first]
		if(buf[i] == '\n')
		{
			flag_clear(flag, IS_REMARK | IS_SECTION | IS_KEY | IS_VALUE);

			flag_set(flag, IS_LINE_FIRST);

			memset(_section, 0x00, sizeof(_section));
			memset(_key    , 0x00, sizeof(_key)    );
			memset(_value  , 0x00, sizeof(_value)  );

			pos = 0;
		}
	}

	// close file
	close(fd);

	// prepare return value when found
	if(flag_check(flag, IS_FOUND_VALUE))
	{
		memcpy(dest, _value, strlen(_value));
	}

	// return value;
	return dest;
}

int read_ini_int(const char* file, const char* section, const char* key)
{
	return 0;
}
