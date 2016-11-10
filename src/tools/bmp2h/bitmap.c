#include <stdio.h>
#include "bitmap.h"

bmp read_bitmap(FILE *bmpFile) {

	unsigned char buffer[50];
	bmp bmpData = {"", 0};
	
	if(fread(buffer, sizeof(buffer), 1, bmpFile) > 0) {
		
		bmpData.signature[0] = buffer[0];
		bmpData.signature[1] = buffer[1];
		bmpData.signature[2] = '\0';

	}
	
	return bmpData;

}
