#ifndef _OUTPUT_L5_H_
#define _OUTPUT_L5_H_

//#include "hls_video.h"
#include "Datatype.h"

void Output_Layer(IMAGE_DATA_TYPE ImgIn[L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH],
		IMAGE_DATA_TYPE &RateOut, IMAGE_DATA_TYPE Bias, IMAGE_DATA_TYPE Weight[L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH]);

#endif
