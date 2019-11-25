#include "ActivationFunctionIP_L3.h"
#include <hls_math.h>
//S1 layer
void Activation_L3(IMAGE_DATA_TYPE ImgIn[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ATVOutput[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE Bias)
{
  int i, j;
  for (i = 0; i < L3_MAX_OUTPUT_HEIGHT; i++)
  {
    for (j = 0; j < L3_MAX_OUTPUT_WIDTH; j++)
    {
      IMAGE_DATA_TYPE temp = ImgIn[i][j] + Bias;
      float t = exp(-(float)temp);
      temp = (IMAGE_DATA_TYPE)t + 1;
      ATVOutput[i][j] = 1 / temp;
    }
  }
}
