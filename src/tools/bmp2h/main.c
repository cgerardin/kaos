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
		
			printf("Windows Bitmap file (%.1f KB)\n\n", (float)bitmap.size / 1000.);
			
			printf("Size : %u\n", bitmap.size);
			printf("Reserved : %u%u\n", bitmap.reserved1, bitmap.reserved2);
			printf("Offset : %u\n", bitmap.data_offset);
			printf("Header size : %d\n", bitmap.image_header_size);
			printf("Width : %d\n", bitmap.width);
			printf("Height : %d\n", bitmap.height);
			printf("Layouts : %d\n", bitmap.layouts);
			printf("Depth : %d\n", bitmap.depth);
			printf("Compression : %d\n", bitmap.compression);
			printf("Image size : %d\n", bitmap.image_size);
			printf("Horizontal Res : %d\n", bitmap.horizontal_res);
			printf("Vertical Res : %d\n", bitmap.vertical_res);
			printf("Palette colors : %d\n", bitmap.palette_colors);
			printf("Important palette colors : %d\n", bitmap.important_palette_colors);
			
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
