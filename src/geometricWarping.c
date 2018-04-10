/*
Code written by:
Name: 				Shreya Gupta
code for: 			Geometric Warping
*/


#include "geometricWarping.h"
#define pi 3.145



uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontal and i for vertical 
}

void bilinear_interpolation(uint8_t *inputimage,float i1,float j1, int width, int height, int bpp, uint8_t *outputs)
{
	
	uint8_t  *current_pix = NULL;
	uint8_t *right_pix = NULL;
	uint8_t *pix_above = NULL;
	uint8_t *bottomRight_pix = NULL;
	int width1,height1;
	float p_frac= i1;
	float q_frac=j1;
	int p= floor(p_frac);
	int q= floor(q_frac);
	float del_p= p_frac - p;
	float del_q= q_frac - q;


	current_pix=pixelposition(inputimage, p, q, width, height, bpp);
	right_pix=pixelposition(inputimage, p, (q+1), width, height, bpp);
	pix_above=pixelposition(inputimage, (p+1), q, width, height, bpp);
	bottomRight_pix=pixelposition(inputimage, (p+1), (q+1), width, height, bpp);

	*(outputs+0)= (1- del_p)*(1-del_q)* (*current_pix)+ \
	(1- del_p)*(del_q)* (*right_pix)+ \
	(del_p)*(1-del_q)* (*pix_above)+ \
	(del_p)*(del_q)* (*bottomRight_pix); 

	*(outputs+1)= (1- del_p)*(1-del_q)* (*(current_pix+1))+
	(1- del_p)*(del_q)* (*(right_pix+1))+ \
	(del_p)*(1-del_q)* (*(pix_above+1))+ \
	(del_p)*(del_q)* (*(bottomRight_pix+1));

	*(outputs+2)= (1- del_p)*(1-del_q)* (*(current_pix+2))+
	(1- del_p)*(del_q)* (*(right_pix+2))+ \
	(del_p)*(1-del_q)* (*(pix_above+2))+ \
	(del_p)*(del_q)* (*(bottomRight_pix+2));

}


int main(int argc, char* argv[])
{
	FILE *file_ptr  = NULL;
	uint8_t *inputimage = NULL;
	uint8_t *warpedOutput = NULL;
	uint8_t *dewarpedOutput = NULL;
	uint8_t *outputs = NULL;
	int height, width,bpp,x,y,i,j,size;
	int radius, rem,floor_i1,floor_j1;
	long int denom;
	float u,v, distance, center,i1,j1,num;
	float degree;

	if(argc < 5)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp>\n", argv[0]);
		exit(1);
	}
	width=atoi(argv[2]);
	height=atoi(argv[3]);
	bpp=atoi(argv[4]);

	center=width/2;
	radius=center;
	
	inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t*));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	warpedOutput=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t*));
	if(warpedOutput == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	dewarpedOutput=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t*));
	if(dewarpedOutput == NULL) {
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

/*		IMAGE WARPING		*/
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{	
			distance= sqrt(pow((center-i),2)+ pow((center-j),2));
			if ( distance!=0)
			{
				num = acos(abs(center - j) / distance);
				degree=(float) (num) *(180.0/(float)pi);

			}
			if((distance == 0) || (degree==90) || (degree==0))
			{
				*(warpedOutput+bpp*(width*i+j)+0)=*(inputimage+bpp*(width*i+j));
				*(warpedOutput+bpp*(width*i+j)+1)=*(inputimage+bpp*(width*i+j)+1);
				*(warpedOutput+bpp*(width*i+j)+2)=*(inputimage+bpp*(width*i+j)+2);				
			} else {
				if(distance<=radius) 
				{	
					if(degree<=45)
					{
						
						
						i1=(float) abs(center-i)/cos(degree*(pi/180.0));
						j1=(float) abs(center-j)/cos(degree*(pi/180.0));
						
					}
					else
					{	
						i1=abs(center-i)/sin(degree*(pi/180));
						j1=abs(center-j)/sin(degree*(pi/180));
						
					}

					// printf("[i1,j1] %d %d %0.2f %0.2f dist %0.2f angle %0.2f\n",i, j, i1,j1, distance, degree);
					/* 1st quadrant */
					if (i<center && j>=center) // 1st quadrant
					{
						j1=j1+center;
						i1=radius-i1;
					}
					else if (i<center && j<center) //2nd quadrant
					{
						i1=radius-i1;
						j1=radius-j1;
					}
					else if (j<center && i>=center) //3rd quadrant
					{
						i1=center+i1;
						j1=radius-j1;
					}
					else //4th quadrant
					{
						i1=i1+center;
						j1=j1+center;
					}
					floor_i1=floor(i1);
					floor_j1=floor(j1);

					*(warpedOutput+bpp*(width*i+j)+0)=*(inputimage+bpp*(width*floor_i1+floor_j1));
					*(warpedOutput+bpp*(width*i+j)+1)=*(inputimage+bpp*(width*floor_i1+floor_j1)+1);
					*(warpedOutput+bpp*(width*i+j)+2)=*(inputimage+bpp*(width*floor_i1+floor_j1)+2);

				} else {
					*(warpedOutput+bpp*(width*i+j))=0;
				}

			}	

		}
	}

