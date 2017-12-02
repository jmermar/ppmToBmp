#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include "Image.h"

int saveImage(Image* img, const char* path)
{
	int padding = (4 - (3 * img->width) % 4) % 4;
	int sizeRow = 3 * img->width + padding;
	FILE* f;
	f = fopen(path, "wb");

	//Writting the header
	byte fileHeader[14];
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	((dword*)(fileHeader + 2))[0] = 54 + 3 * (img->width + padding) * img->height; //File size in bytes
	((dword*)(fileHeader + 2))[2] = 54; //Offset to start of pixel data

	fwrite(fileHeader, 1, 14, f);

	//Writting image header
	byte imageHeader[40];
	((dword*)imageHeader)[0] = 40; //Size of imageHeader
	((dword*)imageHeader)[1] = img->width; //image width in pixels
	((dword*)imageHeader)[2] = img->height; //image height in pixels
	((word*)imageHeader)[6] = 1; //Must be 1
	((word*)imageHeader)[7] = 24; //Bits per pixel
	((dword*)imageHeader)[4] = 0; //Compresion type (0 for uncompressed)
	((dword*)imageHeader)[5] = 0; //Image Size (0 for uncompressed)
	((dword*)imageHeader)[6] = 0; //Bits per metter
	((dword*)imageHeader)[7] = 0; //Bits per metter
	((dword*)imageHeader)[8] = 0; //Color maps used
	((dword*)imageHeader)[9] = 0; //Significant colors

	fwrite(imageHeader, 1, 40, f);

	dword blank = 0;

	//Writting pixel data
	byte *bmpData = malloc(3 * img->width * img->height);

	for (int i = img->height - 1; i >= 0; i--)
	{
		for (int j = 0; j < img->width; j++)
		{
			//converting the img rgb pixel data to bgr
			bmpData[i * sizeRow + 3 * j]		= img->data[i * 3 * img->width + 3 * j + 2];
			bmpData[i * sizeRow + 3 * j + 1]	= img->data[i * 3 * img->width + 3 * j + 1];
			bmpData[i * sizeRow + 3 * j + 2]	= img->data[i * 3 * img->width + 3 * j];
		}
		for (int j = 0; j < padding; j++)
		{
			bmpData[i * sizeRow + 3 * img->width + j] = 0;
		}
	}

	fwrite(bmpData, 1, sizeRow * img->height, f);

	free(bmpData);
	fclose(f);
	return 0;
}