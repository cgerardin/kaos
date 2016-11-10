EXEC			= bmp2h
CC				= clang
CFLAGS			= -std=c11 -Wall -O3
SHELL			= /bin/bash

all: $(EXEC)
	
$(EXEC): 
	@mkdir -p ../../../tools/
	@$(CC) $(CFLAGS) -o ../../../tools/$@ main.c

.PHONY: clean

clean:
	@rm -rf *.o

mrproper: clean
	@rm ../../../tools/$(EXEC)

