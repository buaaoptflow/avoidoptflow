/*
*  FILE stdafx.h
*  AUTHOR Sarah
*  DATE 2015/08/14 22:27
*  TODO: 鏍囧噯绯荤粺鍖呭惈鏂囦欢鐨勫寘鍚枃浠讹紝鎴栨槸缁忓父浣跨敤浣嗕笉甯告洿鏀圭殑鐗瑰畾浜庨」鐩殑鍖呭惈鏂囦欢
*/
#pragma once

#include <stdio.h>

/*
*  ROWS: 鏁扮粍鐢宠鍐呭瓨绌洪棿澶у皬锛屽ぇ浜庣瓑浜嶩EIGHT.
*  COLS: 鏁扮粍鐢宠鍐呭瓨绌洪棿澶у皬锛屽ぇ浜庣瓑浜嶹IDTH.
*  HEIGHT: 閲嶇疆鍥惧儚鍚庣殑楂?
*  WIDTH: 閲嶇疆鍥惧儚鍚庣殑瀹?
*  MAX_CORNERS: PyrLK璁＄畻鐗瑰緛鐐规暟鐩?
*  WINSIZE: 绐楀彛澶у皬.
*  DRAWGAP: 鐢诲厜娴佺殑闂撮殧.
*  UNKNOWN_FLOW_THRESH: 澶т簬姝ゅ?寰楀厜娴佽涓烘槸error鍏夋祦. 
*/
#define ROWS 350
#define COLS 350
//#define HEIGHT 190  //matlab
//#define WIDTH 288   //matlab
#define HEIGHT 180   //ARDrone
#define WIDTH 320   //ARDrone
#define MAX_CORNERS  200
#define WINSIZE 5
#define DRAWGAP 15
#define UNKNOWN_FLOW_THRESH 1e10
#define LK_K 6
#define HS_K 4.3
#define BM_K 1.4
#define FB_K 2
#define SF_K 3 //未测试
#define PYRLK_K 3 //未测试
#define INT_FLOAT 100.0 //matlab中int向上float转型有问题，所以结果乘100，处理时再除100.
#define K_FLAG true  //在避撞偏移量中，false表示k取1，而非上述*_K变量
#define EDGE 0 //0.1429
#define EDGE_OBS 0.33
#define COLOR_SCALE 20
#define THRESHOLD_TIMER 0.75 //同色所占比例大于整幅图像的75%，则认为是墙，停止。
#define THRESHOLD_ZERO 0.8
#define FLOW_ZERO 0
#define FB_SCALE 10e8
#define IS_FLOW_WRITE_FILE false   //是否将左右光流数据写入文件
#define IS_CALIBRATE false//是否进行摄像头标定
//#define TACHOGRAPH_UPX 0.25  //地面
//#define TACHOGRAPH_UPY 0.6
//#define TACHOGRAPH_DOWNX 0.75
//#define TACHOGRAPH_DOWNY 0.9
#define TACHOGRAPH_UPX 0
#define TACHOGRAPH_UPY 0
#define TACHOGRAPH_DOWNX 1
#define TACHOGRAPH_DOWNY 1
#define TACHOGRAPH_ANGLE 40
#define TACHOGRAPH_FOCAL 68 //单位190像素，注：1080p焦距为1141像素
#define TACHOGRAPH_HEIGHT 1.2 // 单位米
#define IS_ROS true //matlab和ros仿真环境中大型障碍物判断不一样，因为matlab中大型障碍物无纹理；光流为0，ROS中反之，所以不同。
#define MAX_CONTOUR 100  //huSimilarity函数hu阵相似度，如果相似则为0，越不相似越大，这里为了防止光流没有轮廓造成空指针，则hu阵直接返回100作为标记。
#define IS_SIMILARITY false  //如果画轮廓，默认求相似度且写入文件result
#define IS_INTERPOLATION false //motiontogray函数时是否插值，即其上下左右INTERPOLATION_GAP范围内，有两个白点，则其插值为白点
#define IS_FILLINTERNALCONTOURS false //是否填充内轮廓
#define INTERPOLATION_GAP 5 //按照半径为INTERPOLATION_GAP的窗口插白点
#define DRAWCONTOUR true //画轮廓


