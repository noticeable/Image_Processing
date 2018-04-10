
/*Code written by:
#Name: 				Shreya Gupta
Texture Classification

*/

#include "textureClassification.h"

#define 	NUM_DATA_PTS			12
#define 	NUM_FEATURES			9
#define		NUM_CENTROIDS			4
#define 	ITERATIONS_MAX			300

 float filter[NUM_FEATURES][5][5] =			{ //E5*E5 6
									     {1/36.0  ,  2/36.0  ,  0  , -2/36.0  , -1/36.0,
										  2/36.0  ,  4/36.0  ,  0  , -4/36.0  , -2/36.0,
										  0 /36.0 ,  0/36.0  ,  0  ,  0/36.0  ,  0/36.0,
										 -2 /36.0 , -4/36.0  ,  0  ,  4/36.0  ,  2/36.0,
										 -1 /36.0 , -2/36.0  ,  0  ,  2/36.0  ,  1/36.0 },
										 
										 
										 //s5*s5,-4
						
										 { 1/16.0  ,  0  , -2/16.0  ,  0  ,  1/16.0,
										  0/16.0  ,  0  ,  0/16.0  ,  0  ,  0/16.0,
										 -2/16.0  ,  0  ,  4/16.0  ,  0  , -2/16.0,
										  0/16.0  ,  0  ,  0/16.0  ,  0  ,  0/16.0,
										  1/16.0  ,  0  , -2/16.0  ,  0  ,  1/16.0},
										 
										 
										 //w5*w5,-6
										 
										{  1/36.0  , -2/36.0  ,  0  ,  2/36.0  , -1/36.0,
										 -2/36.0  ,  4 /36.0 ,  0  , -4 /36.0 ,  2/36.0,
										  0/36.0  ,  0 /36.0 ,  0  ,  0 /36.0 ,  0/36.0,
										  2/36.0  , -4/36.0  ,  0  ,  4 /36.0 , -2/36.0,
										 -1/36.0  ,  2/36.0  ,  0  , -2 /36.0 ,  1/36.0},
										 
										 
										 //5*S5,

										 { 1/36.0  ,  0  , -2/36.0  ,  0  ,  1/36.0,
										  2/36.0  ,  0  , -4/36.0  ,  0  ,  2/36.0,
										  0 /36.0 ,  0  ,  0/36.0  ,  0  ,  0/36.0,
										 -2 /36.0 ,  0  ,  4/36.0  ,  0  , -2/36.0,
										 -1 /36.0 ,  0  ,  2/36.0  ,  0  , -1/36.0},
										 
										 
										 //E5*W5,

										 { 1/36.0  , -2/36.0  ,  0  ,  2/36.0  , -1/36.0,
										  2 /36.0 , -4/36.0  ,  0  ,  4 /36.0 , -2/36.0,
										  0 /36.0 ,  0/36.0  ,  0  ,  0 /36.0 ,  0/36.0,
										 -2 /36.0 ,  4/36.0  ,  0  , -4/36.0  ,  2/36.0,
										 -1 /36.0 ,  2/36.0  ,  0  , -2/36.0  ,  1/36.0},
										 
										 
										 //S5*E5,

										 { 1/24.0  ,  2/24.0  ,  0  , -2/24.0  , -1/24.0,
										  0/24.0  ,  0/24.0  ,  0  ,  0/24.0  ,  0/24.0,
										 -2 /24.0 , -4/24.0  ,  0  ,  4/24.0  ,  2/24.0,
										  0 /24.0 ,  0/24.0  ,  0  ,  0/24.0  ,  0/24.0,
										  1/24.0  ,  2/24.0  ,  0  , -2/24.0  , -1/24.0},
										 
										 
										   //S5*W5

										 { 1/24.0  , -2 /24.0 ,  0  ,  2 /24.0 , -1/24.0,
										  0 /24.0 ,  0 /24.0 ,  0  ,  0 /24.0 ,  0/24.0,
										 -2 /24.0 ,  4 /24.0 ,  0  , -4 /24.0 ,  2/24.0,
										  0 /24.0 ,  0 /24.0 ,  0  ,  0 /24.0 ,  0/24.0,
										  1 /24.0 , -2 /24.0 ,  0  ,  2 /24.0 , -1/24.0},
										 
										 //W5*E5,

										 { 1/36.0  ,  2/36.0  ,  0  , -2/36.0  , -1/36.0,
										 -2/36.0  , -4/36.0  ,  0  ,  4/36.0  ,  2/36.0,
										  0/36.0  ,  0/36.0  ,  0  ,  0/36.0  ,  0/36.0,
										  2/36.0  ,  4/36.0  ,  0  , -4/36.0  , -2/36.0,
										 -1/36.0  , -2/36.0  ,  0  ,  2/36.0  ,  1/36.0},
										 
										// W5*S5,
														 
										 { 1/24.0  ,  0  , -2/24.0  ,  0  ,  1/24.0,
										  -2/24.0  ,  0  ,  4 /24.0 ,  0  , -2/24.0,
										   0/24.0  ,  0  ,  0 /24.0 ,  0  ,  0/24.0,
										   2/24.0  ,  0  , -4/24.0  ,  0  ,  2/24.0,
										  -1/24.0  ,  0  ,  2/24.0 ,  0  , -1/24.0}
										};

