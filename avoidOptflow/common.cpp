#include "common.h"


IplImage* imgResize(IplImage* img){
	IplImage* img_1 = cvCreateImage(cvSize(COLSIZE, ROWSIZE), img->depth , img->nChannels);
	cvResize(img, img_1);
	return img_1;
}

void Lucaskanade(IplImage* imgprev, IplImage* imgcurr){

}

void HornSchunck(IplImage* imgprev, IplImage* imgcurr);

void PyrLK(IplImage* imgprev, IplImage* imgcurr);

void blockMatch(IplImage* imgprev, IplImage* imgcurr);

Mat* matResize(Mat* frame);

void simpleFlow(Mat* frameprev, Mat* framecurr);

void farneBack(Mat* frameprev, Mat* framecurr);
