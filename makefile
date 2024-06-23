# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c

DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O3

all	: bin/cb
	@echo -n ""

bin/cb		: main.o 
		$(CC) $(OPTFLAGS) $^ -o $@
main.o	: src/main.cpp
		$(CC) $(CFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o