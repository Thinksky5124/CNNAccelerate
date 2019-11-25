
//

#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\types_c.h>
#include "opencv2/imgproc/imgproc_c.h"
#include "stdio.h"
#include <fstream>
#include <typeinfo>
#include <stdlib.h>


#include "AcclerateCNNIP.h"

#define INPUT_IMAGE "D:\\FPGAProjectFiles\\testImage\\s.jpg"

void importcnn(CNN_Weight* cnn, const char* filename);
//#define ImageOutTest//测试控制语句

using namespace std;
using namespace cv;


int main()
{
	int i, j, s;
	//Mat srcImage = imread("Test1_Golden_Weight.jpg");
	IplImage *src = cvLoadImage(INPUT_IMAGE,-1);
#ifdef ImageOutTest
	Mat dstImage = imread(INPUT_IMAGE, -1);//默认读取原图像
	IplImage dstimage = IplImage(dstImage);
	IplImage *dst = &dstimage;
	//IplImage *dst = cvCreateImage(cvGetSize(src), src->depth, 1);
	int step = dst->widthStep / sizeof(uchar);
	unsigned char pix;
#else
	IMAGE_DATA_TYPE Out[DIV_NUMBER];
#endif
	CNN_Weight CNN;
	CNN_Weight* CNNp = &CNN;
	importcnn(CNNp, "D:\\FPGAProjectFiles\\cnntest\\minst_Weight.cnn");
	//判断图像是否加载成功
	if (src)
		cout << "图像加载成功!" << endl << endl;
	else
	{
		cout << "图像加载失败!" << endl << endl;
		return -1;
	}
	IMAGE_DATA_TYPE ImgIn[HEIGHT][WIDTH];
	//IMAGE_DATA_TYPE ImgOut[MAX_HEIGHT][MAX_WIDTH];
	double r, g, b, gray;
	//read pix
	//printf("ImageIn:\n");
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 0; j < src->width; j++)
		{
			//b = cvGet2D(src, i, j).val[0]; //进行显示转换
			//g = cvGet2D(src, i, j).val[1];
			//r = cvGet2D(src, i, j).val[2];
			gray = cvGet2D(src, i, j).val[0]; //进行显示转换
			//gray = srcImage.at<uchar>(i, j);
			//ImgIn[2][i][j] = (IMAGE_DATA_TYPE)b;
			//ImgIn[1][i][j] = (IMAGE_DATA_TYPE)g;
			gray = gray / 255.0;
			ImgIn[i][j] = (IMAGE_DATA_TYPE)gray;
			//printf("%f ", ImgIn[i][j]);
		}
		//printf("\n");
	}
	cvShowImage("src", src);

	hls_cnn(ImgIn, Out, CNN);

	//Image write compare
#ifdef ImageOutTest
	printf("ImageOut:\n");
	if (dst != 0) //imgSource为IplImage*
	{
		for (int i = 0; i < dst->height; ++i)
		{
			uchar *pucPixel = (uchar *)dst->imageData;
			for (int j = 0; j < dst->width; ++j)
			{
				pucPixel[i * step + j] = ImgOut[i][j]; //像素第一个通道的值
				//printf("%f ", ImgOut[i][j]);
				//pucPixel[3*j + 1] = 0;//像素第二个通道的值
				//pucPixel[3*j + 2] = 0;//像素第三个通道的值
			}
			printf("\n");
		}
}
	Mat matd = cvarrToMat(dst);
	imshow("dst", matd);
#else
	if (Out[0] != 0)
	{
		float cnt = 0;
		for (int i = 0; i < DIV_NUMBER; i++)
		{
			printf("%d:%f  ", i, Out[i]);
			if (Out[i]== 5)
			{
				cnt = cnt + 1;
			}
			if(i%20 == 0)
				printf("\n");
		}
		printf("正确率:%f \n", cnt / DIV_NUMBER);
	}
#endif
	waitKey(0); //参数<=0时等待按键事件发生，按下键的话返回按键的值， 否则返回-1；
	cvReleaseImage(&src);
	#ifdef ImageOutTest
	cvReleaseImage(&dst);
	#endif
	return 0;
}

// 导入cnn的数据
void importcnn(CNN_Weight* cnn, const char* filename)
{
	FILE  *fp = NULL;
	fp = fopen(filename, "rb");
	if (fp == NULL)
		printf("write file failed\n");

	int i, j, c, r;
	// C1的数据
	for (i = 0; i < L1_inChnal; i++)
		for (j = 0; j < L1_outChnal; j++)
			for (r = 0; r < CONVLENGTH; r++)
				for (c = 0; c < CONVLENGTH; c++) {
					float* in = (float*)malloc(sizeof(float));
					fread(in, sizeof(float), 1, fp);
					(*cnn).L1_ConvMapData_r[i][j][r][c] = *in;
				}

	for (i = 0; i < L1_outChnal; i++)
		fread(&(*cnn).L1_Bias[i], sizeof(float), 1, fp);

	// C3网络
	for (i = 0; i < L3_inChnal; i++)
		for (j = 0; j < L3_outChnal; j++)
			for (r = 0; r < CONVLENGTH; r++)
				for (c = 0; c < CONVLENGTH; c++)
					fread(&(*cnn).L3_ConvMapData_r[i][j][r][c], sizeof(float), 1, fp);

	for (i = 0; i < L3_outChnal; i++)
		fread(&(*cnn).L3_Bias[i], sizeof(float), 1, fp);

	// O5输出层
	for (i = 0; i < L5_outChnal; i++)
		for (j = 0; j < L5_neural_inChnal; j++)
			fread(&(*cnn).L5_Weight[i][j], sizeof(float), 1, fp);

	for (i = 0; i < L5_outChnal; i++)
		fread(&(*cnn).L5_Bias[i], sizeof(float), 1, fp);

	fclose(fp);
}
