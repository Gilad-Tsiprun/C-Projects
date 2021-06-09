CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g
KN_TOUR=knight_tour
INC_BIT=../bit_array
BITARRAY=$(INC_BIT)/bit_array
OBJ = $(BITARRAY).o knight_tour.o knight_tour_test.o
TARGET=a.out

a.out: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ -I$(INC_BIT) -lm

knight_tour.o: $(KN_TOUR).c $(KN_TOUR).h $(BITARRAY).o
	$(CC) $(CFLAGS) -c $(KN_TOUR).c -I$(INC_BIT)

knight_tour_test.o: $(KN_TOUR)_test.c $(KN_TOUR).h $(KN_TOUR).o
	$(CC) $(CFLAGS) -c $(KN_TOUR)_test.c

bit_array.o: $(BITARRAY).c $(BITARRAY).h
	$(CC) $(CFLAGS) -c $(BITARRAY).c
