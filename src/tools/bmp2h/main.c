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
			 * Display picture
			 */
			int j=1;
			unsigned char r, g, b;
			
			for(int i=0; i<=bitmap.height*bitmap.width-1; i++) {

				r = bitmap.image_data[i].r;
				g = bitmap.image_data[i].g;
				b = bitmap.image_data[i].b;

		        if(r>200 || g>200 || b>200) {
					printf("#");
				} else if(r>100 || g>100 || b>100) {
					printf("+");
				} else if(r>50 || g>50 || b>50) {
					printf(".");
				} else {
					printf(" ");
				}

				if(j==bitmap.width) {
					printf("\n");
					j=1;
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
