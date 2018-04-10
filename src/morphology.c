/*
Code written by:
Name: 				Shreya Gupta
Code for: 			Morphological image processing operations
*/


#include "morphology.h"

shrink_list_length *head = NULL;
shrink_list_length *curr = NULL;


//---------------------------------------MASKS for SHRINKING AND THINNING-----------------------------------------//
uint8_t stage1_mask_bond[][10]      = { {0x02, 0x08, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 1
                                          {0x01, 0x04, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 2
                                          {0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x81, 0x00, 0x00}, // 3
                                          {0x05, 0x14, 0x50, 0x41, 0x83, 0x0E, 0x38, 0xE0, 0x00, 0x00}, // 4
                                          {0x0D, 0x85, 0x16, 0x43, 0x07, 0x1C, 0x70, 0xC1, 0x00, 0x00}, // 5
                                          {0x8D, 0x36, 0x0F, 0x87, 0x1E, 0x3C, 0x78, 0xF0, 0xE1, 0xC3}, // 6
                                          {0x8F, 0x3E, 0xF8, 0xE3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 7
                                          {0xC7, 0x1F, 0x7C, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 8
                                          {0xCF, 0xE7, 0x3F, 0x9F, 0x7E, 0xFC, 0xF9, 0xF3, 0x00, 0x00}, // 9
                                          {0xEF, 0xBF, 0xFE, 0xFB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 10
                                          {0xDF, 0x7F, 0xFD, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 11
                                          };  

uint8_t stage2_shrinking[][39] = { {0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 1
	                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	                                           0x00, 0x00, 0x00},
                                            {0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 2
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x81, 0x2a, 
                                             0xa8, 0x8a, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //3
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0x23, 0x8c, 0x62, 0x89, 0x26, 0x98, 0x32, 0xc8, 0x4a, 
                                             0xaa, 0x29, 0xa4, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, // 4
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0x16, 0x0d, 0x85, 0x43, 0x1c, 0x3a, 0x2e, 0x2b, 0x9a, 
                                             0x8e, 0x8b, 0x6a, 0xca, 0xe8, 0xb8, 0xac, 0xa9, 0xe2, // 5
                                             0xb2, 0xa3, 0xa6, 0x25, 0x94, 0x52, 0x49, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0x27, 0x72, 0xc9, 0x9c, 0x3c, 0x1e, 0x15, 0x51, 0x54, 
                                             0x45, 0x5a, 0x4e, 0x4b, 0xba, 0xae, 0xab, 0xea, 0x69, // 6
                                             0x39, 0x2d, 0xe4, 0xb4, 0xa5, 0xd2, 0x96, 0x93, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0x5c, 0xbc, 0x9e, 0x3e, 0x1d, 0x35, 0x95, 0x17, 0xd1, 
                                             0x53, 0x71, 0x59, 0x56, 0xd4, 0x74, 0xc5, 0x65, 0x4d, // 7
                                             0x47, 0x3b, 0x2f, 0x9b, 0x8f, 0x7a, 0x6e, 0xda, 0xce, 
                                             0xcb, 0xf8, 0xec, 0xe9, 0xb9, 0xad, 0xf2, 0xe3, 0xb3, 
                                             0xe6, 0xb6, 0xa7},
                                            {0xdc, 0x7c, 0x5e, 0xbe, 0x9d, 0x3d, 0x1f, 0x97, 0xd3, 
                                             0x79, 0xf4, 0xe5, 0x4f, 0x5b, 0xbb, 0xaf, 0xfa, 0xee, // 8
                                             0xeb, 0x6d, 0xb5, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0xfc, 0xde, 0x7e, 0x5d, 0xbd, 0x9f, 0x3f, 0x7b, 0x6f, 
                                             0xdb, 0xcf, 0xf9, 0xed, 0xf3, 0xf6, 0xe7, 0xb7, 0x00, // 9
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0xfe, 0xdd, 0x7d, 0x5f, 0xbf, 0xfb, 0xef, 0xf5, 0xd7, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 10 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00},
                                            {0xfd, 0xdf, 0x7f, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 11
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                             0x00, 0x00, 0x00}
	                                                      };


uint8_t stage2_skeletonizing[][40] = { {0x80, 0x20, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 1
	                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0x40, 0x01, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 2
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0x2a, 0x8a, 0xa8, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 3
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0x05, 0x14, 0x41, 0x50, 0x4a, 0xaa, 0x29, 0xa4, 0x92, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 4
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0x1c, 0xc1, 0x3a, 0x2e, 0x2b, 0x9a, 0x8e, 0x8b, 0x6a, 0xca, 
                                               0xe8, 0xb8, 0xac, 0xa9, 0xe2, 0xb2, 0xa3, 0xa6, 0x25, 0x94,  // 5
                                               0x52, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0x9c, 0x3c, 0x1e, 0xe1, 0xc9, 0xc3, 0x15, 0x54, 0x51, 0x45, 
                                               0x5a, 0x4e, 0x4b, 0xba, 0xae, 0xab, 0xea, 0x69, 0x39, 0x2d,  // 6
                                               0xe4, 0x93, 0xa5, 0xb4, 0x96, 0xd2, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0x5c, 0xbc, 0x9e, 0x3e, 0x1d, 0xd1, 0xe9, 0xc5, 0xe3, 0xcb, 
                                               0x95, 0x35, 0x17, 0xd4, 0x74, 0x56, 0x71, 0x59, 0x53, 0x65,  // 7
                                               0x4d, 0x47, 0x3b, 0x2f, 0x9b, 0x8f, 0x7a, 0x6e, 0x6b, 0xda, 
                                               0xce, 0xf8, 0xb9, 0xec, 0xe6, 0xa7, 0xf2, 0xb3, 0xad, 0xb6},
                                              {0xdc, 0x7c, 0x5e, 0xbe, 0x9d, 0x3d, 0x1f, 0xf1, 0xd9, 0xe5, 
                                               0xcd, 0xd3, 0xeb, 0xc7, 0x55, 0xb5, 0x97, 0x37, 0xf4, 0x76,  // 8
                                               0x73, 0x67, 0x5b, 0x4f, 0x79, 0x6d, 0xd6, 0xee, 0xaf, 0xfa, 
                                               0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0xfc, 0xde, 0x7e, 0x5d, 0xbd, 0x9f, 0x3f, 0xf9, 0xd5, 0xed, 
                                               0xf3, 0xdb, 0xe7, 0xcf, 0x75, 0x57, 0xb7, 0xf6, 0x7b, 0x6f,  // 9
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0xfe, 0xdd, 0x7d, 0x5f, 0xbf, 0xf5, 0xfb, 0xd7, 0xef, 0x77, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 10
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                              {0xfd, 0xdf, 0x7f, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 11
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	                                                              };



uint8_t* pixelposition(uint8_t *inputimage, int i,int j,int  width, int height, int bpp)
{	
	return (inputimage + bpp*(width*i+j)); //j for horizontal and i for vertical 
}


//---------------------------binarizing the input greyscale image-------------------------------------------------------//
void binarize_inp(uint8_t *inputimage, uint8_t *binary_inp, int width, int height, int bpp,int type, int foreground)
{
	int i,j;
	// int foreground=0;
	int thresh;
	if (type==2)
	{
		thresh=103;
	} else if (type==3)
	{
		thresh=102;
	} else{
		thresh=128;
	}

	for(i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			if (foreground==1)
			{

				if(*(inputimage+bpp*(width*i+j))>thresh)
				{
					*(binary_inp+ bpp*(width*i+j))=1;
				} else {
					*(binary_inp+ bpp*(width*i+j))=0;
				}				
			} else {

				if(*(inputimage+bpp*(width*i+j))>thresh)
				{
					*(binary_inp+ bpp*(width*i+j))=0;
				} else {
					*(binary_inp+ bpp*(width*i+j))=1;
				}
			}	
		}
	}

}
//-------------------------------------retreiving the shrinked dots------------------------------------------------------//
 uint8_t current_window(uint8_t *binary_inp, int i,int j, int width, int height, int bpp)
 {
 	int sum=0;
 	uint8_t *pixel,sum_u;
 	pixel=pixelposition(binary_inp,i,j+1,width,height,bpp);
 	sum+= (int) *pixel*pow(2,0);
 	pixel=pixelposition(binary_inp,i-1,j+1,width,height,bpp);
 	sum+= (int) *pixel*pow(2,1);
 	pixel=pixelposition(binary_inp,i-1,j,width,height,bpp);
 	sum+= (int) *pixel*pow(2,2);
 	pixel=pixelposition(binary_inp,i-1,j-1,width,height,bpp);
 	sum+= (int) *pixel*pow(2,3);
 	pixel=pixelposition(binary_inp,i,j-1,width,height,bpp);
 	sum+= (int) *pixel*pow(2,4);
 	pixel=pixelposition(binary_inp,i+1,j-1,width,height,bpp);
 	sum+= (int) *pixel*pow(2,5);
 	pixel=pixelposition(binary_inp,i+1,j,width,height,bpp);
 	sum+= (int) *pixel*pow(2,6);
 	pixel=pixelposition(binary_inp,i+1,j+1,width,height,bpp);
 	sum+= (int) *pixel*pow(2,7);
 	// sum_u=(uint8_t) sum;
 	// printf(" %u \n",(uint8_t) sum);
 	return (uint8_t) sum;
 }

 
 int shrinked_dots(uint8_t *binary_inp,int width,int height,int bpp)
 {
 	int i,j,k,l,sum;
 	int foreground_dots=0;
 	uint8_t window;
 	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			if (*(binary_inp+bpp*(width*i+j))>0)
			{
				window=current_window(binary_inp,i,j,width,height, bpp);

				if (window==0){
					++foreground_dots;
				}
			}
		}
	}
	return foreground_dots;
 }


 

//------------------/* creating link list for shrinking*/-------------------------------------------------------//

void shrink_list_creation(int length,int count)
{
	shrink_list_length *list=(shrink_list_length *)malloc(sizeof(shrink_list_length));
	list->length=length;
	list->count=count;
	list->next=NULL;
	head=curr=list;
}
void shrink_list_add(int length,int count)
{
	if(head==NULL){
		shrink_list_creation(length,count);
	}	else{
		shrink_list_length *node=(shrink_list_length *)malloc(sizeof(shrink_list_length));
		node->length=length;
		node->count=count;
		node->next=NULL;
		curr->next=node;
		curr=node;
	}
}

void free_list()
{
	shrink_list_length *node = NULL;
	shrink_list_length *nextnode = NULL;
	
	node = head;
	if(node != NULL) {
		nextnode = node->next;
		free(node);
		node = nextnode;
	}
}

//----------------------performing stage one of morphological Processing--------------------------------------------------//
int bond_mask(uint8_t window) {
	int i = 0;
	int bond = 0;
	while(i < 8) {
		bond += ((window >> i) & 0x1)*(2 - i%2);
		++i;
	}
	return bond;
}


int bond_elements_stage1(int bond,int type)
{
	int elements_num=0;
	switch(bond) {
		case 1: 
		case 2: {
			if(type == 1) {
				elements_num = 4;
			}
		} break;
		case 3: {
			if(type == 1) {
				elements_num = 8;
			}
		} break;		
		case 4: {
			elements_num = 8;
			if(type == 1) {
				elements_num = 4;
			}
		} break;	
		case 5: {
			elements_num = 8;
			if(type == 3) {
				elements_num = 0;
			}
		} break;
		case 6: {
			elements_num = 10;
			if(type == 3) {
				elements_num = 8;
			}
		} break;					
		case 7:
		case 8:
		case 10: {
			elements_num = 4;
		} break;
		case 9: {
			elements_num = 8;
		} break;
		case 11: {
			if(type == 3) {
				elements_num = 4;
			}
		} break;
		default: {
			printf("Invalid bond value in stage 1 !!!!! %d\n", bond);
		} break;
	}

	return elements_num;
}

uint8_t mask_stage1(int bond, int index, int type )
{
	uint8_t mask = 0;

	if((bond == 4) && (type == 1)) {
		index += 4;
	} 
	if((bond == 6) && (type == 3)) {
		index += 2;
	}

	mask = stage1_mask_bond[bond-1][index];

	return mask;
}



void stage1(uint8_t *inputimage, uint8_t *binary_inp, uint8_t *M, int type,int width,int height, int bpp)
{
	int h=0;
	int v=0;
	int i=0;
	int bond=0;
	int number_of_masks=0;
	uint8_t window=0;
	uint8_t mask=0;
	uint8_t val=0;
	// printf("M matrix is: \n");
	for (v=0;v<height;v++)
	{
		for (h=0;h<width;h++)
		{
			val=0;
			if (*(binary_inp+bpp*(width*v+h)) > 0){
				window=current_window(binary_inp, v, h, width, height, bpp);
				if (window > 0){
					bond=bond_mask(window);
					if (bond < 12){
						number_of_masks= bond_elements_stage1(bond,type);
						i=0;

						while(i<number_of_masks){
							mask=mask_stage1(bond,i,type);
							if (window == mask){
								val=1;
								break;
							}
							++i;
						}
					}
				}
			}	else{
				val=0;
			}
			*(M+bpp*(width*v+h))=val;
			
		}
	
	}
}

//-----------------------------------------------------Finding M compliment ---------------------------------------//
void m_compliment(uint8_t *inputimage, uint8_t *M, uint8_t *Mbar,int width,int height,int bpp)
{

	int i=0;
	int j=0;
	// printf( "\nM compliment matrix is: \n");
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			*(Mbar+bpp*(width*i+j))= *(M+bpp*(width*i+j)) ^ 0x1;
			 // printf( "%u ", *(Mbar+bpp*(width*i+j)));
		}
		// printf("\n");
	}
}

//-------------------------------------------------stage 2------------------------------------------------//

int bond_elements_stage2(int bond, int type)
{
	int num = 0;

	if((type == 1) || (type == 2)) {
		switch(bond) {
			case 1: num = 2; break;
			case 2: num = 2; break;
			case 3: num = 12; break;		
			case 4: num = 13; break;	
			case 5: num = 25; break;
			case 6: num = 26; break;					
			case 7: num = 39; break;
			case 8: num = 22; break;
			case 9: num = 17; break;
			case 10: num = 9; break;
			case 11: num = 4; break;
			default: {
				printf("INVALID bond value\n");
			} break;
		}
	} else { // SKELETONIZING
		switch(bond) {
			case 1: num = 4; break;
			case 2: num = 4; break;
			case 3: num = 4; break;		
			case 4: num = 9; break;	
			case 5: num = 22; break;
			case 6: num = 26; break;					
			case 7: num = 40; break;
			case 8: num = 31; break;
			case 9: num = 20; break;
			case 10: num = 10; break;
			case 11: num = 4; break;
			default: {
				printf("INVALID bond value in stage 2 %d\n", bond);
			} break;
		}		
	}

	return num;
}

uint8_t mask_stage2(int bond, int index, int type )
{
	uint8_t mask = 0;

	if((type == 1) || (type == 2))
		mask = stage2_shrinking[bond-1][index];
	else
		mask = stage2_skeletonizing[bond-1][index];

	return mask;
	
}


void stage2(uint8_t *inputimage, uint8_t *M, uint8_t *prob_hit, int type,int width,int height, int bpp)
{
	int h=0;
	int v=0;
	int i=0;
	int bond=0;
	int number_of_masks=0;
	uint8_t window=0;
	uint8_t mask=0;
	uint8_t val=0;
	// printf("\nProbability matrix is: \n");
	for (v=0;v<height;v++)
	{
		for (h=0;h<width;h++)
		{
			val=0;
			if (*(M+bpp*(width*v+h))>0){
				window=current_window(M, v, h, width, height, bpp);
				if (window>0){
					bond=bond_mask(window);
					if (bond<12){
						number_of_masks= bond_elements_stage2(bond,type);
						
						i=0;
						while(i<number_of_masks){
							mask=mask_stage2(bond,i,type);
							if (window == mask){
								val=1;
								break;
							}
							++i;
						}
					}
				}
			} else {
				val=0;
			}
			*(prob_hit+bpp*(width*v+h)) = val;
			// printf("%u ",*(prob_hit+bpp*(width*v+h)));
			
		}
		// printf("\n");
	}
}

//-----------------------------------------Binary output----------------------------------------------//

void binary_output(uint8_t *Mbar, uint8_t *prob_hit, uint8_t* binary_inp, int width, int height, int bpp)
{
	int i=0,j=0;
	// printf(" output matrix is: \n");
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			*(binary_inp+bpp*(width*i+j))= *(binary_inp+bpp*(width*i+j)) & (*(Mbar+bpp*(width*i+j)) | *(prob_hit+bpp*(width*i+j)));
			 // printf("%u ",*(binary_inp+bpp*(width*i+j)) );
		}
		// printf("\n");
	}
}

//-------------------------------------------------------output star count---------------------------//
void get_star_count(uint8_t *inputimage, uint8_t *binary_inp)
{
	shrink_list_length *node = NULL;
	int total_star_count = 0;
	int size = 0;
	node = head;

	while(node != NULL) {
		
		size = (int)(1.583 * (float) node->length);

		printf("Size: %d Count: %d\n", size, node->count);
		total_star_count += node->count;
		node = node->next;
	}

	printf("\nTotal Star count: %d\n", total_star_count);
}




int main(int argc, char* argv[])

{
	FILE *file_ptr = NULL;
	uint8_t *M 				= NULL;
	uint8_t *Mbar 			= NULL;
	uint8_t *prob_hit	 	= NULL;
	uint8_t *prev_iteration = NULL;
	uint8_t *binary_inp		= NULL;
	uint8_t *outputimage	= NULL;
	uint8_t *inputimage 	= NULL;
	int width		= 0;
	int height		= 0;
	int bpp			= 0;
	int type 		= 0;
	int foreground;
	int iterations = 0;
	int count_last = 0;
	int count_star = 0;
	int count_curr = 0;
	int i=0;
	int j=0;
	uint8_t *pixel;
	if(argc < 7)
	{
		printf("Usage: %s <image name> <image width> <image height> <image bpp> <Morphology type: \n 1. Shrinking \n 2. Thinning \n 3. Skeletonizing> <Foreground>\n", argv[0]);
		exit(1);
	}
	

	width		= atoi(argv[2]);
	height		= atoi(argv[3]);
	bpp			= atoi(argv[4]);
	type 		= atoi(argv[5]); // 1 for shrinking, 2 for thinning, 3 for skeletonizing
	foreground = atoi(argv[6]) ;

	inputimage=(uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(inputimage == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	M 				= (uint8_t *) malloc((width*height*bpp)*sizeof(uint8_t));
	if(M == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	Mbar 			= (uint8_t *) malloc((width*height*bpp)*sizeof(uint8_t));
	if(Mbar == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	prob_hit	 	= (uint8_t *) malloc((width*height*bpp)*sizeof(uint8_t));
	if(prob_hit == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	prev_iteration = (uint8_t *) malloc((width*height*bpp)*sizeof(uint8_t));
	if(prev_iteration == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	binary_inp		= (uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(binary_inp == NULL) {
		printf("[%d] Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	outputimage	= (uint8_t*)malloc((width*height*bpp)*sizeof(uint8_t));
	if(outputimage == NULL) {
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



	

	binarize_inp(inputimage, binary_inp, width, height, bpp, type, foreground);

	
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			/* assesses all points in the binary
			 image which have a white pixel value 
			 against a balck foreground*/
			pixel = (binary_inp+bpp*(width*i+j));
			if ( *pixel==1)
			{
				++iterations;

				count_star=shrinked_dots(binary_inp,width,height,bpp);
				
				count_curr=count_star - count_last;
				count_last=count_star;
				if(count_curr>0){
					shrink_list_add(iterations,count_curr);
				}

				stage1(inputimage, binary_inp, M, type, width, height,bpp);
				m_compliment(inputimage,M,Mbar,width,height,bpp);
				
				stage2(inputimage, M, prob_hit, type, width, height,bpp);
				binary_output(Mbar, prob_hit, binary_inp, width, height,bpp);
				if(memcmp(prev_iteration, binary_inp, width*height*bpp) == 0) {
							break;
				}	

				memcpy(prev_iteration, binary_inp, width*height*bpp);	
			}
		}
	}


	if(foreground==0){
				memset(outputimage,0xFF,width*height*bpp);
			}
	for(i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{	

			if (foreground==1){
				*(outputimage+bpp*(width*i+j))=*(binary_inp+bpp*(width*i+j)) *255;
			}	else{
					if(*(binary_inp+bpp*(width*i+j))>0)
						*(outputimage+bpp*(width*i+j))=0;
			}
		}
	}


	if (type==1){
		get_star_count(inputimage,binary_inp);
	}
	if (type==1){	
		file_ptr= fopen("Output_shrinked.raw","w");
		fwrite(outputimage,1,width*height*bpp,file_ptr);
		 fclose(file_ptr);	
	} else if (type ==2) {
		file_ptr= fopen("Output_Thinned.raw","w");
		fwrite(outputimage,1,width*height*bpp,file_ptr);
		 fclose(file_ptr);	
	} else if (type ==3) {
		file_ptr= fopen("Output_Skeletonized.raw","w");
		fwrite(outputimage,1,width*height*bpp,file_ptr);
		 fclose(file_ptr);	
	} else {
		printf("Invalid Morphology Type ! \n");
	}

FREE_MEM:
	free(inputimage);
	free(outputimage);
	free(binary_inp);
	free(M);
	free(Mbar);
	free(prev_iteration);
	free(prob_hit);
	M 				= NULL;
	Mbar 			= NULL;
	prob_hit	 	= NULL;
	prev_iteration = NULL;
	binary_inp		= NULL;
	outputimage	= NULL;
	inputimage 	= NULL;
	
	free_list();



}