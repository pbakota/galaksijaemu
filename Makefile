
CFLAGS=-Wall -DDEBUG -DLSB_FIRST -I. -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
LDFLAGS=-O2 -L/usr/lib/x86_64-linux-gnu -lSDL2

all: galaxy Galaxy.c Z80/Z80.c Z80/Z80.h gtp.c

Galaxy.o : Galaxy.c gtp.c
	$(CC) -c Galaxy.c -o Galaxy.o $(CFLAGS)

Z80.o : Z80/Z80.c Z80/Z80.h
	$(CC) -c Z80.c -o Z80.o $(CFLAGS)

galaxy: Galaxy.o Z80/Z80.o
	$(CC) Galaxy.o Z80/Z80.o -o galaxy $(LDFLAGS)

clean:
	rm -f galaxy *.o Z80/*.o