#include <stdio.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {

	if(argc < 2) {
	
		printf("Usage : %s [bmpfile]\n", argv[0]);
		return -1;
		
	}	

	FILE *bmpFile = fopen(argv[1], "rb");
	
	if(bmpFile != NULL) {
	
		bmp test = read_bitmap(bmpFile);
		printf("%s\n", test.signature);

		fclose(bmpFile);
		
	} else {
	
		printf("Unable to open file %s\n", argv[1]);
		return -1;
		
	}

	return 0;
	
}
