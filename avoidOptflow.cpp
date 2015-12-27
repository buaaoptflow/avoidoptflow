// avoidOptflow.cpp : 定义控制台应用程序的入口点。
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
#include "opticalflow.h"
#include "navigation.h"
#include "video.h"
#include "test.h"
#include "usepython.h"
#include "gui_main.h"
using namespace cv;
using namespace std;

void fillHole(const Mat srcBw, Mat &dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp=Mat::zeros(m_Size.height+2,m_Size.width+2,srcBw.type());//延展图像
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//裁剪延展的图像
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}

int main(int argc, char* argv[])
{
	/* 
		1 - balance; 2 - draw optical flow with gap; 4 - motion to color; 
	   8 - get speed; 16 - draw flow without zero;
	   32 - motion to gray.
	*/
    avoidMain(argc, argv);
    //main_py(argc,argv);

	//dealImgVideo("../video/left.avi");
	//getHuImgVideo("../video/gray_img.avi","../video/color_mat.avi");

	//Mat image=imread("../video/3.png");
	//cvtColor(image,image,CV_BGR2GRAY);
	//vector<vector<Point>> contours;
    //gui_main(argc,argv);

	//// find
	//findContours(image,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

	//// draw
	//Mat result(image.size(),CV_8U,Scalar(0));
	//drawContours(result,contours,-1,Scalar(255),2);
	//cvDrawContours();
	//namedWindow("contours");
	//imshow("contours",result);
	//waitKey();

	return 0;  
}

