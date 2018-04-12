#include <math.h>
#include <stdlib.h> 
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#ifndef __TEXTURESEGMENTATION_H__
#define __TEXTURESEGMENTATION_H__


#define NUM_FEATURES	9
#define NUM_DATA_PTS	270000
#define NUM_CENTROIDS   6
#define ITERATIONS_MAX	300


float* pixelposition(float *image, int i,int j,int  width, int height, int bpp);
uint8_t* pixelposition1(uint8_t *image, int i,int j,int  width, int height, int bpp);
float get_image_mean(uint8_t *image,float *Zero_Mean_Img, int width, int height, int bpp);
void get_features(float* img, float* buffer,float **features, int width, int height, int bpp);
float calc_centroid_dist(float centroids[NUM_FEATURES], float *features);
int min_dist_index(float *distance);
void centroids_updated(float **features, float centroids[NUM_CENTROIDS][NUM_FEATURES], int *pred_class);
void clustering_k_means(float **features, int *pred_class, float centroids[NUM_CENTROIDS][NUM_FEATURES]);
void segment_textures(int argc, char *argv[]);
int main(int argc, char* argv[]);

#endif 