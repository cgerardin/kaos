#ifndef H_BITMAP
#define H_BITMAP

#define BMP_BM 0x4D42

typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
} BMP_COLOR;

typedef struct {
	unsigned short magic_number;
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int  data_offset;
	unsigned int image_header_size;
	unsigned int width;
	unsigned int height;
	unsigned int layouts; // 1
	unsigned short depth;
	unsigned int compression;
	unsigned int image_size;
	unsigned int horizontal_res;
	unsigned int vertical_res;
	unsigned int palette_colors;
	unsigned int important_palette_colors;
	BMP_COLOR image_data[100000];
} BMP;

BMP read_bitmap(FILE *bmpFile);

#endif /* H_BITMAP */
