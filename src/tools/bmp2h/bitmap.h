#ifndef H_BITMAP
#define H_BITMAP

typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;	
} BMP_COLOR;

typedef struct {
	unsigned char magic_number[2];
	unsigned long size;
	unsigned int reserved1;
	unsigned int reserved2;
	unsigned long  data_offset;
} BMP_HEADER;

typedef struct {
	unsigned long image_header_size;
	unsigned long width;
	unsigned long height;
	unsigned int layouts; // 1
	unsigned int depth;
	unsigned long compression;
	unsigned long image_size;
	unsigned long horizontal_res;
	unsigned long vertical_res;
	unsigned long palette_colors;
	unsigned long important_palette_colors;
} BMP_IMAGE_HEADER;

typedef struct {
	BMP_HEADER header;
	BMP_IMAGE_HEADER image_header;
	BMP_COLOR image[100000];
} BMP;

BMP read_bitmap(FILE *bmpFile);

#endif /* H_BITMAP */
