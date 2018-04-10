#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#ifndef __SOBELEDGEDETECTOR_H__
#define __SOBELEDGEDETECTOR_H__

uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp);
int main(int argc, char* argv[]);
#endif