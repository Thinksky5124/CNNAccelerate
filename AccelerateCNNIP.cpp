#include "AcclerateCNNIP.h"

void hls_cnn(IMAGE_DATA_TYPE ImgIn[HEIGHT][WIDTH], IMAGE_DATA_TYPE Out[DIV_NUMBER] , CNN_Weight CNN)
{
	int i, j;
	IMAGE_DATA_TYPE Temp[MAX_HEIGHT][MAX_WIDTH] = { 0 };
	for ( i = 0; i < (DIV_HEIGHT / MAX_HEIGHT); i++)
	{
		for ( j = 0; j < (DIV_WIDTH / MAX_WIDTH); j++)
		{
			IMAGE_DATA_TYPE ans_T = 0;
			divideImage(ImgIn, i, j, Temp);
			cnn(Temp, ans_T, CNN);
			Out[i*(DIV_WIDTH / MAX_WIDTH) + j] = ans_T;
		}
	}
}

void cnn(IMAGE_DATA_TYPE ImgIn[MAX_HEIGHT][MAX_WIDTH], IMAGE_DATA_TYPE &Out, CNN_Weight CNN)
{
	int i, j, m, n;
	IMAGE_DATA_TYPE ans[L5_outChnal];
	//Layer Data
	IMAGE_DATA_TYPE L1[L1_outChnal][MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH] = { 0 };
	IMAGE_DATA_TYPE L1_A[L1_outChnal][MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH] = { 0 };
	IMAGE_DATA_TYPE L2[L2_outChnal][AVGP_HEIGHT][AVGP_WIDTH] = { 0 };
	IMAGE_DATA_TYPE L3[L3_outChnal][L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH] = { 0 };
	IMAGE_DATA_TYPE L3_A[L3_outChnal][L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH] = { 0 };
	IMAGE_DATA_TYPE L4[L4_outChnal][L4_AVGP_HEIGHT][L4_AVGP_WIDTH] = { 0 };
	IMAGE_DATA_TYPE L5[L5_inChnal*L4_AVGP_HEIGHT*L4_AVGP_WIDTH] = { 0 };
	//IMAGE_DATA_TYPE Out[L5_outChnal];
	//第一层卷积-激活
	cnn_l1:for (m = 0; m < L1_inChnal; m++)
	{
		for (n = 0; n < L1_outChnal; n++)
		{
			IMAGE_DATA_TYPE T[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH] = { 0 };
			Conv(ImgIn, T, CNN.L1_ConvMapData_r[m][n]);
			L1_addmat(L1[n], T);
			Activation_L1(L1[n], L1_A[n], CNN.L1_Bias[n]);
		}
	}
	//第二层池化
	cnn_l2:for (n = 0; n < L2_outChnal; n++)
	{
		avgPooling(L1_A[n], L2[n]);
	}
	//第三层全连接
	cnn_l3:for (j = 0; j < L3_outChnal; j++)
	{
		for (i = 0; i < L3_inChnal; i++)
		{
			IMAGE_DATA_TYPE T[L3_MAX_OUTPUT_HEIGHT][L3_MAX_OUTPUT_WIDTH] = { 0 };
			Conv_L3(L2[i], T, CNN.L3_ConvMapData_r[i][j]);
			L3_addmat(L3[j], T);
		}
		Activation_L3(L3[j], L3_A[j], CNN.L3_Bias[j]);
	}
	//第四层池化
	cnn_l4:for (n = 0; n < L4_outChnal; n++)
	{
		avgPooling_L4(L3_A[n], L4[n]);
	}
	//第五层输出
	cnn_l5:for (n = 0; n < L5_inChnal; n++)//一维展开
	{
		for (i = 0; i < L4_AVGP_HEIGHT; i++)
		{
			for (j = 0; j < L4_AVGP_WIDTH; j++)
			{
				L5[n*L4_AVGP_HEIGHT*L4_AVGP_WIDTH + i * L4_AVGP_HEIGHT + j] = L4[n][i][j];
			}
		}
	}
	Out = -1;//预设值
	for (i = 0; i < L5_outChnal; i++)
	{
		IMAGE_DATA_TYPE ans_T;
		Output_Layer(L5, ans_T, CNN.L5_Bias[i], CNN.L5_Weight[i]);
		ans[i]=ans_T;
		if (ans[i] > Rate)
		{
			Out = i;
		}
	}
}
