/*Code written by:
#Name: 				Shreya Gupta

code: texture segmentation
*/


#include <math.h>
#include <stdlib.h> 
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#define NUM_FEATURES	9
#define NUM_DATA_PTS	270000
#define NUM_CENTROIDS   6
#define ITERATIONS_MAX	300


float filter[NUM_FEATURES][3][3] =		{ //E3*E3 6
									     
						{0.027778, 0.055556, 0.027778,
						0.055556, 0.111111, 0.055556,
						0.027778, 0.055556, 0.027778},
													 

// float L3E3[3][3] = 	
						{-0.083333, -0.166667, -0.083333,
						0.000000, 0.000000, 0.000000,
						0.083333, 0.166667, 0.083333},
					

// float L3S3[3][3] = 	{
						{-0.083333,	 -0.166667, -0.083333,
						0.166667, 	0.333333, 0.166667,
						-0.083333,	 -0.166667, -0.083333},
															

// float E3L3[3][3] = 	{
						{-0.083333, 0.000000, 0.083333,
						-0.166667, 0.000000, 0.166667,
						-0.083333, 0.000000, 0.083333},
													 

// float E3E3[3][3] = 	
						{0.250000, 0.000000, -0.250000,
						0.000000, 0.000000, 0.000000,
						-0.250000, 0.000000, 0.250000},
					

// float E3S3[3][3] = 	{
						{0.250000, 0.000000, -0.250000,
						-0.500000, 0.000000, 0.500000,
						0.250000, 0.000000, -0.250000},
					

// float S3L3[3][3] = 	
						{-0.083333, 0.166667, -0.083333,
						-0.166667, 0.333333, -0.166667,
						-0.083333, 0.166667, -0.083333},
												 

// float S3E3[3][3] = 	{
						{0.250000, -0.500000, 0.250000,
						0.000000, 0.000000, 0.000000,
						-0.250000, 0.500000, -0.250000},
					

// float S3S3[3][3] = 	{
						{0.250000, -0.500000, 0.250000,
						-0.500000, 1.000000, -0.500000,
						0.250000, -0.500000, 0.250000}
					};


float centroid_initialization[6][9]={{84.03 ,	 64.18 	, 27.58 	, 138.90 ,	 167.67, 	 94.36 ,  88.08 ,	 180.60,    190.40	},
									{71.11 ,	 77.43 	, 47.40 	, 44.91 ,	 152.16, 	 205.31,  16.59 ,	 83.19 ,   224.01 	},
									{117.07, 	 62.60,	  51.05, 	 146.51, 	 118.27, 	 94.91,   121.74, 	 139.96,   149.07 	},
									{99.46 ,	 60.20 	, 21.17 	, 29.05 ,	 56.53 ,	47.18 ,	  6.02 	,  	20.35 ,     32.08 	},
									{233.79,  	 107.08,  36.48, 	 103.40 ,	 45.33 ,	20.61 ,	  58.44 ,	39.16 ,	     18.46	},
									{158.50, 	 107.49,  27.50, 	 57.86 	 ,	101.78 ,	55.50 ,	  9.50 	,  	30.33 ,     39.08 	}};

float* pixelposition(float *image, int i,int j,int  width, int height, int bpp)
{	
	return (image + bpp*(width*i+j)); //j for horizontal and i for vertical
}

uint8_t* pixelposition1(uint8_t *image, int i,int j,int  width, int height, int bpp)
{	
	return (image + bpp*(width*i+j)); //j for horizontal and i for vertical
}

float get_image_mean(uint8_t *image,float *Zero_Mean_Img, int width, int height, int bpp)
{
	float mean1=0.0;
	int N1=24;
	float sum_pix=0;
	int t=0;
	uint8_t *pixel=NULL;
	int temp1 = 0;
	float max_pix=0;
	float min_pix=100;


	for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{			
				for (int k=0;k<bpp;k++)
				{
				 	sum_pix=0;	

			
				 	temp1 = 0;
						
					for( int window_height=i-N1/2;window_height<=i+N1/2;window_height++)
					{
						for (int window_width=j-N1/2;window_width<=j+N1/2;window_width++)
						{
							int window_height_new=window_height;
							int window_width_new=window_width;

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

						

							
							pixel=pixelposition1(image, window_height_new, window_width_new, width, height, 1);
							sum_pix+= *pixel;
						}
					}

				mean1 = sum_pix/(float)(N1*N1);

				*(Zero_Mean_Img + bpp*(width*i+j)+k)= (float ) (*(image+bpp*(width*i+j)+k)-mean1) ;
				
				if (*(Zero_Mean_Img + bpp*(width*i+j)+k)>max_pix){
					max_pix=*(Zero_Mean_Img + bpp*(width*i+j)+k);
				}
				if (*(Zero_Mean_Img + bpp*(width*i+j)+k)<min_pix){
					min_pix=*(Zero_Mean_Img + bpp*(width*i+j)+k);
				}
				
			}
			
		}
	}

}







