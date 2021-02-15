CC=g++
CFLAGS=-O3 -Wall -Wextra -std=c++17

test: bst.cpp bst.hpp iterator.hpp iterator.cpp test.cpp
	$(CC) test.cpp -o test $(CFLAGS)

.PHONY: clean

clean:
	rm test
