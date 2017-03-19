BIT := $(shell getconf LONG_BIT)

CFLAGS = -g -O2 -m$(BIT) -I./include

BIN = gettime writelog writeloghex initest

all: gettime writelog writeloghex initest

gettime: dummy gettime.o lib_time
	gcc -o gettime gettime.o time.o
	strip gettime

writelog: dummy writelog.o lib_log lib_time
	gcc -o writelog writelog.o log.o time.o
	strip writelog

writeloghex: dummy writeloghex.o lib_log lib_time
	gcc -o writeloghex writeloghex.o log.o time.o
	strip writeloghex

initest: dummy initest.o lib_ini
	gcc -o initest initest.o ini.o
	strip initest

gettime.o: dummy
	gcc -c src/example/gettime.c $(CFLAGS)

writelog.o: dummy
	gcc -c src/example/writelog.c $(CFLAGS)

writeloghex.o: dummy
	gcc -c src/example/writeloghex.c $(CFLAGS)

initest.o: dummy
	gcc -c src/example/initest.c $(CFLAGS)

lib_time:
	gcc -c src/util/time.c $(CFLAGS)

lib_log:
	gcc -c src/util/log.c $(CFLAGS)

lib_ini:
	gcc -c src/util/ini.c $(CFLAGS)

clean:
	rm -rf *.o *.txt $(BIN)

dummy:
