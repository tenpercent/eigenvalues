CC = g++
CARG = -Wall -O3
CDARG = -Wall -O3 -g

all: main

main:
	$(CC) $(CARG) main.cpp qr_uptriangle.cpp multiplication.cpp matrix_init.cpp
debug:
	$(CC) $(CDARG) main.cpp qr_uptriangle.cpp multiplication.cpp matrix_init.cpp
clean:
	rm *.out