float centroid_initialization[4][9]={{	342.81, 	117.51, 	15.20, 	82.26, 	60.55, 	183.44, 	43.22, 	53.25, 	38.16 },
									{	45.58, 		16.71, 		1.85, 	13.47 ,	9.19 ,	22.83 ,		5.76,	5.85, 	4.66},
									{	24.74, 		5.57 ,		0.54 ,	4.58 ,	2.80 ,	12.68 ,		1.61,	3.99, 	1.81 },
									{	2.41 ,		2.11 ,		0.65 ,	0.95, 	1.10 ,	2.18 ,		1.14,	1.12, 	1.15 }};



int* pixelposition(int *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontal and i for vertical
}

float get_image_mean(uint8_t *image,int *Zero_Mean_Img, int width, int height, int bpp)
{
	float mean1=0.0;
	int i=0;
	int j=0;
	int k=0;

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			for (k=0;k<bpp;k++)
			{
				mean1+= *(image+bpp*(width*i+j)+k);
			}
			
		}
	}

	mean1 = mean1/(float)(width*height);
	
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			for (k=0;k<bpp;k++)
			{
				*(Zero_Mean_Img + bpp*(width*i+j)+k)=*(image+bpp*(width*i+j)+k)-mean1 ;
			}
			
		}
	}
}

void get_features(int* img, uint8_t* buffer,float features[NUM_DATA_PTS][NUM_FEATURES],int image_num, int width, int height, int bpp)
{
	int N=5;
	double* temp=NULL;
	int t=0;
	float sum_pix=0;
	float avg_energy=0;
	int *pixel;
	int temp1 = 0;

	while (t<9){ 
		avg_energy=0;
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
							sum_pix+= *pixel* filter[t][window_height-i+(N/2)][window_width-j+(N/2)];
						}
					}
					 *(buffer+bpp*(width*i+j))=(uint8_t) sum_pix;
					 avg_energy+= pow(sum_pix,2);
				}
			}
		}


		features[image_num-1][t]=avg_energy/(float)(width*height);
		t++;
	}
}

float calc_centroid_dist(float centroids[NUM_FEATURES], float features[NUM_FEATURES])
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


