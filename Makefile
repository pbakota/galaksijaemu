
CFLAGS=-Wall -DDEBUG -DLSB_FIRST -I. -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
LDFLAGS=-g -L/usr/lib/x86_64-linux-gnu -lSDL2
BAS2BIN=./bas2bin
BIN2GTP=./bin2gtp

all: galaxy Galaxy.c Z80/Z80.c Z80/Z80.h gtp.c Z80/dasm Z80/dasm.c bas2bin bin2gtp \
	SZ szunyog.bas

Galaxy.o : Galaxy.c gtp.c io.c
	$(CC) -c Galaxy.c -o Galaxy.o $(CFLAGS)

Z80.o : Z80/Z80.c Z80/Z80.h
	$(CC) -c Z80.c -o Z80.o $(CFLAGS)

dasm.o : Z80/dasm.c
	$(CC) -c dasm.c -o dasm.o $(CFLAGS)
	
galaxy: Galaxy.o Z80/Z80.o
	$(CC) Galaxy.o Z80/Z80.o -o galaxy $(LDFLAGS)

dasm: Z80/dasm.o
	$(CC) Z80/dasm.o -o dasm $(LDFLAGS)

bas2bin : bas2bin.c basic.c basic.h
	$(CC) bas2bin.c -o bas2bin $(CFLAGS)

bin2gtp : bin2gtp.c gtp.c
	$(CC) bin2gtp.c -o bin2gtp $(CFLAGS) $(LDFLAGS)

SZ: szunyog.bas bas2bin bin2gtp
	$(BAS2BIN) szunyog.bas -o szunyog.bo
	$(BIN2GTP) szunyog.bo -o SZ

clean:
	rm -f galaxy *.o bin2gtp bas2bin Z80/*.o *.bo SZ Z80/dasm
