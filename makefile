//TO BE UPDATED

CC=g++
CFLAGS=-L.
LIBS=-litpp
LDIR=./lib
DEPS=market.h portfolio.h

%.o: %.c $(DEPS)
        $(CC) -c -o $@ $< $(CFLAGS)

my wallet: market.o portfolio.o main_example.o
        $(CC) -o mywallet market.o portfolio.o main_example.o
