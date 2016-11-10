#ifndef H_BITMAP
#define H_BITMAP

typedef struct bmp bmp;
struct bmp {
	char signature[3];
	int size;
};

bmp read_bitmap(FILE *bmpFile);

#endif /* H_BITMAP */
