#include <stdio.h>
#include <string.h>

void ReadNextDataLine(char *buffer, size_t buffer_size, FILE *file) {
	
	do {
		fgets(buffer, buffer_size, file);
	} while(buffer[0]=='#');

}

int main(int argc, char *argv[]) {

	int width, height;
	int r, g, b;

	char buffer[255];
	char buffer2[255];

	char h_name[255];
	char h_filename[255];

	if(argc < 3) {
	
		printf("Usage : %s [ppmfile] [name]\n", argv[0]);
		return -1;
		
	}

	strcpy(h_name, argv[2]);
	strcpy(h_filename, argv[2]);
	strcat(h_filename, ".h");	

	FILE *in = fopen(argv[1], "r");
	FILE *out = fopen(h_filename, "w");

	ReadNextDataLine(buffer, sizeof(buffer), in); // Signature
	ReadNextDataLine(buffer, sizeof(buffer), in); // Size
	sscanf(buffer, "%d %d", &width, &height);
	ReadNextDataLine(buffer, sizeof(buffer), in); // Colors

	sprintf(buffer2, "#ifndef H_PICTURES_%s\n#define H_PICTURES_%s\n", h_name, h_name);
	fputs(buffer2, out);

	sprintf(buffer2, "uint32_t picture_%s_width=%d;\nuint32_t picture_%s_height=%d;\n", h_name, width, h_name, height);
	fputs(buffer2, out);

	sprintf(buffer2, "uint32_t picture_%s[%d]={\n", h_name, width*height);
	fputs(buffer2, out);

	// Pixels
	for(int i=0; i<height*width; i++) {
	
		ReadNextDataLine(buffer, sizeof(buffer), in);
		sscanf(buffer, "%d", &r);
		ReadNextDataLine(buffer, sizeof(buffer), in);
		sscanf(buffer, "%d", &g);
		ReadNextDataLine(buffer, sizeof(buffer), in);
		sscanf(buffer, "%d", &b);

		sprintf(buffer2, "%d,", ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
		fputs(buffer2, out);

	}

	sprintf(buffer2, "};\n#endif /* H_PICTURES_%s */\n", h_name);
	fputs(buffer2, out);
	
	fclose(out);
	fclose(in);	

	return 0;

}




