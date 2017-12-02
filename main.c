#include<stdio.h>
#include <stdlib.h>

#include "Image.h"
int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("You should pass an input file and an output file\n");
		return 0;
	}
	Image image;
	image.data = 0;

	int err = loadImage(&image, argv[1]);
	if (err < 0)
	{
		printf("Cannot read file\n");
		return -1;
	}
	else if (err > 0)
	{
		printf("Invalid format\n");
		return 1;
	}

	if (saveImage(&image, argv[2]))
	{
		printf("Cannot write or create file\n");
		return 2;
	}
	free(image.data);
	return 0;
}