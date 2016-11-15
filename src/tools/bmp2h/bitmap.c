#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmap.h"

bmp read_bitmap(FILE *bmpFile) {

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
	
	return bmpData;

}
