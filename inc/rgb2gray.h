#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#ifndef __RGB2GRAY_H__
#define __RGB2GRAY_H__


/*
	PROGRAM DESCRIPTION
	Performs rgb to grayscale converison by three methods: 
	Luminosity Method
	Lightness Method
	Averaging
*/


/* Function: max_of_3
	input: r , g, b pixel intensities
	output:  maximum of the rgb values
*/
uint8_t max_of_3(uint8_t a, uint8_t b, uint8_t c);

/* Function: min_of_3
	input: r , g, b pixel intensities
	output:  minimum of the rgb values
*/
uint8_t min_of_3(uint8_t a, uint8_t b, uint8_t c);

int main(int argc, char* argv[]);

#endif