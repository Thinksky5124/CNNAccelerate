#ifndef _CONVLAYERIP_H_
#define _CONVLAYERIP_H_

#include "Datatype.h"

//ConvMapData not rotation
/*
const IMAGE_DATA_TYPE ConvMapData_r[CONVLENGTH][CONVLENGTH] = {0, -1, 0,
                                                               -1, 5, -1,
                                                               0, -1, 0};
*/
//Store ConvMapData rotation


// top level function for HW synthesis
void Conv(IMAGE_DATA_TYPE ImgIn[MAX_HEIGHT][MAX_WIDTH], IMAGE_DATA_TYPE ConvOutput[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ConvMapData_r[CONVLENGTH][CONVLENGTH]);
void L1_addmat(IMAGE_DATA_TYPE ImgIn[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH], IMAGE_DATA_TYPE ImgAdd[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH]);
void rotationConv(IMAGE_DATA_TYPE ConvMapData_r[CONVLENGTH][CONVLENGTH], IMAGE_DATA_TYPE ConvMapData[CONVLENGTH][CONVLENGTH]);
#endif