/*		IMAGE DEWARPING		*/
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{	
			distance= sqrt(pow((center-i),2)+ pow((center-j),2));
			if ( distance!=0)
			{
				num = acos(abs(center - j) / distance);
				degree=(float) (num) *(180.0/(float)pi);

			}
			if((distance == 0) || (degree==90) || (degree==0))
			{
				*(dewarpedOutput+bpp*(width*i+j)+0)=*(warpedOutput+bpp*(width*i+j));
				*(dewarpedOutput+bpp*(width*i+j)+1)=*(warpedOutput+bpp*(width*i+j)+1);
				*(dewarpedOutput+bpp*(width*i+j)+2)=*(warpedOutput+bpp*(width*i+j)+2);				
			} else {
				// if(distance<=radius) 
				// {	
					if(degree<=45)
					{
						
						
						i1=(float) abs(center-i)*cos(degree*(pi/180.0));
						j1=(float) abs(center-j)*cos(degree*(pi/180.0));
						
					}
					else
					{	
						i1=abs(center-i)*sin(degree*(pi/180));
						j1=abs(center-j)*sin(degree*(pi/180));
						
					}

					// printf("[i1,j1] %d %d %0.2f %0.2f dist %0.2f angle %0.2f\n",i, j, i1,j1, distance, degree);


						if (i<center && j>=center) // 1st quadrant
						{
							j1=j1+center-1;
							i1=radius-i1+1;
						}
						else if (i<center && j<center) //2nd quadrant
						{
							i1=radius-i1+1;
							j1=radius-j1+1;
						}
						else if (j<center && i>=center) //3rd quadrant
						{
							i1=center+i1-1;
							j1=radius-j1+1;
						}
						else //4th quadrant
						{
							i1=i1+center-1;
							j1=j1+center-1;
						}
					

				outputs = (dewarpedOutput+bpp*(width*i+j)); 
				bilinear_interpolation( warpedOutput, i1, j1, width, height, bpp, outputs);

			}
		}	

	}


	file_ptr= fopen("warpedOutput.raw","w");
	if (file_ptr==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno));  
		exit(0);
	}
	fwrite(warpedOutput,1,width*height*bpp,file_ptr);
	 fclose(file_ptr);
	file_ptr= fopen("dewarpedOutput.raw","w");
	if (file_ptr==NULL)
	{
		printf("[%d] Error %s\n", __LINE__, strerror(errno));  
		exit(0);
	}
	fwrite(dewarpedOutput,1,width*height*bpp,file_ptr);
	 fclose(file_ptr);


FREE_MEM:
	free(inputimage);
	inputimage = NULL;
	free(warpedOutput);
	warpedOutput = NULL;
	free(dewarpedOutput);
	dewarpedOutput = NULL;
	return 0;
}
