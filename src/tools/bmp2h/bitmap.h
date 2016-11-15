#ifndef H_BITMAP
#define H_BITMAP

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;	
} color;

typedef struct {
	char signature[3];
	unsigned long size;
	unsigned long header_size;
	unsigned long width;
	unsigned long height;
	unsigned short depth;
	unsigned long compression;
	color image_data[100000];
} bmp;

bmp read_bitmap(FILE *bmpFile);

#endif /* H_BITMAP */
