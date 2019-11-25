
#include "Output_L5.h"
#include <hls_math.h>
void Output_Layer(IMAGE_DATA_TYPE ImgIn[L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH], 
		IMAGE_DATA_TYPE &RateOut, IMAGE_DATA_TYPE Bias, IMAGE_DATA_TYPE Weight[L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH])
{
	int i;
	IMAGE_DATA_TYPE Temp;
	Temp = 0;
	//RateOut = 0;
	//¾í»ý
	for (i = 0; i < L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH; i++)
	{

		Temp = Temp + ImgIn[i] * Weight[i];
	}
	Temp = Temp + Bias;
	//¼¤»îº¯Êý
	float t = exp(-(float)Temp);
	Temp = (IMAGE_DATA_TYPE)t + 1;
	RateOut = 1 / Temp;
}
