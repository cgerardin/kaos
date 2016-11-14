#include <stdio.h>
#include <string.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {

	if(argc < 2) {
	
		printf("Usage : %s [bmpfile]\n", argv[0]);
		return -1;
		
	}	

	FILE *bmpFile = fopen(argv[1], "rb");
	
	if(bmpFile != NULL) {
	
		bmp test = read_bitmap(bmpFile);
		fclose(bmpFile);
		
		printf("Depth : %i - Compression : %ld\n", test.depth, test.compression);
		
		if(strcmp(test.signature, "BM") == 0 && test.depth == 24 && test.compression == 0) {
		
			printf("Windows Bitmap file (%.1f KB)\n", (float)test.size / 1000.);
			
		} else {
			printf("Invalid file format\n");
			return -1;
		}
		
	} else {
		printf("Unable to open file %s\n", argv[1]);
		return -1;
	}

	return 0;
	
}
