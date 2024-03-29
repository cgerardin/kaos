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
	
	// Get pixels informations (with mirroring)
	unsigned char lines[1000][1000];
	int current_pixel=bmpData.width*bmpData.height-1;
	int j=0;
	BMP_COLOR pixel;
	char empty;
	
	// Lines
	for(int i=0; i<bmpData.height; i++) {
		
		// Current line's pixels
		for(j=bmpData.width*3; j>=3; j-=3) {
		
			fread(&lines[i][j], sizeof(char), 1, bmpFile);
			fread(&lines[i][j-1], sizeof(char), 1, bmpFile);
			fread(&lines[i][j-2], sizeof(char), 1, bmpFile);
			
		}
		
		if(current_pixel <= bmpData.width*bmpData.height) {
			// Current line's padding correction
			for(j=0; j<bmpData.width%4; j++) {
				fread(&empty, sizeof(char), 1, bmpFile);
				printf("empty %d\n", j);
			}
		}
		
		// Fill structures
		for(j=0; j<bmpData.width*3; j+=3) {
			pixel.g = lines[i][j];
			pixel.b = lines[i][j+1];
			pixel.r = lines[i][j+2];
			bmpData.image_data[current_pixel] = pixel;
			current_pixel -= 1;
		}
		
	}
	
	return bmpData;

}
