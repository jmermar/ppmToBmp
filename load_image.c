#include <stdio.h>
#include "Image.h"

int loadImage(Image* img, const char* src)
{
	FILE* f;
	f = fopen(src, "rb");

	if (!f)
	{
		return -1;
	}

	U16 word;
	fread(word, sizeof(U16), 1, f);

	if (word != 'p' * 256 + '6')
	{
		return 1;
	}
	
	U8 byte;
	fread(&byte, 1, 1, f);

	for (byte; byte != EOF; fread(&byte, 1, 1, f))
	{

	}

	fclose(f);
}