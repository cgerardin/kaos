#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmap.h"

bmp read_bitmap(FILE *bmpFile) {

	unsigned char buffer[50];
	bmp bmpData = {"", 0};
	
	if(fread(buffer, sizeof(buffer), 1, bmpFile) > 0) {
		
		memcpy(bmpData.signature, buffer, 2);
		bmpData.signature[2] = '\0';
		memcpy(&bmpData.size, buffer+2, 4);

	}
	
	return bmpData;

}
