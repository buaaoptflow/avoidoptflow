#pragma once

#include "stdafx.h"
#include <cv.h>

#include "optutil.h"

using namespace cv;
using namespace std;

void drawFlowWithoutZero(CvPoint2D32f* cornersprev, CvPoint2D32f* cornerscurr, IplImage* imgdst, bool isleft);

void drawFlowWithoutZero(CvMat* velx, CvMat* vely, IplImage* imgdst);

void drawFlowWithoutZero(Mat flow, Mat &framedst);

void drawFlowForFeatureCvPoint(CvPoint2D32f* cornersprev_11, CvPoint2D32f* cornerscurr_11, IplImage* imgdst, bool isleft = true);

void drawFlowForDenseCvMat(CvMat* velx, CvMat* vely, IplImage* imgdst);

void drawFlowForDenseMat(Mat flow, Mat &framedst);

void drawArrow(CvPoint p, CvPoint q, IplImage* imgdst, CvScalar rgb = CV_RGB(0,0,255) , int thickness = 1);

void drawArrow(CvPoint p, CvPoint q, Mat &framedst, CvScalar rgb = CV_RGB(0,0,255) , int thickness = 1);

#ifndef util_DRAW_FLOW_
#define util_DRAW_FLOW_

#endif