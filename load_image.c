#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "image.h"

enum state {
	ReadingWidth, ReadingHeight, ReadingMaxval
};

int read_number(unsigned int* number, byte b)
{
	*number *= 10;
	if (b < '0' || b > '9')
	{
		return 1;
	}
	else
	{
		*number += b - '0';
	}
	return 0;
}

int load_image(Image* img, const char* src)
{
	img->width = 0;
	img->height = 0;
	FILE* f;
	f = fopen(src, "rb");

	if (!f)
	{
		return -1;
	}

	word identifier = 0;
	fread(&identifier, 1, 2, f);

	if (identifier != 'P' + '6' * 256)
	{
		fclose(f);
		return 1;
	}

	//Read file header
	char readingState = ReadingWidth;
	char comment = 0;
	byte previousByte;
	byte b;
	fread(&previousByte, 1, 1, f);
	fread(&b, 1, 1, f);
	while (!feof(f))
	{
		if (comment)
		{
			if (b == '\n')
			{
				comment = 0;
			}
		}
		else
		{
			if (isspace(b))
			{
				if (!isspace(previousByte))
				{
					readingState++;
				}
			}
			else if (b == '#')
			{
				comment = 1;
			}
			else if (readingState == ReadingWidth)
			{
				if (read_number(&img->width, b))
				{
					fclose(f);
					return 1;
				}
			}
			else if (readingState == ReadingHeight)
			{
				if (read_number(&img->height, b))
				{
					fclose(f);
					return 1;
				}
			}
		}
		
		if (readingState > ReadingMaxval) break;
		
		previousByte = b;
		fread(&b, 1, 1, f);
	}
	
	if (feof(f))
	{
		fclose(f);
		return 1;
	}

	//Read file content
	int size = 3 * img->width * img->height;
	img->data = malloc(size);
	fread(img->data, 1, size, f);
	
	fclose(f);
	return 0;
}
