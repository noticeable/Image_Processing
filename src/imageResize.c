/*
Code written by:
Name: 				Shreya Gupta
USC ID:				3421233566
Email ID :			shreyagu@usc.edu
Submission date:	2/4/2018
Code for: 			Bilinear Transform
*/
#include "imageResize.h"
/*
	PROGRAM DESCRIPTION
	Performs bilinear transformation on images to resize them.

*/


/* Function: pixelposition
	input: input image, pixel at (i,j), width, height, bytes per pixel
	output:  pointer at the location in the 1D array that is pointing to the (i,j)th pixel value
*/
uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontal and i for vertical 
}

int main(int argc, char* argv[])
{
	FILE *f_ptr = NULL;
	uint8_t *inputimage = NULL;
	uint8_t  *buffer = NULL;
	uint8_t  *resized_image = NULL;
	int input_height = 0;
	int input_width = 0;
	int bpp = 0;
	int i = 0;
	int j = 0;
	int output_height = 0;
	int output_width = 0;
	uint8_t *F_x_y = NULL;
	uint8_t *Right_pixel = NULL;
	uint8_t *top_pixel = NULL;
	uint8_t *bottom_right = NULL;
	float row_scale = 0;
	float col_scale = 0;
	int q_out = 0;
	int p_out = 0;

	if(argc < 7)
	{
		printf("Usage: %s <image name> <image width> <image height> <output width> <output height> <image bpp>\n", argv[0]);
		exit(1);
	}

	input_width = atoi (argv[2]);
	input_height= atoi (argv[3]);
	output_width = atoi (argv[4]);
	output_height= atoi (argv[5]);
	bpp = atoi (argv[6]);

	inputimage=(uint8_t*)malloc((input_width*input_height*bpp)*sizeof(uint8_t*));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	f_ptr=fopen(argv[1],"r");
	if (f_ptr==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno)); 
		exit(0);
	}
	fread(inputimage,1,input_width*input_height*bpp, f_ptr);
	fclose(f_ptr);

	buffer=(uint8_t*)malloc((input_width*input_height*bpp)*sizeof(uint8_t*));
	resized_image=(uint8_t*)malloc((output_width*output_height*bpp)*sizeof(uint8_t*));
	printf("%d \n", __LINE__);



	/*Bilinear interpolation code*/
	row_scale= input_width/(float )output_width;
	col_scale= input_height/(float )output_height;

	for(q_out=0;q_out< output_height;q_out++)
	{
		for(p_out=0;p_out<output_width;p_out++)
		{
			float p_frac= p_out * row_scale;
			float q_frac= q_out * col_scale;
			int p= floor(p_frac);
			int q= floor(q_frac);
			float del_p= p_frac - p;
			float del_q= q_frac - q;


			F_x_y=pixelposition(inputimage, p, q, input_width, input_height, bpp);
			Right_pixel=pixelposition(inputimage, p, (q+1), input_width, input_height, bpp);
			top_pixel=pixelposition(inputimage, (p+1), q, input_width, input_height, bpp);
			bottom_right=pixelposition(inputimage, (p+1), (q+1), input_width, input_height, bpp);
			
			*(resized_image +bpp*(p_out*output_width + q_out))= (1- del_p)*(1-del_q)* (*F_x_y)+
													(1- del_p)*(del_q)* (*Right_pixel)+
													(del_p)*(1-del_q)* (*top_pixel)+
													(del_p)*(del_q)* (*bottom_right);

			*(resized_image +bpp*(p_out*output_width + q_out)+1)= (1- del_p)*(1-del_q)* (*(F_x_y+1))+
													(1- del_p)*(del_q)* (*(Right_pixel+1))+
													(del_p)*(1-del_q)* (*(top_pixel+1))+
													(del_p)*(del_q)* (*(bottom_right+1));

			*(resized_image +bpp*(p_out*output_width + q_out)+2)= (1- del_p)*(1-del_q)* (*(F_x_y+2))+
													(1- del_p)*(del_q)* (*(Right_pixel+2))+
													(del_p)*(1-del_q)* (*(top_pixel+2))+
													(del_p)*(del_q)* (*(bottom_right+2));

			
		}
	}
	
	f_ptr= fopen("resizedImage.raw","w");
	if (f_ptr==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno)); 
		exit(0);
	}
	fwrite(resized_image,1,output_width*output_height*bpp,f_ptr);
	fclose(f_ptr);


FREE_MEM:

	free(inputimage);
	inputimage = NULL;
	free(buffer);
	buffer = NULL;
	free(resized_image);
	resized_image = NULL;
	
	return 0;
}

