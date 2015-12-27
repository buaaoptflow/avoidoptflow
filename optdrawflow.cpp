#include "stdafx.h"
#include "optdrawflow.h"
#include "optutil.h"

void drawFlowWithoutZero(CvPoint2D32f* cornersprev, CvPoint2D32f* cornerscurr, IplImage* imgdst, bool isleft){
	for (int i =0; i < MAX_CORNERS; i++)
	{
		if ((int)cornersprev[i].x < 0 || (int)cornersprev[i].x > WIDTH )
		{
			break;
		}
		if (((int)cornersprev[i].x - (int)cornerscurr[i].x) == 0 && ((int)cornersprev[i].y - (int)cornerscurr[i].y) == 0)
		{
			continue;
		}
		CvPoint p,q;
		p.x = isleft ? (int) cornersprev[i].x : ((int)cornersprev[i].x + WIDTH/2);
		p.y = (int) cornersprev[i].y;
		q.x = isleft ? (int) cornerscurr[i].x : ((int)cornerscurr[i].x + WIDTH/2);;
		q.y = (int) cornerscurr[i].y;
		drawArrow(p, q, imgdst);
	}
}

void drawFlowWithoutZero(CvMat* velx, CvMat* vely, IplImage* imgdst){
	for (int i = 0; i < HEIGHT; i += 1)
	{
		for(int j = 0; j < WIDTH; j += 1){
			if ((int) cvGetReal2D(velx, i, j) == 0 && (int) cvGetReal2D(vely, i, j) == 0 )
			{
				continue;
			}
			CvPoint p, q;
			q.x = (int) cvGetReal2D(velx, i, j) + j;
			q.y = (int) cvGetReal2D(vely, i, j) + i;
			p.x = j;
			p.y = i;
			drawArrow(p, q, imgdst);
		}
	}
}

void drawFlowWithoutZero(Mat flow, Mat &framedst){
	for (int i = 0; i < HEIGHT; i += 1)
	{
		for(int j = 0; j < WIDTH; j += 1){
			Vec2f flow_at_point = flow.at<Vec2i>(i, j);
			float fx = flow_at_point[0]/10e8;
			float fy = flow_at_point[1]/10e8;
			if (fabs(fx) > UNKNOWN_FLOW_THRESH || fabs(fy) > UNKNOWN_FLOW_THRESH || (fx == 0 && fy == 0))
			{
				continue;
			}
			CvPoint p, q;
			p.x = j;
			p.y = i;
			q.x = fx + j;
			q.y = fy + i;
			//printf("p: (%d,%d). q :(%d, %d)\n", p.x, p.y, flow.at<Vec2i>(i, j)[0], flow.at<Vec2i>(i, j)[1]);
			drawArrow(p, q, framedst);
		}
	}
}

void drawFlowForFeatureCvPoint(CvPoint2D32f* cornersprev, CvPoint2D32f* cornerscurr, IplImage* imgdst, bool isleft){
	for (int i =0; i < MAX_CORNERS; i++)
	{
		if ((int)cornersprev[i].x < 0 || (int)cornersprev[i].x > WIDTH )
		{
			break;
		}
		CvPoint p,q;
		p.x = isleft ? (int) cornersprev[i].x : ((int)cornersprev[i].x + WIDTH/2);
		p.y = (int) cornersprev[i].y;
		q.x = isleft ? (int) cornerscurr[i].x : ((int)cornerscurr[i].x + WIDTH/2);;
		q.y = (int) cornerscurr[i].y;
		drawArrow(p, q, imgdst);
	}
}

void drawFlowForDenseCvMat(CvMat* velx, CvMat* vely, IplImage* imgdst){
	for (int i = DRAWGAP; i < HEIGHT; i += DRAWGAP)
	{
		for(int j = DRAWGAP; j < WIDTH; j += DRAWGAP){
			CvPoint p, q;
			q.x = (int) cvGetReal2D(velx, i, j) + j;
			q.y = (int) cvGetReal2D(vely, i, j) + i;
			p.x = j;
			p.y = i;
			drawArrow(p, q, imgdst);
		}
	}
}

void drawFlowForDenseMat(Mat flow, Mat &framedst){
	for (int i = DRAWGAP; i < HEIGHT; i += DRAWGAP)
	{
		for(int j = DRAWGAP; j < WIDTH; j += DRAWGAP){
			Vec2f flow_at_point = flow.at<Vec2i>(i, j);
			float fx = flow_at_point[0]/10e8;
			float fy = flow_at_point[1]/10e8;
			if (fabs(fx) > UNKNOWN_FLOW_THRESH || fabs(fy) > UNKNOWN_FLOW_THRESH)
			{
				continue;
			}
			CvPoint p, q;
			p.x = j;
			p.y = i;
			q.x = fx + j;
			q.y = fy + i;
			//printf("p: (%d,%d). q :(%d, %d)\n", p.x, p.y, flow.at<Vec2i>(i, j)[0], flow.at<Vec2i>(i, j)[1]);
			drawArrow(p, q, framedst);
		}
	}
}

void drawArrow(CvPoint p, CvPoint q, IplImage* imgdst, CvScalar rgb , int thickness){
	double angle; 
	angle = atan2((double) p.y - q.y, (double) p.x - q.x);
	double hypotenuse; 
	hypotenuse = sqrt(((p.y - q.y)*(p.y - q.y) +(p.x - q.x)*(p.x - q.x))*1.0);

	q.x = (int) (p.x - 3 * hypotenuse * cos(angle));
	q.y = (int) (p.y - 3 * hypotenuse * sin(angle));
	cvLine(imgdst, p, q, rgb,thickness);

	p.x = (int) (q.x + 2 * hypotenuse * cos(angle + CV_PI / 4));
	p.y = (int) (q.y + 2 * hypotenuse * sin(angle + CV_PI / 4));
	cvLine(imgdst, p, q, rgb,thickness);
	p.x = (int) (q.x + 2 * hypotenuse * cos(angle - CV_PI / 4));
	p.y = (int) (q.y + 2 * hypotenuse * sin(angle - CV_PI / 4));
	cvLine(imgdst, p, q, rgb,thickness);
}

void drawArrow(CvPoint p, CvPoint q, Mat &framedst, CvScalar rgb , int thickness){
	double angle; 
	angle = atan2((double) p.y - q.y, (double) p.x - q.x);
	double hypotenuse; 
	hypotenuse = sqrt(((p.y - q.y)*(p.y - q.y) +(p.x - q.x)*(p.x - q.x))*1.0);

	q.x = (int) (p.x - 3 * hypotenuse * cos(angle));
	q.y = (int) (p.y - 3 * hypotenuse * sin(angle));
	line(framedst, p, q, rgb,thickness);

	p.x = (int) (q.x + 3 * cos(angle + CV_PI / 4));
	p.y = (int) (q.y + 3  * sin(angle + CV_PI / 4));
	line(framedst, p, q,rgb,thickness );

	p.x = (int) (q.x + 3 * cos(angle - CV_PI / 4));
	p.y = (int) (q.y + 3 * sin(angle - CV_PI / 4));
	line(framedst, p, q, rgb,thickness );
}

