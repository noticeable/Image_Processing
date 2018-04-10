#include <math.h>
#include <stdlib.h> 
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>


#ifndef __TEXTURECLASSIFICATION_H__
#define __TEXTURECLASSIFICATION_H__

#define 	NUM_DATA_PTS			12
#define 	NUM_FEATURES			9
#define		NUM_CENTROIDS			4
#define 	ITERATIONS_MAX			300

int* pixelposition(int *inputimage, int i,int j,int  width, int height, int bpp);
float get_image_mean(uint8_t *image,int *Zero_Mean_Img, int width, int height, int bpp);
void get_features(int* img, uint8_t* buffer,float features[NUM_DATA_PTS][NUM_FEATURES],int image_num, int width, int height, int bpp);
float calc_centroid_dist(float centroids[NUM_FEATURES], float features[NUM_FEATURES]);
int min_dist_index(float *distance);
void centroids_updated(float features[NUM_DATA_PTS][NUM_FEATURES], float centroids[NUM_CENTROIDS][NUM_FEATURES], int *pred_class);
void clustering_k_means(float features[NUM_DATA_PTS][NUM_FEATURES], int *pred_class, float centroids[NUM_CENTROIDS][NUM_FEATURES]);
void classify_textures();
int main();

#endif