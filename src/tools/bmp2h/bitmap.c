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


    /*if(fread(&bmpData.image_header, sizeof(bmpData.image_header), 1, bmpFile)<1) {
        printf("Erreur de lecture du fichier (2)\n");
    } else {
        printf("Largeur : %lu\n", bmpData.image_header.width);
    }*/
    
    /*fseek(bmpFile, bmpData.header.data_offset, SEEK_SET);

	BMP_COLOR pixel;
	int i=0;
	while(fread(&pixel, sizeof(pixel), 1, bmpFile)>0) {
        printf("Pixel : %d %d %d\n", pixel.r, pixel.g, pixel.b);
        i++;
    }
	*/

	/*
	unsigned char buffer[10000];
	unsigned long data_offset=0;
	bmp bmpData = {"", 0, 0, 0, 0, 0, 0, {{0}, {0}, {0}}};
	
	fread(buffer, sizeof(buffer), 1, bmpFile);
	
	for(int i=0; i<sizeof(buffer); i++) {

		// Signature
		if(i==0) {
			memcpy(bmpData.signature, buffer, 2);
			bmpData.signature[2] = '\0';
		}
		if(i==2) memcpy(&bmpData.size, buffer+i, 4); // File size
		if(i==10) memcpy(&data_offset, buffer+i, 4); // Data offset
		if(i==14) memcpy(&bmpData.header_size, buffer+i, 4); // Image header size
		if(i==18) memcpy(&bmpData.width, buffer+i, 4); // Width
		if(i==22) memcpy(&bmpData.height, buffer+i, 4); // Height
		if(i==28) memcpy(&bmpData.depth, buffer+i, 2); // Depth
		if(i==30) memcpy(&bmpData.compression, buffer+i, 4); // Compression
		
		if(bmpData.header_size != 0 && i>=data_offset) {
		
			memcpy(&bmpData.image_data[0].b, buffer+i, 4);
			memcpy(&bmpData.image_data[0].g, buffer+i+4, 4);
			memcpy(&bmpData.image_data[0].r, buffer+i+8, 4);

			printf("%d %d %d - ", bmpData.image_data[0].r, bmpData.image_data[0].g, bmpData.image_data[0].b);
			
			i += 12+4; //(on saute 'X')
		
		}
		
		//if(bmpData.size != 0 && i>=bmpData.size) break;
	
	}
	
	*/
	
	return bmpData;

}
