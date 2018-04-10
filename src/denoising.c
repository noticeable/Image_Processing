
#include "denoising.h"

/*
	PROGRAM DESCRIPTION
	Program to remove salt and pepper noise,
	followed by white noise and PSNR calculation.
*/



/* Function: pixelposition
	input: input image, pixel at (i,j), width, height, bytes per pixel
	output:  pointer at the location in the 1D array that is pointing to the (i,j)th pixel value
*/

uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); 
}

/* Function: compare
	Description: compare function for the qsort command in the main function
*/
int compare( const void * first, const void * second)
{
	return(*(uint8_t*)first - *(uint8_t*)second);
}

int main(int argc, char* argv[])
{
	FILE *f_ptr = NULL;

	//for all inputs and results after filtering
	uint8_t *inputimage = NULL;
	uint8_t *output1 = NULL;
	uint8_t *median_filtered = NULL;
	uint8_t *output3 = NULL;
	uint8_t *outputimage= NULL;
	uint8_t *psnrimage = NULL;
	

	//for windows and scanning images
	int i=0;
	int j=0;
	int k=0;
	int l=0; 
	int k_new=0;
	int l_new=0;
	int w=0;// N is NxN window size
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	int x=0;
	int temp = 0;

	// for applying median and gaussian filters in NxN window sizes.
	int pixel_pos=0;
	int sumr = 0;
	int sumg = 0;
	int sumb = 0;
	uint8_t *pixel=NULL;
	uint8_t *pixelr=NULL;
	int countr[4]={};
	int countg[4]={};
	int countb[4]={};
	int p,q = 0;

	int width; 	
	int height; 	
	int bpp; 	
	int N; 		
	uint8_t psnr; 

	if(argc < 7)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp>\n", argv[0]);
		exit(1);
	}

	width	=atoi(argv[2]);
	height	=atoi(argv[3]);
	bpp	=atoi(argv[4]);
	N 	= atoi(argv[5]);
	psnr=atoi(argv[6]); // original image to be compared with


	uint8_t medianr = 0;
	uint8_t mediang = 0;
	uint8_t medianb = 0;

	float MSEr = 0;
	float MSEg = 0;
	float MSEb = 0;
	uint8_t *pixely, *pixelx;
	float differencer = 0;
	float differenceg = 0;
	float differenceb = 0;
	float PSNR_r = 0;
	float PSNR_g = 0;
	float PSNR_b = 0;
	uint8_t *window_valuesr = NULL;
	uint8_t *window_valuesg = NULL;
	uint8_t *window_valuesb = NULL;

	if(argc < 7)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp> <Window size> <Image for PSNR comparison>\n", argv[0]);
		exit(1);
	}

	width	=atoi(argv[2]);
	height	=atoi(argv[3]);
	bpp	=atoi(argv[4]);
	N 	= atoi(argv[5]);
	psnr=atoi(argv[6]); // original image to be compared with	inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));

	inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	median_filtered=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(median_filtered == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	outputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(outputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	window_valuesr=(uint8_t*)malloc((N*N)*sizeof(uint8_t));
	if(window_valuesr == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	window_valuesg=(uint8_t*)malloc((N*N)*sizeof(uint8_t));
	if(window_valuesg == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	window_valuesb=(uint8_t*)malloc((N*N)*sizeof(uint8_t));
	if(window_valuesb == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	
	
	f_ptr=fopen(argv[1],"r");
	if (f_ptr==NULL)
	{
		printf("Error %s\n", strerror(errno)); 
		exit(0);
	}
	fread(inputimage,1,width*height*bpp, f_ptr);
	fclose(f_ptr);

	psnrimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	f_ptr=fopen(argv[6],"r");
	if (f_ptr==NULL)
	{
		printf("Error %s\n", strerror(errno)); 
		exit(0);
	}
	fread(psnrimage,1,width*height*bpp, f_ptr);
	fclose(f_ptr);
	


	/*
					 Median Filtering 
	*/
	for(int i=0;i<height;i++) 
	{
		for(int j=0;j<width;j++) 
		{			
			w=0;

			/* This part focuses on extracting values of each of the pixels in the NxN window 
			it also takes into account the edge pixels and how to 
			create window by replication of pixel values at the edges*/
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
					
					pixel=pixelposition(inputimage, k_new, l_new, width, height, bpp);		
					if(bpp==3)
					{	window_valuesr[w]=*pixel;
						window_valuesg[w]=*(pixel+1);
						window_valuesb[w]=*(pixel+2);
						w++;	
					} else if (bpp==1)	{
						window_valuesr[w]=*pixel;
						w++;
					}			
					
				}
			}
			// sorting the values of pixel intensities for finding median
			// and assigning this median to the pixel at which the window is applied.
				if (bpp==3)
			{		
				qsort(window_valuesr,N*N,sizeof(uint8_t), compare);
				 medianr=window_valuesr[((N*N)/2 )-1];
				qsort(window_valuesg,N*N,sizeof(uint8_t), compare);
				 mediang=window_valuesg[((N*N)/2 )-1];
				qsort(window_valuesb,N*N,sizeof(uint8_t), compare);
				 medianb=window_valuesb[((N*N)/2 )-1];

				*(median_filtered+ bpp*(j+i*width))= medianr ;
				*(median_filtered+ bpp*(j+i*width)+1)= mediang;
				*(median_filtered+ bpp*(j+i*width)+2)= medianb;
			} else if(bpp==1) {
				qsort(window_valuesr,N*N,sizeof(uint8_t), compare);
				 medianr=window_valuesr[((N*N)/2 )-1];
				 *(median_filtered+ bpp*(j+i*width))= medianr ;
			}
		}
	}


						/* Linear Filtering */
	/* This part aims at running a window function through each pixel of the image
		and calculating a mean for all the pixel intensities in the window and 
		assigning the mean to the pixel position*/

for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			sumr=0;
			sumg=0;
			sumb=0;
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

					if (k>=height)
					{
						k_new=2*height-1-k ;
					}		
					if (l>=width)
					{
						l_new=2*width-1-l;
					}
					
					pixelr=pixelposition(inputimage, k_new, l_new, width, height, bpp);
					
					
					sumr +=*(pixelr);
					if(bpp==3)
					{
					sumg +=*(pixelr+1);
					sumb +=*(pixelr+2);	
					}				
				}
			}

			
			*(outputimage+ bpp*(j+i*width))=(uint8_t)  (sumr/(N*N));
			if(bpp==3)
			{
				*(outputimage+ bpp*(j+i*width)+1)=(uint8_t)  (sumg/(N*N));
				*(outputimage+ bpp*(j+i*width)+2)= (uint8_t) (sumb/(N*N));
			}
			
		}
	}


		/* PSNR calculation */
	

	

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			pixely = pixelposition(outputimage, i, j, width, height, bpp);
			pixelx = pixelposition(psnrimage, i, j, width, height, bpp);
			

			differencer=pow((*(pixely+0) - *(pixelx+0)),2);
			MSEr+=(differencer/(width*height));
			if(bpp==3)
			{
				differenceg=pow((*(pixely+1) - *(pixelx+1)),2);
				MSEg+=(differenceg/(width*height));

				differenceb=pow((*(pixely+2) - *(pixelx+2)),2);
				MSEb+=(differenceb/(width*height));
			}
		}
	}
	
	printf("\n -------- the PSNR values for window size %d is: --------", N);
	PSNR_r=10*log10(255*255/MSEr);
	printf("\n PSNR for channel r: %f ", PSNR_r);
	
	if (bpp==3)
	{	
		PSNR_g=10*log10(255*255/MSEg);
		PSNR_b=10*log10(255*255/MSEb);
		printf("\n PSNR for channel g: %f ", PSNR_g);
		printf("\n PSNR for channel b: %f \n", PSNR_b);
	}


	
	f_ptr= fopen("output_Denoising.raw","w");
	if (f_ptr==NULL)
	{
		printf("Error %s\n", strerror(errno)); 
		exit(0);
	}
	fwrite(outputimage,1,width*height*bpp,f_ptr);
	fclose(f_ptr);

FREE_MEM:
	free(inputimage);
	inputimage = NULL;
	free(median_filtered);
	median_filtered = NULL;
	free(outputimage);
	outputimage = NULL;
	free(window_valuesr);
	window_valuesr = NULL;
	free(window_valuesg);
	window_valuesg = NULL;
	free(window_valuesb);
	window_valuesb = NULL;

	return 0;	
}
