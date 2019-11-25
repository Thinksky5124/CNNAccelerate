
#include "ConvLayerIP_L3.h"

//#include <hls_math.h>
//S1 layer
void Conv_L3(IMAGE_DATA_TYPE ImgIn[AVGP_HEIGHT][AVGP_WIDTH], IMAGE_DATA_TYPE ConvOutput[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ConvMapData_r[CONVLENGTH][CONVLENGTH])
{
  int i, j, m, n;
  IMAGE_DATA_TYPE ImgConv[AVGP_HEIGHT + 2 * (IMAGE_EDAGE_INCREASE)][AVGP_WIDTH + 2 * (IMAGE_EDAGE_INCREASE)];
  IMAGE_DATA_TYPE ImgConvTemp[AVGP_HEIGHT + CONVLENGTH - 1][AVGP_WIDTH + CONVLENGTH - 1];
  IMAGE_DATA_TYPE ConvMapData[CONVLENGTH][CONVLENGTH];
  
  //increase image boundary to conv
  for (i = 0; i < AVGP_HEIGHT + 2 * IMAGE_EDAGE_INCREASE; i++)
  {
    for (j = 0; j < AVGP_WIDTH + 2 * IMAGE_EDAGE_INCREASE; j++)
    {
      if (i < IMAGE_EDAGE_INCREASE || j < IMAGE_EDAGE_INCREASE || i >= (AVGP_HEIGHT + IMAGE_EDAGE_INCREASE) || j >= (AVGP_WIDTH + IMAGE_EDAGE_INCREASE))
      {
        ImgConv[i][j] = 0;
      }
      else
      {
        ImgConv[i][j] = ImgIn[i - IMAGE_EDAGE_INCREASE][j - IMAGE_EDAGE_INCREASE];
      }
    }
  }
  rotationConv_L3(ConvMapData_r, ConvMapData);
  //Conv
  for (i = 0; i < AVGP_HEIGHT + CONVLENGTH - 1; i++)
  {
    for (j = 0; j < AVGP_WIDTH + CONVLENGTH - 1; j++)
    {
      ImgConvTemp[i][j] = 0;
      for (m = 0; m < CONVLENGTH_L3; m++)
      {
        for (n = 0; n < CONVLENGTH_L3; n++)
        {
          ImgConvTemp[i][j] = ImgConvTemp[i][j] + ImgConv[m + i][n + j] * ConvMapData[m][n];
        }
      }
    }
  }
  //Temp to output
  for (i = 0; i < AVGP_HEIGHT + CONVLENGTH - 1; i++)
  {
    for (j = 0; j < AVGP_WIDTH + CONVLENGTH - 1; j++)
    {
		if (j >= IMAGE_EDAGE_INCREASE && i >= IMAGE_EDAGE_INCREASE &&
			i < AVGP_HEIGHT && j < AVGP_WIDTH)
		{
			ConvOutput[i - IMAGE_EDAGE_INCREASE][j - IMAGE_EDAGE_INCREASE] = ImgConvTemp[i][j];
		}
    }
  }
}

void L3_addmat(IMAGE_DATA_TYPE ImgIn[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ImgAdd[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH])
{
	int i, j;
	for (i = 0; i < L3_MAX_OUTPUT_HEIGHT; i++)
	{
		for (j = 0; j < L3_MAX_OUTPUT_WIDTH; j++)
		{
			ImgIn[i][j] = ImgIn[i][j] + ImgAdd[i][j];
		}
	}
}
void rotationConv_L3(IMAGE_DATA_TYPE ConvMapData_r[CONVLENGTH][CONVLENGTH], IMAGE_DATA_TYPE ConvMapData[CONVLENGTH][CONVLENGTH])
{
	int r, c;
	for (r = 0; r < CONVLENGTH; r++)
		for (c = 0; c < CONVLENGTH; c++)
			ConvMapData[r][c] = ConvMapData_r[CONVLENGTH - r - 1][CONVLENGTH - c - 1];
}
