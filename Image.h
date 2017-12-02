#ifndef IMAGE_H
#define IMAGE_H

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

typedef struct {
	unsigned int width, height;
	byte* data;
} Image;

int loadImage(Image* img, const char* src);
int saveImage(Image* img, const char* path);

#endif