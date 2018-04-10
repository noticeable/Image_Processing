#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>



#ifndef __MORPHOLOGY_H__
#define __MORPHOLOGY_H__

typedef struct shrink_list_length {
	int length;
	int count;
	struct shrink_list_length *next;

} shrink_list_length;



uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp);
void binarize_inp(uint8_t *inputimage, uint8_t *binary_inp, int width, int height, int bpp,int type, int foreground);
uint8_t current_window(uint8_t *binary_inp, int i,int j, int width, int height, int bpp);
int shrinked_dots(uint8_t *binary_inp,int width,int height,int bpp);
void shrink_list_creation(int length,int count);
void shrink_list_add(int length,int count);
void free_list();
int bond_mask(uint8_t window);
int bond_elements_stage1(int bond,int type);
uint8_t mask_stage1(int bond, int index, int type);
void stage1(uint8_t *inputimage, uint8_t *binary_inp, uint8_t *M, int type,int width,int height, int bpp);
void m_compliment(uint8_t *inputimage, uint8_t *M, uint8_t *Mbar,int width,int height,int bpp);
int bond_elements_stage2(int bond, int type);
uint8_t mask_stage2(int bond, int index, int type );
void stage2(uint8_t *inputimage, uint8_t *M, uint8_t *prob_hit, int type,int width,int height, int bpp);
void binary_output(uint8_t *Mbar, uint8_t *prob_hit, uint8_t* binary_inp, int width, int height, int bpp);
void get_star_count(uint8_t *inputimage, uint8_t *binary_inp);
int main(int argc, char* argv[]);

#endif