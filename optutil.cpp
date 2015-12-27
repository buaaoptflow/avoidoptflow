#include "stdafx.h"
#include "optutil.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

using namespace std;
using namespace cv;

/***************************************************************************************/
/*    ����TTC������Ƿ�Ϊ��ȫ�����ݶ�����ttcAvgΪ��ȫ1����֮������ȫ0��Ȼ���ٵ���        
      cols ������ttc�������ttc���飻ttcAvg��ttcƽ��ֵ��k������ϵ��
      tagSafe : ͨ���ȽϹ������Ƿ�ȫ�����֮                                       */
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
/*  �ж�����Ƿ�׼ȷ����ȫ�ж���ȫ1����ȫ�ǰ�ȫ2���ǰ�ȫ�ж���ȫ3���ǰ�ȫ�ж��ǰ�ȫ4    
  tagOrigin: ׼ȷ��tag��ǣ� tagSafe�������㷨���Ƶ�tag��ǣ�tags����������ԭ���ж����
  �������أ�����1-4��������ش˹���������ײ������ָ��,ֵԽ������Խ��                   */
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
	return tag_1 + tag_2*1.5 + tag_3*2 + tag_4;//���������ݶ�1-1.5-2-1��
}

float balanceControlLR(bool isBig, int leftSumFlow, int rightSumFlow, float k){
    if(isBig){ //ǰ������ͬһɫ�����ϰ������-2
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
	cvFindContours(src, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL); //������������򷵻ص�һ������ָ�룬���û���������򷵻�null 
	return contours;  
} 

double huSimilarity(IplImage* src, IplImage* gray){
	CvSeq* contourssrc = getImageContours(src);  //�õ�src������
	CvSeq* contoursgray = getImageContours(gray);  //�õ�gray������
	if (contoursgray == NULL)
	{
		return MAX_CONTOUR;
	}else{

	}
	return cvMatchShapes(contourssrc, contoursgray, 1);   // ���������ͼ����������������ǵ�hu�ص����ƶ� 
}

// ���������   
// ����:   
// 1. pBinary: �����ֵͼ�񣬵�ͨ����λ��IPL_DEPTH_8U��  
// 2. dAreaThre: �����ֵ�������������С�ڵ���dAreaThreʱ��������䡣   
void FillInternalContours(IplImage *pBinary, double dAreaThre)   
{   
	double dConArea;   
	CvSeq *pContour = NULL;   
	CvSeq *pConInner = NULL;   
	CvMemStorage *pStorage = NULL;   
	// ִ������   
	if (pBinary)   
	{   
		// ������������   
		pStorage = cvCreateMemStorage(0);   
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);   
		// �����������   
		cvDrawContours(pBinary, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));  
		// ������ѭ��   
		int wai = 0;  
		int nei = 0;  
		for (; pContour != NULL; pContour = pContour->h_next)   
		{   
			wai++;  
			// ������ѭ��   
			for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)   
			{   
				nei++;  
				// ���������   
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
		avgValue += i * histogram[i];  //����ͼ���ƽ���Ҷ�    
	}       

	int threshold;        
	float maxVariance=0;      
	float w = 0, u = 0;      
	for(int i = 0; i < 256; i++)     
	{      
		w += histogram[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷȵ�����(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���    
		u += i * histogram[i];  // �Ҷ�i ֮ǰ������(0~i)��ƽ���Ҷ�ֵ�� ǰ�����ص�ƽ���Ҷ�ֵ    

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

