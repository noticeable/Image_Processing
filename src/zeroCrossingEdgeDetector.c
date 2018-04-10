/*
#Code written by:
#Name: 				Shreya Gupta
ZERO CROSSING EDGE DETECTOR
*/

#include "zeroCrossingEdgeDetector.h"

int N=3;//window size

/* Function: pixewindow_widthposition
	input: input image, pixewindow_width at (i,j), width, height, bytes per pixewindow_width
	output_gray:  pointer at the window location in the 1D array that is pointing to the (i,j)th pixewindow_width vawindow_widthue
*/
uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontawindow_width and i for verticawindow_width 
}
int* pixelposition1(int *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontawindow_width and i for verticawindow_width 
}



int main(int argc, char* argv[])
{
	FILE *file_ptr = NULL;
	int N_gaussian=3;			
	int width 	= 0;
	int height 	= 0;
	int bpp		= 0;
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
	float I_gaussian[3][3]={{1/16 , 2/16, 1/16},{2/16, 4/16, 2/16},{1/16 , 2/16, 1/16}};
	float I_laplace[5][5]={{0,0, 1/16.0, 0, 0}, 							
							{0,1/16.0, 2/16.0, 1/16.0, 0}, 					
							{1/16.0, 2/16.0, -16/16.0, 2/16.0, 1/16.0}, 	
							{0,0, 1/16.0, 0, 0},							
							{0,1/16.0, 2/16.0, 1/16.0, 0}};
	
	float first_der=0;
	uint8_t *pixel= NULL;
	int *pixel1;
	// int I_gauss;
	int x=0;
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	float y1 = 0;
	float max_gradient=0;
	float min_laplace=300;
	float max_laplace=-100;
	int center=0;
	int left=0;
	int right=0;
	int up=0;
	int down=0;


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
	uint8_t *buffer3 		=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(buffer3 == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	file_ptr=fopen(argv[1],"rb");
	if (file_ptr==NULL)
	{
		printf("Error %s\n", strerror(errno)); 
		exit(0);
	}
	fread(inputimage,1,width*height*bpp, file_ptr);
	fclose(file_ptr);

//-------------------RGB to grayscale-------------------------------------------------------------------------------//
	for (x=0;x<width*height;x++)
	{
		pixel = (inputimage+bpp*x); 
		
		r= *(pixel+0);
		g= *(pixel+1);
		b= *(pixel+2);
		y1=0.21*r+0.72*g+0.07*b;	// luminosity method
		
		
		*(output_gray+x)= (uint8_t) y1;	
	}


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
					*(output_gray+ width*i+j) += *( pixel)   * I_gaussian [window_height- i + 1][window_width - j + 1];

				}
			}	
			
		}
	}





//---------------------------------------------laplacian filter-----------------------------------------------------------//
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{			
			sum_der_x=0;			
			for( window_height=i-N/2;window_height<=i+N/2;window_height++)
			{
				for (window_width=j-N/2;window_width<=j+N/2;window_width++)
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
					sum_der_x +=(float) *( pixel)  * I_laplace[ window_height -i + N/2][window_width - j + N/2];
				}
			}
			if ((sum_der_x >= max_laplace)){
				max_laplace = sum_der_x;
			}
			if ((sum_der_x <= min_laplace)){
				min_laplace = sum_der_x;
			}
			*(output_edges+ width*i+j)= sum_der_x;
		}
	}
	




/*-------------------------------------	Normalization---------------------------------------------	*/
	
	float threshold= 0.1*max_laplace; 
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{	
			if (*(output_edges+ width*i+j)>threshold){
				*(norm_output+ width*i+j)=1;
			} else if (*(output_edges+ width*i+j)<-threshold){
				*(norm_output+ width*i+j)=-1;
			}	else {
				*(norm_output+ width*i+j)=0;
			}
		}
	}

//-------------------------------------------------------------Zero crossing detection-------------------------------//


	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{	
			
			center=*(norm_output+width*i+j);
			if((width*i+j+1)<width)	{		
				right=*(norm_output+width*i+j+1);}	else {
				right=0;	
			}

			if ((width*i+j-1)>=0)	{
				left=*(norm_output+width*i+j-1); }	else {
				left=0;
			}

			if ((width*i+j-width)>=00){
				up=*(norm_output+width*i+j-width); }else {
				up=0;
			}

			if ((width*i+j+width)<height) {	
				down=*(norm_output+width*i+j+width);} else {
				down=0;	
			}

			if ((center>0) && (right>0) && (left>0) && (up>0) && (down>0))
			{
				*(buffer3+width*i+j)=255;
			} else if ((center<0) && (right<0) && (left<0) && (up<0) && (down<0)) {
				*(buffer3+width*i+j)=255;
			} else{
				*(buffer3+width*i+j)=0;
			}
			
		}
	}

//-------------------------------------------------grayscale before zero thresholding-------------//
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{	
			
			if (*(norm_output+ width*i+j)==1){
				*(norm_output+ width*i+j)=192;
			} else if (*(norm_output+ width*i+j)==-1){
				*(norm_output+ width*i+j)=64;
			}	else {
				*(norm_output+ width*i+j)=128;
			}
		}
	}

	file_ptr= fopen("zerocrossing.raw","w");
	fwrite(buffer3,1,width*height,file_ptr);
	 fclose(file_ptr);
	
	file_ptr= fopen("zerocrossing_norm.raw","w");
	fwrite(norm_output,1,width*height,file_ptr);
	 fclose(file_ptr);

FREE_MEM:
	free(inputimage);
	free(output_edges);
	free(output_gray);
	free(norm_output);
	inputimage = NULL;
	output_edges = NULL;
	output_gray = NULL;
	norm_output = NULL;
	return 0;
}