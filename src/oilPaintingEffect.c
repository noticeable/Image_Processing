


#include "oilPaintingEffect.h"

/*
	PROGRAM DESCRIPTION
	Performs oil painting effect by graylevel quantization
*/



/* Function: pixelposition
	input: input image, pixel at (i,j), width, height, bytes per pixel
	output:  pointer at the location in the 1D array that is pointing to the (i,j)th pixel value
*/
uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontal and i for vertical 
}


/* Function: maximum_array
	input: input array, array size
	output:  position of the maximum value in the array
*/
int maximum_array(int a[ ], int arraysize)
{
	int max=0, maximum_pos=0;
	int x;
	for (x=0;x<arraysize;x++)
	{
		if(max<a[x])
			{
				max=a[x];
				maximum_pos=x;
			}
	}
	return maximum_pos;
}


int main(int argc, char* argv[])
{
	FILE *file_ptr = NULL;
	uint8_t *inputimage = NULL;
	uint8_t *quantized = NULL;
	uint8_t *median_of_quantized = NULL;
	int height = 0;
	int width = 0; 
	int bpp = 0;
	int size = 0;
	int N = 0;
	int  k_new = 0;
	int l_new = 0;
	int quant = 0;
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0; 

	int x = 0;
	int temp = 0;
	int pixel_pos = 0;
	uint8_t *pixelr = NULL;
	uint8_t *pixelg = NULL;
	uint8_t *pixelb = NULL;
	int countr[4]={};
	int countg[4]={};
	int countb[4]={};
	int countr1[8]={};
	int countg1[8]={};
	int countb1[8]={};
	int depth = 0;
	int bin_size = 0;
	
	
	if(argc < 7)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp>\n", argv[0]);
		exit(1);
	}
	width=atoi(argv[2]);
	height=atoi(argv[3]);
	bpp=atoi(argv[4]);
	N= atoi(argv[5]);
	quant=atoi(argv[6]);
	if(quant == 256) {
	    printf("Unsuppoted quantization level\n");
	    exit(1);
	}

	inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	
	quantized=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(quantized == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	median_of_quantized=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(median_of_quantized == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	file_ptr = fopen(argv[1],"r");
	if (file_ptr==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno));  
		exit(0);
	}
	fread(inputimage,1,width*height*bpp, file_ptr );
	fclose(file_ptr);
	
	depth    = (int) cbrt(quant);
	bin_size = 256 / depth;
		
	
		/* For 64 bit quantization */
	for (x=0;x<width*height*bpp;x++)
	{
		//for reading 3 bytes of r,g,b values for each pixel
		
		r=*(inputimage+x);
	
		temp = ((r / bin_size)*bin_size) + ((bin_size / 2) - 1);
		
		*(quantized+x)= temp ;

	}

	/*
		Assigning median value of the NxN window 
		to the center pixel
	*/
	
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{			
			for(int w=0;w<4;w++)
			{
				countr[w]=0;
				countg[w]=0;
				countb[w]=0;
			
			}
			for(int w=0;w<8;w++)
			{
				
				countr1[w]=0;
				countg1[w]=0;
				countb1[w]=0;
			}

			for(int k=i-N/2;k<=i+N/2;k++)
			{
				for (int l=j-N/2;l<=j+N/2;l++)
				{
					k_new=k;
					l_new=l;

					if(k<0)
					{
						k_new=abs(k)-1;
					}
					if(l<0) 
					{
						l_new=abs(l)-1;
					}
					
					pixelr=pixelposition(quantized, k_new, l_new, width, height, bpp);
					if (quant==64)
						{
							countr[*pixelr/64]++;
							countg[*(pixelr+1)/64]++;
							countb[*(pixelr+2)/64]++;
						}
					else if (quant==512)
						{
							countr1[*pixelr/32]++;
							countg1[*(pixelr+1)/32]++;
							countb1[*(pixelr+2)/32]++;
						}
				}
			}
			
				    			    

			if (quant==64)

			{
				pixel_pos= maximum_array(countr,4);
				*(median_of_quantized+ bpp*(j+i*width))= (pixel_pos+1)*64 - 33 ;
				pixel_pos= maximum_array(countg,4);
				*(median_of_quantized+ bpp*(j+i*width)+1)= (pixel_pos+1)*64 - 33;
				pixel_pos= maximum_array(countb,4);
				
				*(median_of_quantized+ bpp*(j+i*width)+2)= (pixel_pos+1)*64 - 33;
				
			}
			else if(quant==512)
			{
				pixel_pos= maximum_array(countr1,8);
				*(median_of_quantized+ bpp*(j+i*width))= (pixel_pos+1)*32 - 17 ;
				
				pixel_pos= maximum_array(countg1,8);
				*(median_of_quantized+ bpp*(j+i*width)+1)= (pixel_pos+1)*32 - 17;
				
				pixel_pos= maximum_array(countb1,8);
				*(median_of_quantized+ bpp*(j+i*width)+2)= (pixel_pos+1)*32 - 17;
				
			}

		}
	}




		file_ptr= fopen("oilPainting.raw","w");
		fwrite(median_of_quantized,1,width*height*bpp,file_ptr);
		fclose(file_ptr);
	

FREE_MEM: 
	free(inputimage);
	free(quantized);
	free(median_of_quantized);
	inputimage = NULL;
	quantized = NULL;
	median_of_quantized = NULL;
	return 0;	
}

