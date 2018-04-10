#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#ifndef __HISTOGRAMEQUALIZTION_H__
#define __HISTOGRAMEQUALIZATION_H__


/*
	PROGRAM DESCRIPTION
	Performs histogram eqaliztion by tranfer function method. 
*/


uint8_t max_of_3(uint8_t a, uint8_t b, uint8_t c);
uint8_t min_of_3(uint8_t a, uint8_t b, uint8_t c);
int main(int argc, char* argv[]);

#endif