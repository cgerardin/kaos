EXEC			= bmp2h
CC				= clang
SHELL			= /bin/bash

all: $(EXEC)
	
$(EXEC): 
	@mkdir -p ../../../tools/
	@$(CC) $(CFLAGS) -o ../../../tools/$@ -c main.c

.PHONY: clean

clean:
	@rm -rf *.o

mrproper: clean
	@rm ../../../tools/$(EXEC)