void centroids_updated(float features[NUM_DATA_PTS][NUM_FEATURES], float centroids[NUM_CENTROIDS][NUM_FEATURES], int *pred_class)
{
	int 	i=0;
	int 	j=0;
	int 	k=0;
	float 	*sum=NULL;
	int 	matching_pt=0;

	sum=(float*)malloc(NUM_FEATURES*sizeof(float));
	if( sum == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
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
FREE_MEM:
	free(sum);
	sum = NULL;
}


void clustering_k_means(float features[NUM_DATA_PTS][NUM_FEATURES], int *pred_class, float centroids[NUM_CENTROIDS][NUM_FEATURES])
{

	int i=0;
	int j=0;
	int k=0;

	float *centroid_distance=(float*)malloc(NUM_CENTROIDS*sizeof(float));
	if( centroid_distance == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	int *previous_class_prediction=(int*)malloc(NUM_DATA_PTS*sizeof(int));
	if(previous_class_prediction == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	

	for (i=0; i<NUM_CENTROIDS;i++)
	{
		memcpy(centroids[i],centroid_initialization[i],NUM_FEATURES*sizeof(float));
	}
	

	for (i=0;i<ITERATIONS_MAX;i++)
	{
		for (j=0;j<NUM_DATA_PTS;j++)
		{
			for (k=0;k<NUM_CENTROIDS;k++)
			{
				centroid_distance[k]=calc_centroid_dist(centroids[k],features[j]);	// num_centroids x num_features (4x9) matrix
			}
			pred_class[j]=min_dist_index(centroid_distance);
		}

		if (memcmp(pred_class,previous_class_prediction,NUM_DATA_PTS*sizeof(int)) == 0){
			break;
		}

		(memcpy(previous_class_prediction, pred_class, NUM_DATA_PTS*sizeof(int)));

		centroids_updated(features, centroids, pred_class);

	}
FREE_MEM:
	free(centroid_distance);
	free(previous_class_prediction);
	centroid_distance 		= NULL;
	previous_class_prediction	= NULL;
}


void classify_textures()
{

	//read each image
	FILE *fp1;
	
	int i=0;
	int j=0;
	char filename[30]={0};
	int height=128;
	int width=128;
	int bpp=1;

	uint8_t *image=(uint8_t*)malloc(width*height*bpp*sizeof(uint8_t));
	if(image == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	int *Zero_Mean_Img=(int*)malloc((width*height*bpp)*sizeof(int));
	if(Zero_Mean_Img == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	uint8_t *buffer=(uint8_t*)malloc(width*height*bpp*sizeof(uint8_t));
	if( buffer == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	int pred_class[NUM_DATA_PTS]={0};
	float features[NUM_DATA_PTS][NUM_FEATURES]={0};
	float centroids[NUM_CENTROIDS][NUM_FEATURES]={0};
	char clusters_str[4][20]={"rock", "grass", "weave", "sand"};

	for (i=1;i<=NUM_DATA_PTS;i++)
	{
		sprintf(filename, "texture%d.raw", (i));
		fp1=fopen(filename,"r");
		fread(image,1, width*height*bpp, fp1);


	//get zero mean image 
		get_image_mean(image,Zero_Mean_Img, width, height, bpp);

	//find features by applying laws' filter and computing average energy

		get_features(Zero_Mean_Img,buffer,features,i,width, height, bpp);

	}
	for (i=1;i<=NUM_DATA_PTS;i++)
	{
		printf("data point %d: ",i);
		for (j=0;j<NUM_FEATURES;j++)
		{
			printf("%.2f \t",features[i-1][j]);
		}
		printf("\n");
	}

	//apply k means
	clustering_k_means(features,pred_class,centroids);
	

	//print results 
	for (j=0;j<NUM_DATA_PTS;j++)
	{
		printf("Texture: %d   Class:%s \n ", (j+1), clusters_str[pred_class[j]]);
	}

FREE_MEM:
	free(image);
	free(Zero_Mean_Img);
	free(buffer);
	image = NULL;
	Zero_Mean_Img = NULL;
	buffer = NULL;
}




int main()
{
	classify_textures();
	return 0;
}



