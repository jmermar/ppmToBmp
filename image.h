#ifndef IMAGE_H
#define IMAGE_H

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

typedef struct {
	unsigned int width, height;
	byte* data;
} Image;

int load_image(Image* img, const char* src);
int save_image(Image* img, const char* path);

#endif
