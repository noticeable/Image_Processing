/*
#Code written by:
#Name: 				Shreya Gupta
SOBEL EDGE DETECTOR
*/
#include "SobelEdgeDetector.h"

int N=3;//window size

/* Function: pixewindow_widthposition
	input: input image, pixewindow_width at (i,j), width, height, bytes per pixewindow_width
	output_gray:  pointer at the window location in the 1D array that is pointing to the (i,j)th pixewindow_width vawindow_widthue
*/
uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontawindow_width and i for verticawindow_width 
}



int main(int argc, char* argv[])
{
	FILE *fp1, *fp2;
	int N_gaussian=3;			
	int width 	= 0 ;
	int height 	= 0 ;
	int bpp		= 0 ;
	uint8_t *inputimage 	= NULL;
	float   *output_edges	= NULL;
	uint8_t *output_gray 	= NULL;
	uint8_t *norm_output 	= NULL;
	int der_x=0;
	int der_y=0;
	float sum_der_x=0;
	float sum_der_y=0;
	int i=0;
	int j=0;
	int window_height=0;
	int window_width=0;
	int window_height_new=0;
	int window_width_new=0;

	float I_derX[3][3]={	{-1/4.0, 0, 1/4.0},
							{-2/4.0, 0, 2/4.0},
							{-1/4.0, 0, 1/4.0}
						};
	float I_derY[3][3]={	{1/4.0, 2/4.0, 1/4.0},
							{0, 0, 0},
							{-1/4.0, -2/4.0, -1/4.0}};
	// float I_gaussian[5][5]= {{1/100.0, 2/100.0,4/100.0,2/100.0,1/100.0}, {2/100.0,4/100.0,8/100.0,4/100.0,2/100.0}, {4/100.0,8/100.0,16/100.0,8/100.0,4/100.0} , {2/100.0,4/100.0,8/100.0,4/100.0,2/100.0}, {1/100.0, 2/100.0,4/100.0,2/100.0,1/100.0}};
	float I_gaussian[3][3]={{1/16 , 2/16, 1/16},
							{2/16, 4/16, 2/16},
							{1/16 , 2/16, 1/16}};
	
	
	float first_der=0;
	uint8_t *pixel;
	int x=0;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	float y1;
	float max_gradient=0;
	width 	=atoi(argv[2]);
	height 	=atoi(argv[3]);
	bpp		=atoi(argv[4]);


	inputimage 		=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	output_edges 		=(float*)malloc((width*height)*sizeof(float));
	if(output_edges == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	output_gray 	=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(output_gray == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	norm_output 		=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(norm_output == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}


	fp1=fopen(argv[1],"rb");
	if (fp1==NULL)
	{
		printf("Error %s\n", strerror(errno)); 
		exit(0);
	}
	fread(inputimage,1,width*height*bpp, fp1);
	fclose(fp1);

	

//-------------------RGB to grayscale-------------------------------------------------------------------------------//
	for (x=0;x<width*height;x++)
	{
		pixel = (inputimage + bpp*x); 
		
		r= *(pixel+0);
		g= *(pixel+1);
		b= *(pixel+2);
		y1=(float) 0.21*r+ (float) 0.72*g+ (float) 0.07*b;	// luminosity method
		
		
		*(output_gray+x)= (uint8_t) y1;	
	}

// ---------------------------sobel descriptor---------------------------------------------------------------------////
	//---------------------------------------------gaussian filter-----------------------------------------------------------//

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{			
			sum_der_x=0;
			sum_der_y=0;
			
			for(window_height=i-N_gaussian/2;window_height<=i+N_gaussian/2;window_height++)
			{
				for (window_width=j-N_gaussian/2;window_width<=j+N_gaussian/2;window_width++)
				{
					window_height_new=window_height;
					window_width_new=window_width;

					if(window_height<0)
					{
						window_height_new=abs(window_height)-1;
					}
					if(window_width<0) 
					{
						window_width_new=abs(window_width)-1;
					}
					if (window_height>=height)
					{
						window_height_new=2*height-1-window_height ;
					}
					if (window_width>=width)
					{
						window_width_new=2*width-1-window_width;
					}
			
					
					pixel=pixelposition(output_gray, window_height_new, window_width_new, width, height, 1);
					*(output_gray+ width*i +j) += *( pixel)   * I_gaussian [window_height- i + (N_gaussian/2)][window_width - j + (N_gaussian/2)];

				}
			}	
			
		}
	}

	//-----------------------------------------sobel edge detector---------------------------------------------------------//
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{			
			sum_der_x=0;
			sum_der_y=0;
			 
			for(window_height=i-N/2;window_height<=i+N/2;window_height++)
			{
				for (window_width=j-N/2;window_width<=j+N/2;window_width++)
				{
					window_height_new = window_height;
					window_width_new = window_width;

					if(window_height<0)
					{
						window_height_new=abs(window_height)-1;
					}
					if(window_width<0) 
					{
						window_width_new=abs(window_width)-1;
					}
					if (window_height>=height)
					{
						window_height_new=2*height-1-window_height ;
					}
					if (window_width>=width)
					{
						window_width_new=2*width-1-window_width;
					}
			
					pixel=pixelposition(output_gray, window_height_new, window_width_new, width, height, 1);
					sum_der_x += *( pixel) * I_derX[window_height- i + (N/2)][window_width - j + (N/2)];
					sum_der_y += *( pixel) * I_derY[window_height- i + (N/2)][window_width - j + (N/2)];
				}
			}	
			first_der= sqrt(pow(sum_der_x,2) + pow(sum_der_y,2));
			if ((first_der > max_gradient) && max_gradient<=255){
				max_gradient = first_der;
			}

			if (first_der>=255){	
					*(output_edges + (width*i+j)) = 255;
				} else if (first_der<0){
					*(output_edges + (width*i+j)) = 0;
				} else{
					*(output_edges + (width*i+j)) = first_der;
				}
		}
	} 
	fp2= fopen("SobelDetector_noisyImage_norm.raw","w");
	fwrite(norm_output,1,width*height,fp2);
	 fclose(fp2);

	//---------------------------------------------Threshold-----------------------------------------------------//
	float per_threshold=15/100.0;
	float threshold= (per_threshold) * (float) max_gradient;
	
	for (x=0;x<width*height;x++)
	{
		if (*(output_edges+x)>threshold){
			*(output_edges+ x)=0;
		} else{
			*(output_edges + x)=255;
		}
	}




	fp1= fopen("SobelDetector_noisy_edges.raw","w");
	fwrite(output_edges,1,width*height,fp1);
	 fclose(fp1);

FREE_MEM:
	free(inputimage);
	free(output_edges);
	free(output_gray);
	free(norm_output);
	inputimage = NULL;
	output_edges	= NULL;
	output_gray= NULL;
	norm_output 	= NULL;

	return 0;
}