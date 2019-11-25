#include "ActivationFunctionIP_L1.h"
#include <hls_math.h>
//S1 layer
void Activation_L1(IMAGE_DATA_TYPE ImgIn[ATV_HEIGHT][ATV_WIDTH], IMAGE_DATA_TYPE ATVOutput[ATV_HEIGHT][ATV_WIDTH], IMAGE_DATA_TYPE Bias)
{
  int i, j;
  for (i = 0; i < ATV_HEIGHT; i++)
  {
    for (j = 0; j < ATV_WIDTH; j++)
    {
      IMAGE_DATA_TYPE temp = ImgIn[i][j] + Bias;
      float t = exp(-(float)temp);
      temp = (IMAGE_DATA_TYPE)t + 1;
      ATVOutput[i][j] = 1 / temp;
    }
  }
}
