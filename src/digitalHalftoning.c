/*
Code written by:
Name: 				Shreya Gupta
PURPOSE : 			Digital Half Toning
*/

#include "digitalHalftoning.h"
/*
	PROGRAM DESCRIPTION
	Performs Digital Halftoning for binary quantization
*/


/* 
Defining Bayer's index matrices 
*/

int I2[2][2]=  {{1,2},
				{3,0}};

int	I4[4][4]=  {{5,9,6,10},
				{13,1,14,2},
				{7,11,4,8},
				{15,3,12,0}};


int I8[8][8]=  {{21,37,25,41,22,38,26,42},
				{53,5,57,9,54,6,58,10},
				{29,45,17,33,30,46,18,34},
				{61,13,49,1,62,14,50,2},
				{23,39,27,43,20,36,24,40},
				{55,7,59,11,52,4,56,8},
				{31,47,19,35,28,44,16,32},
				{63,15,51,3,60,12,48,0}};




int main(int argc, char* argv[])
{
	FILE *file_ptr = NULL;
	uint8_t *inputimage = NULL;
	uint8_t *thresholded = NULL;
	uint8_t *dithering4x4 = NULL;
	uint8_t *dithering8x8 = NULL;
	int height = 0;
	int width = 0;
	int bpp = 0;
	int N = 0;
	int threshold = 0;
	int x = 0;
	int y = 0;
	int temp = 0;
	int i = 0;
	int j = 0;
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	int type_thresholding = 0;
	int T2[2][2]={};
	int T4[4][4]={}; 
	int T8[8][8]={};


	if(argc < 6)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp> <type thresholding \n 1. Fixed Thresholding \n 2. Random Thresholding \n 3. Bayer's dithering>\n", argv[0]);
		exit(1);
	}
	width = atoi(argv[2]);
	height = atoi(argv[3]);
	bpp = atoi(argv[4]);
	type_thresholding= atoi(argv[5]);

	
	

	inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	thresholded=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if( thresholded== NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	dithering4x4=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if( dithering4x4== NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	dithering8x8=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if( dithering8x8== NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}


	file_ptr=fopen(argv[1],"r");
	if (file_ptr==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno));  
		exit(0);
	}
	fread(inputimage,1,width*height*bpp, file_ptr);
	fclose(file_ptr);

/* 
	Setting Thresholds for the respective 
	digital halftoning techniques to be used. 
*/
	if (type_thresholding==1 )
	{	
		threshold=127; /*Fixed thresholding */
	}
	else if (type_thresholding==2)
	{
		struct timespec spec; 
 		clock_gettime(CLOCK_MONOTONIC,&spec);
		srand((spec.tv_nsec/1.0e6));
		threshold=rand()%256;
		printf("\n %d \n", threshold);
	}
	else if (type_thresholding==3)
	{
		for (i=0;i<2;i++)
		{
			for (j=0;j<2;j++)
			{
				T2[i][j]= 255*(I2[i][j]+0.5)/4;
			}
		}
		for (i=0;i<4;i++)
		{
			for (j=0;j<4;j++)
			{
				T4[i][j]= 255*(I4[i][j]+0.5)/16;
			}
		}
		for (i=0;i<8;i++)
		{
			for (j=0;j<8;j++)
			{
				T8[i][j]= 255*(I8[i][j]+0.5)/64;
			}
		}
	} 


	if (type_thresholding==1 || type_thresholding==2)
	{
		for (x=0;x<width*height*bpp;x++)
		{	
			
			r=*(inputimage+x);
			if (r>=0 && r<threshold)
			{
				temp=0;
			}
			else if (r>=threshold && r<=255)
			{
				temp=255;
			}
			*(thresholded+x)= temp ;
		}
	}
	else if (type_thresholding==3)
	{
		N=2;
		for (i=0;i<height; i++)
		{
			for(j=0;j<width;j++)
			{
				 if (*(inputimage+bpp*(i*width)+j)<T2[i%N][j%N] )
				 {
				 	*(thresholded+i*width+j) = 0;
				 }
				 else
				 	*(thresholded+i*width+j)=255;
				 
			}
		}

		N=4;
		for (i=0;i<height; i++)
		{
			for(j=0;j<width;j++)
			{
				 if (*(inputimage+bpp*(i*width)+j)<T4[i%N][j%N] )
				 {
				 	*(dithering4x4+i*width+j) = 0;
				 }
				 else
				 	*(dithering4x4+i*width+j)=255;
			}
		}

		N=8;
		for (i=0;i<height; i++)
		{
			for(j=0;j<width;j++)
			{
				 if (*(inputimage+bpp*(i*width)+j)<T8[i%N][j%N] )
				 {
				 	*(dithering8x8+i*width+j) = 0;
				 }
				 else
				 	*(dithering8x8+i*width+j)=255;
			}
		}
	}


/*	OUtputing results in '.raw' formats	*/

	if (type_thresholding==1)
	{
		file_ptr= fopen("DigitalHalftoning_fixedThres.raw","w");
		if (file_ptr==NULL)
		{
			printf("[%d] Error %s\n", __LINE__, strerror(errno));  
			exit(0);
		}
		fwrite(thresholded,1,width*height*bpp,file_ptr);
		fclose(file_ptr);
	}
	else if (type_thresholding==2)
	{
		file_ptr= fopen("DigitalHalftoning_randomThres.raw","w");
		if (file_ptr==NULL)
		{
			printf("[%d] Error %s\n", __LINE__, strerror(errno));  
			exit(0);
		}
		fwrite(thresholded,1,width*height*bpp,file_ptr);
		 fclose(file_ptr);
	}

	else if (type_thresholding==3)
	{
		file_ptr= fopen("bayers_dithering_2x2.raw","w");
		if (file_ptr==NULL)
		{
			printf("[%d] Error %s\n", __LINE__, strerror(errno));  
			exit(0);
		}
		fwrite(thresholded,1,width*height*bpp,file_ptr);
		fclose(file_ptr);
		file_ptr= fopen("bayers_dithering_4x4.raw","w");
		if (file_ptr==NULL)
		{
			printf("[%d] Error %s\n", __LINE__, strerror(errno));  
			exit(0);
		}
		fwrite(dithering4x4,1,width*height*bpp,file_ptr);
		fclose(file_ptr);
		file_ptr= fopen("bayers_dithering_8x8.raw","w");
		if (file_ptr==NULL)
		{
			printf("[%d] Error %s\n", __LINE__, strerror(errno));  
			exit(0);
		}
		fwrite(dithering8x8,1,width*height*bpp,file_ptr);
		fclose(file_ptr);
	}
	

FREE_MEM:

	free(inputimage);
	inputimage = NULL;
	free(thresholded);
	thresholded = NULL;
	free(dithering4x4);
	dithering4x4 = NULL;
	free(dithering8x8);
	dithering8x8 = NULL;

	return 0;
}