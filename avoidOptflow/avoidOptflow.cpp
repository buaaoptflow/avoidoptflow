// avoidOptflow.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cv.h>
#include <highgui.h>
#include "opencv2/legacy/legacy.hpp"
#include <math.h>

#include <cstdio>
#include <iostream>

#include "optutil.h"

using namespace cv;
using namespace std;

#define COLSIZE 320
#define ROWSIZE 280

//����LK����
void Lucaskanade(IplImage* imgA, IplImage* imgB){
	//(1) ǰ�ڳ�ʼ������
	int  rows, cols;
	CvMat *velx, *vely;

	if (imgA == NULL)
	{
		/* Why did we get a NULL frame? We shouldn't be at the end. */
		printf("Image #1 : %s cannot be read\n");
		exit(1);
	}
	IplImage* imgA_1 = cvCreateImage(cvGetSize(imgA), imgA->depth, 1);
	cvCvtColor(imgA, imgA_1, CV_BGR2GRAY);
	if (imgB == NULL)
	{
		/* Why did we get a NULL frame? We shouldn't be at the end. */
		printf("Image #2 : %s cannot be read\n");
		exit(1);
	}
	IplImage* imgB_1 = cvCreateImage(cvGetSize(imgB), imgB->depth, 1);
	cvCvtColor(imgB, imgB_1, CV_BGR2GRAY);

	cols = imgB->width;
	rows = imgB->height;
	velx = cvCreateMat (rows, cols, CV_32FC1);
	vely = cvCreateMat (rows, cols, CV_32FC1);
	cvSetZero (velx);
	cvSetZero (vely);
	CvSize winSize = cvSize(5, 5);

	printf("Read two images of size [rows = %d, cols = %d]\n",
		rows, cols);

	// (2)���㣨LK��
	float start = (float)getTickCount();
	cvCalcOpticalFlowLK (imgA_1, imgB_1, winSize, velx, vely);
	printf("cvCalcOpticalFlowLK : %lf sec\n", (getTickCount() - start) / getTickFrequency());
	
	// (3)����foe��
	Vec2i vec2iFOE = foeForDenseCvMat(velx, vely);
	printf("[rows(y) = %d,cols(x) = %d]\n", vec2iFOE[1],vec2iFOE[0]);

	cvReleaseImage(&imgA_1);
	cvReleaseImage(&imgB_1);
}


int _tmain(int argc, _TCHAR* argv[])
{
	IplImage* imgA=cvLoadImage("9.jpg", 1);
	IplImage* imgB=cvLoadImage("10.jpg", 1);
	IplImage* imgA_1 = cvCreateImage(cvSize(COLSIZE, ROWSIZE), imgA->depth , imgA->nChannels);
	IplImage* imgB_1 = cvCreateImage(cvSize(COLSIZE, ROWSIZE), imgB->depth , imgB->nChannels);
	cvResize(imgA, imgA_1);
	cvResize(imgB, imgB_1);
	Lucaskanade(imgA,imgB);
	printf("resize\n");
	
	Lucaskanade(imgA_1,imgB_1);

	cvReleaseImage(&imgA_1);
	cvReleaseImage(&imgB_1);
	cvReleaseImage(&imgA);
	cvReleaseImage(&imgB);

	return 0;
}

