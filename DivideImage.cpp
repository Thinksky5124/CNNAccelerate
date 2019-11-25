
#include "DivideImage.h"

void divideImage(IMAGE_DATA_TYPE ImgIn[HEIGHT][WIDTH],int m, int n, IMAGE_DATA_TYPE ImgOut[MAX_HEIGHT][MAX_WIDTH])
{
	int i, j;
	//IMAGE_DATA_TYPE Img[DIV_HEIGHT][DIV_WIDTH];
	//increase boundary
	for (i = 0; i < MAX_HEIGHT; i++)
	{
		for (j = 0; j < MAX_WIDTH; j++)
		{
			if (m*MAX_HEIGHT + i >= HEIGHT ||n*MAX_WIDTH+ j >= WIDTH)
			{
				ImgOut[i][j] = 0.0;
			}
			else
			{
				ImgOut[i][j] = ImgIn[m*MAX_HEIGHT + i][n* MAX_WIDTH + j];
			}
		}
	}
}
