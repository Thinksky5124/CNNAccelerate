#ifndef _ACCLERATECNNIP_H_
#define _ACCLERATECNNIP_H_

#include "Datatype.h"
#include "ActivationFunctionIP_L1.h"
#include "AVGPooling_L2.h"
#include "AVGPooling_L4.h"
#include "ConvLayerIP_L1.h"
#include "ConvLayerIP_L3.h"
#include "ActivationFunctionIP_L3.h"
#include "Output_L5.h"
#include "DivideImage.h"

// I/O Image Settings

typedef struct cnn_weight {
	IMAGE_DATA_TYPE L1_ConvMapData_r[L1_inChnal][L1_outChnal][CONVLENGTH][CONVLENGTH];
	IMAGE_DATA_TYPE L1_Bias[L1_outChnal];
	IMAGE_DATA_TYPE L3_ConvMapData_r[L3_inChnal][L3_outChnal][CONVLENGTH][CONVLENGTH];
	IMAGE_DATA_TYPE L3_Bias[L3_outChnal];
	IMAGE_DATA_TYPE L5_Weight[L5_outChnal][L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH];
	IMAGE_DATA_TYPE L5_Bias[L5_outChnal];
}CNN_Weight;

// top level function for HW synthesis
void hls_cnn(IMAGE_DATA_TYPE ImgIn[HEIGHT][WIDTH], IMAGE_DATA_TYPE Out[DIV_NUMBER], CNN_Weight CNN);
void cnn(IMAGE_DATA_TYPE ImgIn[MAX_HEIGHT][MAX_WIDTH], IMAGE_DATA_TYPE &Out, CNN_Weight CNN);

#endif
