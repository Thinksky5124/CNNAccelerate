#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include <stdio.h>
#include <iostream>
#include <ap_fixed.h>
#include <ap_int.h>

// Image size
#define HEIGHT 480
#define WIDTH 640

// Image size increase to divide
#define DIV_HEIGHT 504
#define DIV_WIDTH 644

// CNN image size
#define MAX_HEIGHT 28
#define MAX_WIDTH 28

#define DIV_NUMBER ((DIV_HEIGHT / MAX_HEIGHT)*(DIV_WIDTH / MAX_WIDTH))
// typedef video library core structures
//typedef ap_fixed<28,2> IMAGE_DATA_TYPE;
typedef float IMAGE_DATA_TYPE;
//typedef float  IMAGE_DATA_TYPE;

#define Rate 0.98//¼ì²âµÄãÐÖµ

//LAYER IMAGE SIZE DEFINE
//L1 convolution
#define L1_inChnal 1
#define L1_outChnal 6
#define CONVLENGTH 5

#define MAX_OUTPUT_HEIGHT (MAX_HEIGHT - CONVLENGTH + 1)
#define MAX_OUTPUT_WIDTH (MAX_WIDTH - CONVLENGTH + 1)

#define IMAGE_EDAGE_INCREASE (CONVLENGTH - 1)

//L1 activation image size
#define L1_inChnal 1
#define L1_outChnal 6
#define ATV_HEIGHT MAX_OUTPUT_HEIGHT
#define ATV_WIDTH MAX_OUTPUT_WIDTH

#define PoolingKernal 2 //Pool kernal size
//L2 avgPooling
#define L2_inChnal 6
#define L2_outChnal 6
#define AVGP_HEIGHT (ATV_HEIGHT / PoolingKernal)
#define AVGP_WIDTH (ATV_WIDTH / PoolingKernal)

//L3 convolution
#define L3_inChnal 6
#define L3_outChnal 12
#define CONVLENGTH_L3 5

#define L3_MAX_OUTPUT_HEIGHT (AVGP_HEIGHT - CONVLENGTH_L3 + 1)
#define L3_MAX_OUTPUT_WIDTH (AVGP_WIDTH - CONVLENGTH_L3 + 1)

//L4 avgPooling
#define L4_inChnal 12
#define L4_outChnal 12
#define L4_AVGP_HEIGHT (L3_MAX_OUTPUT_HEIGHT / PoolingKernal)
#define L4_AVGP_WIDTH (L3_MAX_OUTPUT_WIDTH / PoolingKernal)

//L5 OutPut
#define L5_inChnal 12
#define L5_neural_inChnal L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH 
#define L5_outChnal 10
#endif
