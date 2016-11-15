EXEC			= bmp2h
CC				= clang
DEBUG			= true
CFLAGS			= -std=c11 -Wall
ifeq ($(DEBUG), true)
	CFLAGS		+= -g
else
	CFLAGS		+= -O3
endif
SHELL			= /bin/bash

all: $(EXEC)
	
$(EXEC): 
	@mkdir -p ../../../tools/
	@$(CC) $(CFLAGS) -o ../../../tools/$@ main.c bitmap.c

.PHONY: clean

clean:
	@rm -rf *.o

mrproper: clean
	@rm ../../../tools/$(EXEC)

