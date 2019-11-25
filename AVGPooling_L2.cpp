#include "AVGPooling_L2.h"

//#include <hls_math.h>

void avgPooling(IMAGE_DATA_TYPE ImgIn[ATV_HEIGHT][ATV_WIDTH], IMAGE_DATA_TYPE ImgOut[AVGP_HEIGHT][AVGP_WIDTH])
{
  int i,j , m, n;
  IMAGE_DATA_TYPE sum;
  for (i = 0; i < AVGP_HEIGHT; i++)
  {
    for (j = 0; j < AVGP_WIDTH; j++)
    {
      sum = 0;
      for (m = 0; m < PoolingKernal; m++)
      {
        for (n = 0; n < PoolingKernal; n++)
        {
          sum = sum + ImgIn[m + i * PoolingKernal][n + j * PoolingKernal];
        }
      }
      ImgOut[i][j] = sum / (PoolingKernal * PoolingKernal);
    }
  }
}
