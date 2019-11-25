
#include "ConvLayerIP_L1.h"

//#include <hls_math.h>
//S1 layer
void Conv(IMAGE_DATA_TYPE ImgIn[MAX_HEIGHT][MAX_WIDTH], IMAGE_DATA_TYPE ConvOutput[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ConvMapData_r[CONVLENGTH][CONVLENGTH])
{
  int i, j, m, n;
  IMAGE_DATA_TYPE ImgConv[MAX_HEIGHT + 2 * (IMAGE_EDAGE_INCREASE)][MAX_WIDTH + 2 * (IMAGE_EDAGE_INCREASE)];
  IMAGE_DATA_TYPE ImgConvTemp[MAX_HEIGHT + CONVLENGTH - 1][MAX_WIDTH + CONVLENGTH - 1];
  IMAGE_DATA_TYPE ConvMapData[CONVLENGTH][CONVLENGTH];
  //increase image boundary to conv
  for (i = 0; i < MAX_HEIGHT + 2* IMAGE_EDAGE_INCREASE; i++)
  {
    for (j = 0; j < MAX_WIDTH + 2* IMAGE_EDAGE_INCREASE; j++)
    {
      if (i < IMAGE_EDAGE_INCREASE || j < IMAGE_EDAGE_INCREASE || i >= (MAX_HEIGHT + IMAGE_EDAGE_INCREASE) || j >= (MAX_WIDTH + IMAGE_EDAGE_INCREASE))
      {
        ImgConv[i][j] = 0.0;
      }
      else
      {
        ImgConv[i][j] = ImgIn[i - IMAGE_EDAGE_INCREASE][j - IMAGE_EDAGE_INCREASE];
      }
    }
  }
  rotationConv(ConvMapData_r, ConvMapData);
  //Conv
  Conv_floaw:for (i = 0; i < MAX_HEIGHT + CONVLENGTH - 1; i++)
  {
    for (j = 0; j < MAX_WIDTH + CONVLENGTH - 1; j++)
    {
      ImgConvTemp[i][j] = 0;
      for (m = 0; m < CONVLENGTH; m++)
      {
        for (n = 0; n < CONVLENGTH; n++)
        {
          ImgConvTemp[i][j] = ImgConvTemp[i][j] + ImgConv[m + i][n + j] * ConvMapData[m][n];
        }
      }
    }
  }
  //shrink edge and Temp to output
  for (i = 0; i < MAX_HEIGHT + CONVLENGTH - 1; i++)
  {
    for (j = 0; j < MAX_WIDTH + CONVLENGTH - 1; j++)
    {
		if (j >= IMAGE_EDAGE_INCREASE  && i >= IMAGE_EDAGE_INCREASE  && 
			i < MAX_HEIGHT && j < MAX_WIDTH)
		{
			ConvOutput[i - IMAGE_EDAGE_INCREASE][j - IMAGE_EDAGE_INCREASE] = ImgConvTemp[i][j];
		}
    }
  }
}

void L1_addmat(IMAGE_DATA_TYPE ImgIn[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ImgAdd[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH])
{
	int i, j;
	for ( i = 0; i < MAX_OUTPUT_HEIGHT; i++)
	{
		for ( j = 0; j < MAX_OUTPUT_WIDTH; j++)
		{
			ImgIn[i][j] = ImgIn[i][j] + ImgAdd[i][j];
		}
	}
}

void rotationConv(IMAGE_DATA_TYPE ConvMapData_r[CONVLENGTH][CONVLENGTH], IMAGE_DATA_TYPE ConvMapData[CONVLENGTH][CONVLENGTH])
{
	int r, c;
	for (r = 0; r < CONVLENGTH; r++)
		for (c = 0; c < CONVLENGTH; c++)
			ConvMapData[r][c] = ConvMapData_r[CONVLENGTH - r - 1][CONVLENGTH - c - 1];
}
