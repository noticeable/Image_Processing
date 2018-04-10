#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


#ifndef __OILPAINTINGEFFECT_H__
#define __OILPAINTINGEFFECT_H__


/*
	PROGRAM DESCRIPTION
	Performs oil painting effect by graylevel quantization
*/


uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp);
int maximum_array(int a[ ], int arraysize);
int main(int argc, char* argv[]);

#endif