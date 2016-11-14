#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmap.h"

bmp read_bitmap(FILE *bmpFile) {

	unsigned char buffer[10000];
	bmp bmpData = {"", 0, 0, 0, 0, 0, 0};
	
	if(fread(buffer, sizeof(buffer), 1, bmpFile) > 0) {
	
		unsigned long dataOffset=0;
		unsigned long imageData=0;
		
		for(int i=0; i<sizeof(buffer); i++) {
		
			// Signature
			if(i==0) {
				memcpy(bmpData.signature, buffer, 2);
				bmpData.signature[2] = '\0';
			}
			if(i==2) memcpy(&bmpData.size, buffer+i, 4); // File size
			if(i==10) memcpy(&dataOffset, buffer+i, 4); // Data offset
			if(i==14) memcpy(&bmpData.header_size, buffer+i, 4); // Image header size
			if(i==18) memcpy(&bmpData.width, buffer+i, 4); // Width
			if(i==22) memcpy(&bmpData.height, buffer+i, 4); // Height
			if(i==28) memcpy(&bmpData.depth, buffer+i, 2); // Depth
			if(i==30) memcpy(&bmpData.compression, buffer+i, 4); // Compression
			
			
			if(i>=1078) {
			
				memcpy(&imageData, buffer+i, 4);
				printf("%ld ", imageData);
			
			}
			
		};
		
	}
	
	return bmpData;

}
