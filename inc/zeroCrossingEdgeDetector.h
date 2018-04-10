#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include <string.h>
#include <errno.h>

#ifndef __ZEROCROSSINGEDGEDETECTOR_H__
#define __ZEROCROSSINGEDGEDETECTOR_H__

uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp);
int* pixelposition1(int *inputimage, int i,int j,int  width, int height, int bpp);
int main(int argc, char* argv[]);

#endif