
#include "AVGPooling_L4.h"

#include <hls_math.h>

void avgPooling_L4(IMAGE_DATA_TYPE ImgIn[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_HEIGHT], IMAGE_DATA_TYPE ImgOut[L4_AVGP_HEIGHT][L4_AVGP_WIDTH])
{
  int i, j, m, n;
  IMAGE_DATA_TYPE sum;
  for (i = 0; i < L4_AVGP_HEIGHT; i++)
  {
    for (j = 0; j < L4_AVGP_WIDTH; j++)
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
