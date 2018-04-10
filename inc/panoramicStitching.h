#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>



#ifndef __PANORAMICSTITCHING_H__
#define __PANORAMICSTITCHING_H__

/*
	PROGRAM DESCRIPTION
	performs Image stitching to form panorama of 3 images
*/

void print_matrix(float **matrix, int col, int row);
float** matrix_multiplication(float **A,int a_i,int a_k, float **b, int b_k,int b_j);
int main(int argc, char* argv[]);

#endif