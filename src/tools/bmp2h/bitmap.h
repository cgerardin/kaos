#ifndef H_BITMAP
#define H_BITMAP

typedef struct {
	char signature[3];
	unsigned long size;
	unsigned long header_size;
	unsigned long width;
	unsigned long height;
	unsigned short depth;
	unsigned long compression;
} bmp;

bmp read_bitmap(FILE *bmpFile);

#endif /* H_BITMAP */
