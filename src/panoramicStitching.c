#include "panoramicStitching.h"
/*
	PROGRAM DESCRIPTION
	performs Image stitching to form panorama of 3 images
*/

void print_matrix(float **matrix, int col, int row)
{
	int i,j;
	for(i=0;i<col;++i) {
		printf("\n");
		for(j=0;j<row;j++) {
			printf("%0.2f\t",matrix[i][j]);
		}
	}

	printf("\n\n");
}

float** matrix_multiplication(float **A,int a_i,int a_k, float **b, int b_k,int b_j)
{
	int i,j,k;
	float **op=(float**)malloc(a_i*sizeof(float**));
	for (i=0;i<3;i++)
	{
		op[i]=(float*)malloc(b_j*sizeof(float*));
	}


	// print_matrix(A, a_i, a_k);
	// print_matrix(b, b_k, b_j);

	for (i=0;i<a_i;i++)
	{
		for (j=0;j<b_j;j++)
		{
			op[i][j] = 0;

			for (k=0;k<a_k;k++)
			{
				op[i][j]+= A[i][k]*b[k][j];
			}
		}
	}

	return op;
}

int main(int argc, char* argv[])
{
	FILE *file_ptr = NULL;
	int i,j,k,h,w,i2,j2;
	float x_inp, y_inp;
	uint8_t *inputimage = NULL;
	uint8_t *output1 = NULL;
	uint8_t *inputimage2 = NULL;
	uint8_t *inputimage3 = NULL;
	int height = 0;
	int width = 0;
	int bpp = 0;
	int u = 0;
	int v = 0;
	int name2 = 0;
	int name3 = 0;
	float **op = NULL;
	int a_i = 3;
	int a_k = 3;
	int b_k = 0;
	int b_j = 0;




	if(argc < 7)
	{
		printf("Usage: %s <first image name> <image width> <image height> <image bpp> <second image name> <third immage name>\n", argv[0]);
		exit(1);
	}
	width=atoi(argv[2]);
	height=atoi(argv[3]);
	bpp=atoi(argv[4]);

// 	reading image 1 (middle image)
	inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t*));
	if(inputimage == NULL) {
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


// 	reading image 2 (left)
	inputimage2=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t*));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	file_ptr=fopen(argv[5],"rb");
	if (file_ptr==NULL)
	{
		printf("Error %s\n", strerror(errno)); 
		exit(0);
	}
	fread(inputimage2,1,width*height*bpp, file_ptr);
	fclose(file_ptr);


// 	reading image 3 (right)
	inputimage3=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t*));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	file_ptr=fopen(argv[6],"rb");
	if (file_ptr==NULL)
	{
		printf("Error %s\n", strerror(errno)); 
		exit(0);
	}
	fread(inputimage3,1,width*height*bpp, file_ptr);
	fclose(file_ptr);

// allocating memory for output
	output1=(uint8_t*)malloc((5000*5000*3)*sizeof(uint8_t*));

	/* double pointer for matrix 1*/
	float **H=(float**)malloc(3*sizeof(float**));

	for (i=0;i<3;i++)
	{
		H[i]=(float*)malloc(3*sizeof(float*));
	}

	float **O=(float**)malloc(3*sizeof(float**));

	for (i=0;i<3;i++)
	{
		O[i]=(float*)malloc(3*sizeof(float*));
	}


