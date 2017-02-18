BIT := $(shell getconf LONG_BIT)

CFLAGS = -g -O2 -m$(BIT) -I./include

BIN = gettime writelog writeloghex

all: gettime writelog writeloghex

gettime: dummy gettime.o lib_time
	gcc -o gettime gettime.o time.o
	strip gettime

writelog: dummy writelog.o lib_log lib_time
	gcc -o writelog writelog.o log.o time.o
	strip writelog

writeloghex: dummy writeloghex.o lib_log lib_time
	gcc -o writeloghex writeloghex.o log.o time.o
	strip writeloghex

gettime.o: dummy
	gcc -c src/example/gettime.c $(CFLAGS)

writelog.o: dummy
	gcc -c src/example/writelog.c $(CFLAGS)

writeloghex.o: dummy
	gcc -c src/example/writeloghex.c $(CFLAGS)

lib_time:
	gcc -c src/util/time.c $(CFLAGS)

lib_log:
	gcc -c src/util/log.c $(CFLAGS)

clean:
	rm -rf *.o *.txt $(BIN)

dummy:
