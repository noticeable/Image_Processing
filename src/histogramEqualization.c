

#include "histogramEqualization.h"
/*
	PROGRAM DESCRIPTION
	Performs histogram eqaliztion by tranfer function method. 
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
	FILE *f_ptr;	
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t y1 = 0;
	uint8_t y2 = 0;
	int width = 0;
	int height = 0;
	int bpp = 0;
	int x=0;
	int y=0;
	uint8_t *pixel;
	/*
	Variables used for plotting the 
	histogram and equalized histograms 
	*/

	// for calculating cumalative probability of occurence of each intensity
	float probcumr[256]={}; 
	float probcumg[256]={}; 
	float probcumb[256]={}; 
	int countr[256]={};
	int countg[256]={};
	int countb[256]={};
	int pixelnumber=0;
	int pixelvalue=0;
	int totalr = 0;
	int totalg = 0;
	int totalb = 0;
	float probr[256] = {};
	float probg[256] = {};
	float probb[256] = {};

	//for calculating the transfer function histogram 
	int enhancedprobr[256] = {};
	int enhancedprobg[256] = {};
	int enhancedprobb[256] = {};
	float pcr = 0;
	float pcg = 0;
	float pcb = 0;

	if(argc < 5)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp>\n", argv[0]);
		exit(1);
	}

	width=atoi(argv[2]);
	height=atoi(argv[3]);
	bpp=atoi(argv[4]);
	
	
	uint8_t *inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	uint8_t *r_channel=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(r_channel == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	uint8_t *g_channel=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(g_channel == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	uint8_t *b_channel=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(b_channel == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	uint8_t *outputr=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(outputr == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	uint8_t *outputg=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(outputg == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	uint8_t *outputb=(uint8_t*)malloc((width*height)*sizeof(uint8_t));
	if(outputb == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	
	uint8_t *outputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(outputimage == NULL) {
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
	

	for (x=0;x<width*height;x++)
	{
		/*
		for reading 3 bytes of 
		r,g,b values for each pixel
		*/
		pixel=inputimage+3*x; 

		r= *(pixel+0);
		g=*(pixel+1);
		b=*(pixel+2);
		
		*(r_channel+x)=r;
		*(g_channel+x)=g;
		*(b_channel+x)=b;
		
	}

	for(pixelnumber=0; pixelnumber<width*height;pixelnumber++)
	{
		for(pixelvalue=0; pixelvalue<256;pixelvalue++)
		{
			if (*(r_channel+pixelnumber)==pixelvalue)
			{
				countr[pixelvalue]++;
			}
			if (*(g_channel+pixelnumber)==pixelvalue)
			{
				countg[pixelvalue]++;
			}
			if (*(b_channel+pixelnumber)==pixelvalue)
			{
				countb[pixelvalue]++;
			}
		}

	}

	for(x=0;x<256;x++)
	{
		printf ("\n  %d %d %d ", countr[x],countg[x], countb[x]);
	}



	for(x=0;x<256;x++)
	{
		totalr=totalr+countr[x];
		totalg=totalg+countg[x];
		totalb=totalb+countb[x];
	}


	for (x=0;x<256;x++)
	{
		probr[x]=countr[x]/(float) totalr;
		probg[x]=countg[x]/(float) totalg;
		probb[x]=countb[x]/(float) totalb;
	}

	for(x=0;x<256;x++)
	{
		for(y=0;y<=x;y++)
		{
			probcumr[x]=probcumr[x]+probr[y];
			probcumg[x]=probcumg[x]+probg[y];
			probcumb[x]=probcumb[x]+probb[y];
		}
	}


	for(x=0;x<256;x++)
	{	
		enhancedprobr[x]=floor(255*(probcumr[x]));
		enhancedprobg[x]=floor(255*(probcumg[x]));
		enhancedprobb[x]=floor(255*(probcumb[x]));
	}
	for (x=0;x<width*height;x++)
	{
		for(y=0;y<256;y++)
		{
			if (*(r_channel+x)==y)
			{
				*(outputr+x)=enhancedprobr[y];
			}
			if (*(g_channel+x)==y)
			{
				*(outputg+x)=enhancedprobg[y];
			}
			if (*(b_channel+x)==y)
			{
				*(outputb+x)=enhancedprobb[y];
			}	
		}

		*(outputimage+bpp*x)=*(outputr+x);
		*(outputimage+bpp*x+1)=*(outputg+x);
		*(outputimage+bpp*x+2)=*(outputb+x);
		
	}

	f_ptr= fopen("outputhist.raw","w");
	fwrite(outputimage,1,width*height*bpp,f_ptr);
	fclose(f_ptr);

FREE_MEM:

	
	free(inputimage);
	inputimage = NULL;
	free(r_channel);
	r_channel = NULL;
	free(g_channel);
	g_channel = NULL;
	free(b_channel);
	b_channel = NULL;
	free(outputr);
	outputr = NULL;
	free(outputg);
	outputg = NULL;
	free(outputb);
	outputb = NULL;
	free(outputimage);
	outputimage = NULL;



	return 0;	
}
