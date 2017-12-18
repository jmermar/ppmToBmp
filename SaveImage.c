#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include "Image.h"

#pragma pack(1)
typedef struct {
	byte type[2];
	dword size;
	word reserved1;
	word reserved2;
	dword offset;
} FileHeader;

#pragma pack(1)
typedef struct {
	dword size;
	dword width;
	dword height;
	word planes; //Must be 1
	word bpp; //Bits per pixel
	dword compression;
	dword sizeImage;
	dword xppm; //x pixels per meter
	dword yppm; //y pixels per meter
	dword clru; //Colors used
	dword clri; //Colors important
} ImageHeader;

int saveImage(Image* img, const char* path)
{
	int padding = (4 - (3 * img->width) % 4) % 4;
	int sizeRow = 3 * img->width + padding;
	FILE* f;
	f = fopen(path, "wb");

	FileHeader fh;
	fh.type[0] = 'B';
	fh.type[1] = 'M';
	fh.reserved1 = fh.reserved2 = 0;
	fh.size = sizeof(FileHeader) + sizeof(ImageHeader) + sizeRow * img->height;
	fh.offset = sizeof(FileHeader) + sizeof(ImageHeader);

	fwrite(&fh, 1, sizeof(fh), f);

	ImageHeader ih;
	ih.size = sizeof(ImageHeader);
	ih.width = img->width;
	ih.height = img->height;
	ih.planes = 1;
	ih.bpp = 24;
	ih.compression = 0;
	ih.sizeImage = 0;
	ih.xppm = 0;
	ih.yppm = 0;
	ih.clru = 0;
	ih.clri = 0;

	fwrite(&ih, 1, sizeof(ih), f);

	dword blank = 0;

	//Writting pixel data
	byte *bmpData = malloc(sizeRow * img->height);

	int e = 0;
	for (int i = img->height - 1; i >= 0; i--)
	{
		for (int j = 0; j < img->width; j++)
		{
			//converting the img rgb pixel data to bgr
			bmpData[e * sizeRow + 3 * j]		= img->data[i * 3 * img->width + 3 * j + 2];
			bmpData[e * sizeRow + 3 * j + 1]	= img->data[i * 3 * img->width + 3 * j + 1];
			bmpData[e * sizeRow + 3 * j + 2]	= img->data[i * 3 * img->width + 3 * j];
		}
		for (int j = 0; j < padding; j++)
		{
			bmpData[i * sizeRow + 3 * img->width + j] = 0;
		}
		e++;
	}

	fwrite(bmpData, 1, sizeRow * img->height, f);

	free(bmpData);
	fclose(f);
	return 0;
}