void get_features(float* img, float* buffer,float **features, int width, int height, int bpp)
{
	int N=3;
	int N1=45;
	double* temp=NULL;
	int t=0;
	float sum_pix=0;
	float avg_energy=0;
	float *pixel=NULL;
	float *pixel1=NULL;
	int temp1 = 0;
	float sum_window=0;
	float max_sum[3]={0};

	while (t<9){ 
		avg_energy=0;
		
		/*	------------------------------------------------------------	
			APPLYING LAWS FILTER 			
		-----------------------------------------------------------------*/
		

		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{			
				for (int k=0;k<bpp;k++)
				{
				 	sum_pix=0;	

				 	temp1 = 0;
						
					for( int window_height=i-N/2;window_height<=i+N/2;window_height++)
					{
						for (int window_width=j-N/2;window_width<=j+N/2;window_width++)
						{
							int window_height_new=window_height;
							int window_width_new=window_width;

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
							pixel=pixelposition(img, window_height_new, window_width_new, width, height, 1);
							sum_pix+= *pixel * filter[t][window_height-i+(N/2)][window_width-j+(N/2)];
						}
					}
					 *(buffer+bpp*(width*i+j))= sum_pix;			 
				}
			}
		}


		/*------------------------------------------------------------------------------
				FINDING FEATURES USING LOCAL AVERAGE ENERGY
		---------------------------------------------------------------------------------*/
		

		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{			
				for (int k=0;k<bpp;k++)
				{
				 	sum_window=0;	

			
				 	temp1 = 0;
				 	avg_energy=0;
						
					for( int window_height1=i-N1/2;window_height1<=i+N1/2;window_height1++)
					{
						for (int window_width1=j-N1/2;window_width1<=j+N1/2;window_width1++)
						{
							int window_height1_new=window_height1;
							int window_width1_new=window_width1;

							if(window_height1<0)
							{
								window_height1_new=abs(window_height1)-1;
							}
							if(window_width1<0) 
							{
								window_width1_new=abs(window_width1)-1;
							}

							if (window_height1>=height)
							{
								window_height1_new=2*height-1-window_height1 ;
							}
							if (window_width1>=width)
							{
								window_width1_new=2*width-1-window_width1;
							}
							pixel1 = pixelposition(buffer, window_height1_new, window_width1_new, width, height, 1);
							avg_energy += pow(*pixel1,2);

						}
					}
					features[bpp*(width*i+j)+k][t]=(avg_energy/(float)(N1*N1));
				}
			}
		}
		t++;
	}


}




float calc_centroid_dist(float centroids[NUM_FEATURES], float *features)
{
	int i=0;
	float sum=0.0;


	for (i=0;i<NUM_FEATURES;i++)
	{
		sum+= pow(abs(centroids[i]-features[i]),2);
	}

	return sqrt(sum);

}

int min_dist_index(float *distance)
{
	int i=0;
	float minimum=1000.0;
	int pos_min=0;
	for (i=0;i<NUM_CENTROIDS;i++)
	{
		if (minimum>distance[i])
		{
			minimum=distance[i];
			pos_min=i;
		}
	}
	return pos_min;
}


void centroids_updated(float **features, float centroids[NUM_CENTROIDS][NUM_FEATURES], int *pred_class)
{
	int 	i=0;
	int 	j=0;
	int 	k=0;
	float 	*sum=NULL;
	int 	matching_pt=0;

	sum=(float*)malloc(NUM_FEATURES*sizeof(float));
	if (sum==NULL){
		printf("\n Error detected! %d\n", __LINE__);
		exit;
	}

	for (i=0;i<NUM_CENTROIDS;i++)
	{


		memset(sum, 0x00, NUM_FEATURES * sizeof(float));
		matching_pt=0;
		for (j=0;j<NUM_DATA_PTS;j++)
		{
			if (pred_class[j]==i)
			{
				for (k=0;k<NUM_FEATURES;k++)
				{
					sum[k]+=features[j][k];
				}
				matching_pt++;
			}
		}

		for (k=0;k<NUM_FEATURES;k++)
		{
			centroids[i][k]= sum[k]/(float) matching_pt;
		}
	}

	free(sum);
}



