#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "Image.h"

enum state {
	ReadingWidth, ReadingHeight, ReadingMaxval
};

int loadImage(Image* img, const char* src)
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
			if (isspace(b) && b != ' ')
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
			else
			{
				if (b == '#')
				{
					comment = 1;
				}
				else if (readingState == ReadingWidth)
				{
					img->width *= 10;
					if (b < '0' || b > '9')
					{
						fclose(f);
						return 1;
					}
					else
					{
						img->width += b - '0';
					}
				}
				else if (readingState == ReadingHeight)
				{
					img->height *= 10;
					if (b < '0' || b > '9')
					{
						fclose(f);
						return 1;
					}
					else
					{
						img->height += b - '0';
					}
				}
			}
		}
		
		if (readingState > ReadingMaxval) break;
		
		previousByte = b;
		fread(&b, 1, 1, f);
	}
	
	//Read file content
	int size = 3 * img->width * img->height;
	img->data = malloc(size);
	fread(img->data, 1, size, f);
	
	fclose(f);
	return 0;
}