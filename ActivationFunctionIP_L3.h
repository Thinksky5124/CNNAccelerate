#ifndef _ACTIVATIONFUNCTION_L3_H_
#define _ACTIVATIONFUNCTION_L3_H_

//#include "hls_video.h"
#include "Datatype.h"

//activation module Sigma

void Activation_L3(IMAGE_DATA_TYPE ImgIn[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ATVOutput[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE Bias);

#endif