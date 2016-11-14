#ifndef H_BITMAP
#define H_BITMAP

typedef struct {
	char signature[3];
	unsigned long size;
} bmp;

bmp read_bitmap(FILE *bmpFile);

#endif /* H_BITMAP */
