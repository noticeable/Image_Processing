
#include "rgb2gray.h"

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
uint8_t max_of_3(uint8_t a, uint8_t b, uint8_t c)
{
	uint8_t max=a;
	if (b>a && b>c)
		max=b;
	else if(c>a && c>b)
		max=c;
	else
		max=a;

	return max;
}


/* Function: min_of_3
	input: r , g, b pixel intensities
	output:  minimum of the rgb values
*/
uint8_t min_of_3(uint8_t a, uint8_t b, uint8_t c)
{
	uint8_t min=a;
	if (b<a && b<c)
		min=b;
	else if(c<a && c<b)
		min=c;
	else
		min=a;

	return min;
}


int main(int argc, char* argv[])
{
	
	FILE    *file_ptr = NULL;
	FILE *displayimage1 = NULL;
	FILE *displayimage2 = NULL;
	FILE *displayimage3 = NULL;
	int width = 0;
	int height = 0;
	int bpp = 0; // bytes per pixel depending on rgb or monochrome
	int moveptr = 0;
	int x=0;
	uint8_t *output_luminosity = NULL;
	uint8_t *output_lightness = NULL;
	uint8_t *output_average = NULL;
	uint8_t *inputimage = NULL; 
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	int luminosity_eqn=0;
	int max_rgb=0;
	int min_rgb=0;
	int lightness_eqn=0;
	int average_eqn=0;

	if(argc < 5)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp>\n", argv[0]);
		exit(1);
	}
	
	width  = atoi(argv[2]); 
	height = atoi(argv[3]);
	bpp    = atoi(argv[4]);

	/* Allocate memory for input and output image buffer
	 based on bpp, width and height of image.
	 */

	inputimage 			= (uint8_t*) malloc((width*height*bpp)*sizeof(uint8_t));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	output_luminosity 	= (uint8_t*) malloc((width*height)*sizeof(uint8_t));
	if(output_luminosity == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	output_lightness 	= (uint8_t*) malloc((width*height)*sizeof(uint8_t));
	if(output_lightness == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	output_average 		= (uint8_t*) malloc((width*height)*sizeof(uint8_t));
	if(output_average == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	file_ptr=fopen(argv[1], "r");
	if (file_ptr==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno));  
		exit(0);
	}
	fread(inputimage,1,width*height*bpp,file_ptr);

	/*
		COLOR TO GRAYSCALE CONVERSION LOGIC
	*/
	for(x=1;x<=height*width;x++)
	{
		r = *(inputimage + x*bpp + 0);
		g = *(inputimage + x*bpp + 1);
		b = *(inputimage + x*bpp + 2);
		
		luminosity_eqn	=0.21*r+0.72*g+0.07*b;	
		max_rgb = max_of_3(r,g,b);
		min_rgb = min_of_3(r,g,b);
		lightness_eqn	= (max_rgb+min_rgb)/2;
		average_eqn		= (r+g+b) /3; 


		*(output_luminosity + x)	= luminosity_eqn;
		*(output_lightness + x)		= lightness_eqn;
		*(output_average + x)		= average_eqn;			
	}
	
	
	/*
			YIQ image
	*/ 

	//luminosity method
	displayimage1=fopen("output_luminosityMethod.raw", "w+");
	if (displayimage1==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno)); 
		exit(0);
	}
	fwrite(output_luminosity,1,width*height,displayimage1);


	// lightness method
	displayimage2=fopen("output_LightnessMethod.raw", "w+");
	if (displayimage2==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno)); 
		exit(0);
	}
	fwrite(output_lightness,1,width*height,displayimage2);

	//average method
	displayimage3=fopen("output_AverageMethod.raw", "w+");
	if (displayimage3==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno)); 
		exit(0);
	}
	fwrite(output_average,1,width*height,displayimage3);


FREE_MEM:

	free(inputimage);
	inputimage = NULL;
	free(output_luminosity);
	output_luminosity = NULL;
	free(output_lightness);
	output_lightness = NULL;
	free(output_average);
	output_average = NULL;

	return 0;
}

