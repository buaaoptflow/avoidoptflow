#include "stdafx.h"
#include "optutil.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

using namespace std;
using namespace cv;

/***************************************************************************************/
/*    根据TTC，标记是否为安全区域，暂定大于ttcAvg为安全1，反之，不安全0，然后再调整        
      cols 列数；ttc：计算的ttc数组；ttcAvg：ttc平均值；k：比例系数
      tagSafe : 通过比较规则获得是否安全，标记之                                       */
/***************************************************************************************/
void tagSafeAreaByTTC(int cols, float* ttc, float ttcAvg , float k,int *tagSafe){
	for (int i = 0; i < cols; i++)
	{
		if (ttc[i] > ttcAvg*k)
		{
			tagSafe[i] = 1;
		}
	}
}

/***************************************************************************************/
/*  判定标记是否准确。安全判定安全1，安全非安全2，非安全判定安全3，非安全判定非安全4    
  tagOrigin: 准确的tag标记； tagSafe：根据算法估计的tag标记；tags：根据上述原则判定结果
  函数返回：根据1-4结果，返回此光流方法避撞的评价指数,值越大性能越差                   */
/***************************************************************************************/
float compareTag(int *tagOrigin, int *tagSafe, int cols, int *tags){
	int tag_1 = 0, tag_2 = 0, tag_3 = 0, tag_4 = 0;
	for (int i = 0; i < cols; i++)
	{
		if (tagOrigin[i] == 1 && tagSafe[i] == 1)
		{
			tags[i] = 1;
			tag_1++;
		}else if (tagOrigin[i] == 1 && tagSafe[i] == 0)
		{
			tags[i] = 2;
			tag_2++;
		}else if (tagOrigin[i] == 0 && tagSafe[i] == 1)
		{
			tags[i] = 3;
			tag_3++;
		} 
		else if (tagOrigin[0] == 0 && tagSafe[i] == 0)
		{
			tags[i] = 4;
			tag_4++;
		}
	}
	return tag_1 + tag_2*1.5 + tag_3*2 + tag_4;//各比例项暂定1-1.5-2-1；
}

float balanceControlLR(bool isBig, int leftSumFlow, int rightSumFlow, float k){
    if(isBig){ //前方遇到同一色巨型障碍物，返回-2
        return -2*INT_FLOAT;
    }
    if(leftSumFlow == 0 || rightSumFlow == 0){
        return turnLRScale(leftSumFlow, rightSumFlow, k);
    }
    float gain = (rightSumFlow*INT_FLOAT)/(leftSumFlow*1.0);
    //return gain;
    if(gain < INT_FLOAT*k && gain > INT_FLOAT/k){
        return 0;
    }else{
        return turnLRScale(leftSumFlow, rightSumFlow, k);
    }
}
float turnLRScale(float leftSumFlow, float rightSumFlow, float k){
    if(leftSumFlow == rightSumFlow){
        return 0;
    }else{
        if(K_FLAG){
            k = 1;
        }
        if(leftSumFlow > rightSumFlow){
            return ((leftSumFlow - k*rightSumFlow)*INT_FLOAT)/(leftSumFlow + rightSumFlow);
        }else{
            return ((k*leftSumFlow - rightSumFlow)*INT_FLOAT)/(leftSumFlow + rightSumFlow);
        }
    }
}

Mat calibrate(Mat img)
{
	static double mtx[3][3]={562.89836209,0,314.41994795,0 ,552.27825968,160.37443571,0,0,1};
	static Mat matrix(Size(3,3),CV_64F,mtx);
	static Vec<float,5> dist(-5.25438307e-01,3.76324874e-01,-4.78114662e-04,3.51717002e-04,-1.37709825e-01); 
	static Mat newcameramtx = getOptimalNewCameraMatrix(matrix , dist ,Size(img.rows,img.cols), 0,Size(img.rows,img.cols));
	Mat img2;
	undistort(img,img2,matrix,dist,newcameramtx);
    //matrix.release();
    //img2.release();
    //newcameramtx.release();
	return img2;
}

void calibrate(IplImage* &iplimg)
{
	Mat img(iplimg,false);
	Mat img2 = calibrate(img);
	//iplimg = (IplImage *) cvClone(&IplImage(img2));
	IplImage img3 = IplImage(img2);
	iplimg = (IplImage *)cvClone(&img3);
    img.release();
    img2.release();
}

void writeFile(const char* lineStr){
	FILE *fp = fopen("../video/result.txt", "a+");
	if (fp == 0)
	{
		printf("can't open file\n");
		return;
	}
	fseek(fp, 0, SEEK_END);
	fwrite(lineStr, strlen(lineStr), 1, fp);
	fclose(fp);
}

