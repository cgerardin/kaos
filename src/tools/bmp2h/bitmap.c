#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmap.h"

BMP read_bitmap(FILE *bmpFile) {

	unsigned char bmpHeader[54];
	BMP bmpData;
	
	fread(&bmpHeader, sizeof(bmpHeader), 1, bmpFile);
	
	memcpy(&bmpData.magic_number, bmpHeader+0, 2);
	memcpy(&bmpData.size, bmpHeader+2, 4);
	memcpy(&bmpData.reserved1, bmpHeader+6, 2);
	memcpy(&bmpData.reserved2, bmpHeader+8, 2);
	memcpy(&bmpData.data_offset, bmpHeader+10, 4);
    memcpy(&bmpData.image_header_size, bmpHeader+14, 4);
	memcpy(&bmpData.width, bmpHeader+18, 4);
	memcpy(&bmpData.height, bmpHeader+22, 4);
	memcpy(&bmpData.layouts, bmpHeader+26, 2);
	memcpy(&bmpData.depth, bmpHeader+28, 2);
	memcpy(&bmpData.compression, bmpHeader+30, 4);
	memcpy(&bmpData.image_size, bmpHeader+34, 4);
	memcpy(&bmpData.horizontal_res, bmpHeader+38, 4);
	memcpy(&bmpData.vertical_res, bmpHeader+42, 4);
	memcpy(&bmpData.palette_colors, bmpHeader+46, 4);
	memcpy(&bmpData.important_palette_colors, bmpHeader+50, 4);
	
	fseek(bmpFile, bmpData.data_offset, SEEK_SET);
	
	// Get piexls informations
	// Todo : align bits to x4 for each line of the picture
	BMP_COLOR pixel;
	int i=0;
	while(fread(&pixel, sizeof(pixel), 1, bmpFile)) {
        printf("Pixel[%d] : %d %d %d\n", i, pixel.r, pixel.g, pixel.b);
        i++;
    }
	
	return bmpData;

}
