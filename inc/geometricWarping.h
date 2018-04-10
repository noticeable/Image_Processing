#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#ifndef __GEOMETRICWARPING_H__
#define __GEOMETRICWARPING_H__

uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp);
void bilinear_interpolation(uint8_t *inputimage,float i1,float j1, int width, int height, int bpp, uint8_t *outputs);
int main(int argc, char* argv[]);

#endif