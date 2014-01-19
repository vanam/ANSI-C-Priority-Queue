# compilator
CC=gcc

# compilator flags
CFLAGS=-O3 -std=c89 -Wall -pedantic-errors

# binary file name
BIN=pq_example

# object files needed to make binary file
OBJ=example.o priority_queue.o

# how to make a object file
%.o: %.c clean
	$(CC) -c $(CFLAGS) $< -o $@

# how to make a binary file
$(BIN): $(OBJ)
	@echo -e "\nLinking...."
	$(CC) $(CFLAGS) $^ -o $@
	@echo -e "Linking....OK\n"
	@make clean

# clean after previous compilation
.PHONY: clean
clean:
	@echo -ne "\nCleaning...."
	@rm -f *.o
	@echo -e "OK\n"