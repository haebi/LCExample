BIT := $(shell getconf LONG_BIT)

CFLAGS = -g -O2 -m$(BIT) -I./include

BIN = gettime writelog

all: gettime writelog

gettime: dummy gettime.o lib_time
	gcc -o gettime gettime.o time.o
	strip gettime

writelog: dummy writelog.o lib_log lib_time
	gcc -o writelog writelog.o log.o time.o
	strip writelog

writelog.o: dummy
	gcc -c src/example/writelog.c $(CFLAGS)

gettime.o: dummy
	gcc -c src/example/gettime.c $(CFLAGS)

lib_time:
	gcc -c src/util/time.c $(CFLAGS)

lib_log:
	gcc -c src/util/log.c $(CFLAGS)

clean:
	rm -rf *.o *.txt $(BIN)

dummy:
