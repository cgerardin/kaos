TOOLS	= bmp2h
SHELL	= /bin/bash

all:
	@for tool in $(TOOLS); do \
		$(MAKE) -C $$tool; \
	done

.PHONY: clean

clean:
	@for tool in $(TOOLS); do \
		$(MAKE) clean -C $$tool; \
	done
	
mrproper: 
	@rm -r ../../tools