//		HOMOGRAPHY MATRICES	
/* 
inv(H)=
	3.425513	-0.026753	1721.548526
	1.308901	1.336882	-4948.538407
	0.000981	-0.000006	1.019178

taken by computing 4 points from the image

0.519366	0.006729	-844.424150
-0.170482	0.712950	-356.068762
-0.000514	-0.000003	1.796890



	*/

	H[0][0]= 0.519366;	
	H[0][1]= 0.006729;	
	H[0][2]= -844.424150;
	H[1][0]=-0.170482;
	H[1][1]= 0.712950;
	H[1][2]=-356.068762;	
	H[2][0]=-0.000514;
	H[2][1]=-0.000003;
	H[2][2]=1.796890;
	
	/*
O=
-0.393808	0.012757	2226.713685
-0.760081	0.938395	1035.054231
-0.000566	0.000004	1.009430

O_inv=

1.170153	-0.004169	-2576.980331
0.225610	1.069847	-1594.680800
0.000655	-0.000007	-0.446618

	*/	
	
	O[0][0]=1.170153;
	O[0][1]=-0.004169;
	O[0][2]=-2576.980331;
	O[1][0]=0.225610;
	O[1][1]=1.069847;
	O[1][2]=-1594.680800;	
	O[2][0]=0.000655;
	O[2][1]=-0.000007;
	O[2][2]=-0.446618;


	/* double pointer for matrix 1*/
	float **b=(float**)malloc(3*sizeof(float**));
	for (i=0;i<3;i++)
	{
		b[i]=(float*)malloc(sizeof(float*));
	}

	// pasting middle image onto canvas
	for (h=0;h<height;h++)
		{
			for (w=0;w<width;w++)
			{
				b[0][0]=w+0.5;//x
				b[1][0]=height-0.5-h; //y
				b[2][0]=1;


				i=5000- 0.5 - b[1][0] - 1000;
				j=b[0][0]- 0.5 + 2000;

	/*			Pasting middle image on output 		*/
					*(output1 + bpp*(5000*i+j)+0) = *(inputimage + bpp*(width*h+w)+0);
					*(output1 + bpp*(5000*i+j)+1) = *(inputimage + bpp*(width*h+w)+1);
					*(output1 + bpp*(5000*i+j)+2) = *(inputimage + bpp*(width*h+w)+2);


			}
		}



// pasting left image onto canvas
	for (h=0;h<5000;h++)
	{
		for (w=0;w<5000;w++)
		{
			b[0][0]=w+0.5;//x
			b[1][0]=5000-0.5-h; //y
			b[2][0]=1;
			b_k=3;
			b_j=1;

			

			op=(matrix_multiplication(H, a_i,a_k, b, b_k,b_j));
			x_inp= floor(op[0][0]*(1/op[2][0])); 
			y_inp= floor(op[1][0]*(1/op[2][0]));
			i2= height-0.5-y_inp;
			j2=	x_inp-0.5;
			
			if (i2<height && i2>=0 && j2<width && j2>=0)
			{
				
				// i2=5000-0.5- op[1][0] - 1000;
				// j2=op[0][0]-0.5 + 1000;
				
				/*			Pasting left image on output 		*/
				*(output1 + bpp*(5000*h+w)) = *(inputimage2 + bpp*(width*i2+j2));
				*(output1 + bpp*(5000*h+w)+1) = *(inputimage2 + bpp*(width*i2+j2)+1);
				*(output1 + bpp*(5000*h+w)+2) = *(inputimage2 + bpp*(width*i2+j2)+2);
			}
			
			
		}
		// getchar();
	}
// pasting right image onto canvas
	for (h=0;h<5000;h++)
	{
		for (w=0;w<5000;w++)
		{
			b[0][0]=w+0.5;//x
			b[1][0]=5000-0.5-h; //y
			b[2][0]=1;
			b_k=3;
			b_j=1;

			

			op=(matrix_multiplication(O, a_i,a_k, b, b_k,b_j));
			x_inp= floor(op[0][0]*(1/op[2][0])); 
			y_inp= floor(op[1][0]*(1/op[2][0]));
			i2= height-0.5-y_inp;
			j2=	x_inp-0.5;
			
			
			if (i2<height && i2>=0 && j2<width && j2>=0)
			{
				
				// i2=5000-0.5- op[1][0] - 1000;
				// j2=op[0][0]-0.5 + 1000;
				

				
				/*			Pasting left image on output 		*/
				*(output1 + bpp*(5000*h+w)) = *(inputimage3 + bpp*(width*i2+j2));
				*(output1 + bpp*(5000*h+w)+1) = *(inputimage3 + bpp*(width*i2+j2)+1);
				*(output1 + bpp*(5000*h+w)+2) = *(inputimage3 + bpp*(width*i2+j2)+2);
				

				// printf("[%d %d] %u -> [%d %d] %u\n",h, w,*(inputimage3 + bpp*(width*i2+j2)), i2, j2,*(output1 + bpp*(5000*h+w)));
			}
			
			
		}
		// getchar();
	}


	file_ptr= fopen("homography.raw","w");
	fwrite(output1,1,5000*5000*3,file_ptr);
	fclose(file_ptr);

FREE_MEM:

	free(inputimage);
	inputimage = NULL;
	free(inputimage2);
	inputimage2 = NULL;
	free(output1);
	output1 = NULL;
	return 0;
}