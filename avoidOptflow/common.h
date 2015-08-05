/************************************************************************/
/* common.h  optflow function                                           */
/************************************************************************/
#include "stdafx.h"
#include <cv.h>
#include "optutil.h"

using namespace cv;
using namespace std;

#ifndef util_OPTFLOW_

#define COLSIZE 320
#define ROWSIZE 280
#define MAX_CORNERS  800

IplImage* imgResize(IplImage* img);

void Lucaskanade(IplImage* imgprev, IplImage* imgcurr);

void HornSchunck(IplImage* imgprev, IplImage* imgcurr);

void PyrLK(IplImage* imgprev, IplImage* imgcurr);

void blockMatch(IplImage* imgprev, IplImage* imgcurr);

Mat* matResize(Mat* frame);

void simpleFlow(Mat* frameprev, Mat* framecurr);

void farneBack(Mat* frameprev, Mat* framecurr);

#endif /* util_OPTFLOW_ */