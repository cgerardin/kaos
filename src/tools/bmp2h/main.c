#include <stdio.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {

	if(argc < 2) {
	
		printf("Usage : %s [bmpfile]\n", argv[0]);
		return -1;
		
	}	

	FILE *bmpFile = fopen(argv[1], "rb");
	
	if(bmpFile != NULL) {
	
		BMP bitmap = read_bitmap(bmpFile);
		fclose(bmpFile);
		
		if(bitmap.magic_number == BMP_BM && bitmap.depth == 24 && bitmap.compression == 0) {
		
			printf("Windows Bitmap file (%dx%d px, %d bpp, %.1f KB)\n", 
				bitmap.width, bitmap.height, bitmap.depth, (float)bitmap.size / 1000.);
			
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