void drawOrientation(Vec2i leftSumFlow, Vec2i rightSumFlow, int px, int py, float result, IplImage* imgdst){
	Vec2i diffFlow = Vec2i(leftSumFlow[0] - rightSumFlow[0], leftSumFlow[1] - rightSumFlow[1]);
	//printf("diffFlow: %d , %d \n", diffFlow[0], diffFlow[1]);
	//printf("left :%d  right:%d\n", leftSumFlow[0], rightSumFlow[0]);
	cvLine(imgdst, cvPoint(px, py), cvPoint(px+diffFlow[0], py), CV_RGB (0, 255, 0), 3, CV_AA, 0);

	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 1, 1, 0, 2);

	if (result == -2*INT_FLOAT)
	{
		cvPutText(imgdst, "S", cvPoint(20, 20), &font, CV_RGB(255, 0, 0));
	}else if (result  == 0)
	{
		cvPutText(imgdst, "F", cvPoint(20, 20), &font, CV_RGB(255, 0, 0));
	}else if (result < 0)
	{
		cvPutText(imgdst, "L", cvPoint(20, 20), &font, CV_RGB(255, 0, 0));
	}else if (result > 0)
	{
		cvPutText(imgdst, "R", cvPoint(20, 20), &font, CV_RGB(255, 0, 0));
	}

	char c[50];
	//itoa(result, c, 10);
	sprintf(c, "%d", (int)result);
	cvPutText(imgdst, c, cvPoint(10, 50), &font, CV_RGB(255, 0, 0));
}

CvSeq *getImageContours(IplImage *src)  
{  
	cvThreshold(src, src, 100, 255, CV_THRESH_BINARY);  
	CvMemStorage * storage = cvCreateMemStorage(0);  
	CvSeq * contours;  
	cvFindContours(src, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL); //如果有轮廓，则返回第一个轮廓指针，如果没有轮廓，则返回null 
	return contours;  
} 

double huSimilarity(IplImage* src, IplImage* gray){
	CvSeq* contourssrc = getImageContours(src);  //得到src的轮廓
	CvSeq* contoursgray = getImageContours(gray);  //得到gray的轮廓
	if (contoursgray == NULL)
	{
		return MAX_CONTOUR;
	}else{

	}
	return cvMatchShapes(contourssrc, contoursgray, 1);   // 根据输入的图像或轮廓来计算它们的hu矩的相似度 
}

// 内轮廓填充   
// 参数:   
// 1. pBinary: 输入二值图像，单通道，位深IPL_DEPTH_8U。  
// 2. dAreaThre: 面积阈值，当内轮廓面积小于等于dAreaThre时，进行填充。   
void FillInternalContours(IplImage *pBinary, double dAreaThre)   
{   
	double dConArea;   
	CvSeq *pContour = NULL;   
	CvSeq *pConInner = NULL;   
	CvMemStorage *pStorage = NULL;   
	// 执行条件   
	if (pBinary)   
	{   
		// 查找所有轮廓   
		pStorage = cvCreateMemStorage(0);   
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);   
		// 填充所有轮廓   
		cvDrawContours(pBinary, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));  
		// 外轮廓循环   
		int wai = 0;  
		int nei = 0;  
		for (; pContour != NULL; pContour = pContour->h_next)   
		{   
			wai++;  
			// 内轮廓循环   
			for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)   
			{   
				nei++;  
				// 内轮廓面积   
				dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ));  
				//printf("%f\n", dConArea);  
				if (dConArea <= dAreaThre)   
				{   
					cvDrawContours(pBinary, pConInner, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, CV_FILLED, 8, cvPoint(0, 0));  
				}   
			}   
		}   
		//printf("wai = %d, nei = %d\n", wai, nei);  
		cvReleaseMemStorage(&pStorage);   
		pStorage = NULL;   
	}   
}   
int Otsu(IplImage* src)      
{      
	int height=src->height;      
	int width=src->width;          

	//histogram      
	float histogram[256] = {0};      
	for(int i=0; i < height; i++)    
	{      
		unsigned char* p=(unsigned char*)src->imageData + src->widthStep * i;      
		for(int j = 0; j < width; j++)     
		{      
			histogram[*p++]++;      
		}      
	}      
	//normalize histogram      
	int size = height * width;      
	for(int i = 0; i < 256; i++)    
	{      
		histogram[i] = histogram[i] / size;      
	}      

	//average pixel value      
	float avgValue=0;      
	for(int i=0; i < 256; i++)    
	{      
		avgValue += i * histogram[i];  //整幅图像的平均灰度    
	}       

	int threshold;        
	float maxVariance=0;      
	float w = 0, u = 0;      
	for(int i = 0; i < 256; i++)     
	{      
		w += histogram[i];  //假设当前灰度i为阈值, 0~i 灰度的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例    
		u += i * histogram[i];  // 灰度i 之前的像素(0~i)的平均灰度值： 前景像素的平均灰度值    

		float t = avgValue * w - u;      
		float variance = t * t / (w * (1 - w) );      
		if(variance > maxVariance)     
		{      
			maxVariance = variance;      
			threshold = i;      
		}      
	}      

	return threshold;      
}   

