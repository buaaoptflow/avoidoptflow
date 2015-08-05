/************************************************************************/
/* optflowutil.h  foe ttc                                               */
/************************************************************************/
#include "stdafx.h"
#include <cv.h>

using namespace cv;
using namespace std;

#define COLS 3000
#define ROWS 2000

#ifndef util_FOE_TTC_

Vec2i foeForDenseCvMat(CvMat* velx, CvMat* vely);

float ttcForDenseCvMat(CvMat* vely, int foeY, float *ttc);

void tagSafeAreaByTTC(int cols, float* ttc, float ttcAvg , float k,int *tagSafe);

float compareTag(int *tagOrigin, int *tagSafe, int cols, int *tags);

#endif /* util_FOE_TTC_ */