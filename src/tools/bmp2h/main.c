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
			
			/* 
			 * Display pixels
			 * TODO : invert image horizontaly
			 */
			int j=0;
			unsigned char r, g, b;
			
			for(int i=bitmap.height*bitmap.width-1; i>=0; i--) {
			
				r = bitmap.image_data[i].r;
				g = bitmap.image_data[i].g;
				b = bitmap.image_data[i].b;
			
				if(r>127 || g>127 || b>127) {
					printf("+");
				} else {
					printf(" ");
				}
				
				if(j==bitmap.width-1) {
					printf("\n");
					j=0;
				} else {				
					j++;
				}
				
			}
			
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
