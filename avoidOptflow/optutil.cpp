#include "stdafx.h"
#include "optutil.h"
#include <cv.h>

using namespace std;
using namespace cv;

/************************************************************************/
/* ���ܹ�����������������ΪCvMat��FOE��x��y���ļ���
   velx��x���������vely��y�������                                     */
/************************************************************************/
Vec2i foeForDenseCvMat(CvMat* velx, CvMat* vely){
	//calculate x of foe
	float cols[COLS];
	for (int col = 0; col < velx->width; col++)
	{
		float tmp = 0;
		for (int row = 0; row < velx->height; row++)
		{
			float* pData = velx->data.fl + row*velx->step/4;
			tmp += *(pData + col); 
		}
		cols[col] = tmp;
	}
	float colsLeft[COLS];//��col���Ĺ���֮��
	for (int i = 0; i < velx->width; i++)
	{
		colsLeft[i] = cols[i];
		if (i > 0)
		{
			colsLeft[i] += colsLeft[i-1];
		}
	}
	float colsRight[COLS];//��col�Ҳ�Ĺ���֮��
	for (int j = velx->width - 1; j >= 0; j--)
	{
		colsRight[j] = cols[j];
		if (j < velx->width -1)
		{
			colsRight[j] += colsRight[j+1];
		};
	}
	//�ҳ�xʹ����������֮����С��Ϊfoe���x
	float colsMin = abs(abs(colsLeft[0]) - abs(colsRight[0]));
	int foeX = 0;
	for (int i = 1; i < velx->width; i++)
	{
		float tmp = abs(abs(colsLeft[i]) - abs(colsRight[i]));
		if (tmp < colsMin)
		{
			colsMin = tmp;
			foeX = i;
		}
	}

	//calculate y of foe
	float rows[ROWS];
	for (int row = 0 ; row < vely->height; row++ ){
		float tmp = 0;
		float* pData = vely->data.fl + row*vely->step/4;
		for(int col = 0 ; col < vely->width; col++){
		}
	}
	float rowsUp[ROWS];
	for (int i = 0; i < vely->height; i++)
	{
		rowsUp[i] = rows[i];
		if (i > 0)
		{
			rowsUp[i] += rowsUp[i-1];
		}
	}
	float rowsDown[ROWS];
	for (int j = vely->height - 1; j >= 0; j--)
	{
		rowsDown[j] = rows[j];
		if (j < vely->height-1)
		{
			rowsDown[j] += rowsDown[j+1];
		}
	}
	float rowsMin = abs(abs(rowsUp[0]) - abs(rowsDown[0]));
	int foeY = 0;
	for (int i = 1; i < vely->height; i++)
	{
		float tmp = abs(abs(rowsUp[i]) - abs(rowsDown[i]));
		if (tmp < rowsMin)
		{
			rowsMin = tmp;
			foeY = i;
		}
	}
	return Vec2i(foeX, foeY);
}

/************************************************************************/
/* ���ܹ����ҹ�����������ΪCvMat��TTC���㣬����ƽ��TTCֵ
   vely��y��������� foeY��foe��y�����ꣻttc�������õ�ÿ�е�ttc       */
/************************************************************************/
float ttcForDenseCvMat(CvMat* vely, int foeY, float *ttc){
	float sum  = 0;
	for (int col = 0; col < vely->width; col++)
	{
		float tmp = 0;
		for (int row = 0; row < vely->height; row++)
		{
			float* pData = vely->data.fl + row*vely->step/4 + col;
			if (*pData == 0)
			{
				tmp += abs((row - foeY)*10); //(row - foeY)/0.1,���Ϊ0������Ϊ�ٶ�Ϊ0.1
			}else{
				tmp += abs((row - foeY)/(*pData));
			}
		}
		ttc[col] = tmp/(vely->height);
		sum += ttc[col];
	}
	return sum/(vely->width);
}


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

