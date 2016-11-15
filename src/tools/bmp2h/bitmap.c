#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmap.h"

BMP read_bitmap(FILE *bmpFile) {

	BMP bmpData;

	if (fread(&bmpData.header, sizeof(bmpData.header), 1, bmpFile)<1) {
        printf("Erreur de lecture du fichier (1)\n");
    } else {
        printf("L'offeset du premier octet est : %lu\n", bmpData.header.data_offset);
        printf("Magic number: %c%c\n", bmpData.header.magic_number[0], bmpData.header.magic_number[1]);
    }
    
    if (fread(&bmpData.image_header, sizeof(bmpData.image_header), 1, bmpFile)<1) {
        printf("Erreur de lecture du fichier (2)\n");
    } else {
        printf("Largeur : %lu\n", bmpData.image_header.width);
    }
    
    fseek(bmpFile, bmpData.header.data_offset, SEEK_SET);

	BMP_COLOR pixel;
	int i=0;
	while(fread(&pixel, sizeof(pixel), 1, bmpFile)>0) {
        printf("Pixel : %d %d %d\n", pixel.r, pixel.g, pixel.b);
        i++;
    }
	

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
