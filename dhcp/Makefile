CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -lm
DHCP=dhcp
TRIE=trie
DEPS = dhcp.h trie.h
OBJ = trie.o dhcp.o dhcp_test.o

a.out: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

dhcp.o: $(DHCP).c $(DHCP).h $(TRIE).o
	$(CC) $(CFLAGS) -c $(DHCP).c

dhcp_test.o: $(DHCP)_test.c $(DHCP).h $(DHCP).o
	$(CC) $(CFLAGS) -c $(DHCP)_test.c

trie.o: $(TRIE).c $(TRIE).h
	$(CC) $(CFLAGS) -c $(TRIE).c