void clustering_k_means(float **features, int *pred_class, float centroids[NUM_CENTROIDS][NUM_FEATURES])
{

	int i=0;
	int j=0;
	int k=0;

	float *centroid_distance=(float*)malloc(NUM_CENTROIDS*sizeof(float));
	int *previous_class_prediction=(int*)malloc(NUM_DATA_PTS*sizeof(int));
	

	for (i=0; i<NUM_CENTROIDS;i++)
	{
		memcpy(centroids[i],centroid_initialization[i],NUM_FEATURES*sizeof(float));
	}
	

	for (i=0;i<ITERATIONS_MAX;i++){
		for (j=0;j<NUM_DATA_PTS;j++){
			for (k=0;k<NUM_CENTROIDS;k++){
				centroid_distance[k]=calc_centroid_dist(centroids[k],features[j]);	// num_centroids x num_features (4x9) matrix
			}
			pred_class[j]=min_dist_index(centroid_distance);
		}
		if (memcmp(pred_class,previous_class_prediction,NUM_DATA_PTS*sizeof(int)) == 0){
			printf(" Maximum iterations required : %d ",i); 
			break;	
		}
		(memcpy(previous_class_prediction, pred_class, NUM_DATA_PTS*sizeof(int)));
		centroids_updated(features, centroids, pred_class);
	}

	free(centroid_distance);
	free(previous_class_prediction);
}

void segment_textures(int argc, char *argv[])
{

	//read each image
	FILE *fp1;
	
	int i=0;
	int j=0;
	int k=0;
	int height=atoi(argv[3]);
	int width=atoi(argv[2]);
	int bpp=atoi(argv[4]);

	uint8_t *input=(uint8_t*)malloc(width*height*bpp*sizeof(uint8_t));	
	uint8_t *output=(uint8_t*)malloc(width*height*bpp*sizeof(uint8_t));
	float *Zero_Mean_Img=(float*)malloc((width*height*bpp)*sizeof(float));
	float *buffer=(float*)malloc(width*height*bpp*sizeof(float));
	int *pred_class=(int*)malloc(NUM_DATA_PTS*sizeof(int));

	float centroids[NUM_CENTROIDS][NUM_FEATURES]={0};
	int clusters_str[6]={0, 51, 102, 153, 204, 255};



	if(pred_class == NULL) {
		printf("%d\n", __LINE__);
		fflush(stdout);
	}
	float **features=(float**)malloc(NUM_DATA_PTS*sizeof(float *));
	if(features == NULL) {
		printf("%d\n", __LINE__);
		fflush(stdout);
	}
	for (i=0;i<NUM_DATA_PTS;i++){
		features[i]=(float*)malloc(NUM_FEATURES*sizeof(float));
		if(features[i] == NULL) {
			printf("%d\n", __LINE__);
			fflush(stdout);
		}
	}
	// float centroids[NUM_CENTROIDS][NUM_FEATURES]={0};

	fp1=fopen(argv[1],"r");
	fread(input,1, width*height*bpp, fp1);
	fclose(fp1);

	//apply 25x25 patches on image and calculate mean and return a zero mean image
	get_image_mean(input,Zero_Mean_Img, width, height, bpp);


	//apply 3x3 laws filters and find average energy
	get_features(Zero_Mean_Img,buffer,features,width, height, bpp);

	for (i=0;i<height;i++){
		for (k=0;k<width;k++){
			printf("[%d , %d] data point %d: ",i,k, (width*i+k));
			for (j=0;j<NUM_FEATURES;j++){
				printf(" %.2f \t",features[width*i + k][j]);
			}
			printf("\n");
		}
		
	}


	// apply k means

	clustering_k_means(features,pred_class,centroids);
	for (i=0;i<height;i++){
		for (k=0;k<width;k++){
			if (*(pred_class+width*i+k)==0) *(output+width*i+k)=(uint8_t) clusters_str[0];
			if (*(pred_class+width*i+k)==1) *(output+width*i+k)=(uint8_t) clusters_str[1];
			if (*(pred_class+width*i+k)==2) *(output+width*i+k)=(uint8_t) clusters_str[2];
			if (*(pred_class+width*i+k)==3) *(output+width*i+k)=(uint8_t) clusters_str[3];
			if (*(pred_class+width*i+k)==4) *(output+width*i+k)=(uint8_t) clusters_str[4];
			if (*(pred_class+width*i+k)==5) *(output+width*i+k)=(uint8_t) clusters_str[5];

		}
	}

	fp1=fopen("segmented_texture.raw", "w");
	fwrite(output,1,width*height*bpp,fp1);
	fclose(fp1);
	

	free(pred_class);
	free(input);
	free(Zero_Mean_Img);
	free(buffer);
	free(output);
	free(features);
	
}
int main(int argc, char* argv[])
{

	segment_textures(argc, argv);
	return 0;
}