#include <cv.h>
#include <highgui.h>
#include <vector>
#include <stdafx.h>
using namespace std;
using namespace cv;
int buaaMatchShapes(Mat calc_contours,  //需要计算轮廓的图像
                    vector<vector<Point> >& contours,//轮廓
                    vector<Vec4i>& hierarchy,//层次结构
                    char* source_window//绘制轮廓窗口名称
                    ){
    Mat canny_output,dst;

    Canny(calc_contours,canny_output,80,255,3);
    findContours(canny_output,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

    double maxarea = 0;
    int maxAreaIdx = 0;
    double minarea = WIDTH*HEIGHT/36;
    double whRatio = 0;
    RNG rng;

    for (int i = 0; i<contours.size(); i++)
    {

        double tmparea = fabs(contourArea(contours[i]));
        if (tmparea>maxarea)
        {
            maxarea = tmparea;
            maxAreaIdx = i;
            continue;
        }

        if (tmparea < minarea)
        {
            //删除面积小于设定值的轮廓
            contours.erase(contours.begin() + i);
            std::wcout << "delete a small area" << std::endl;
            continue;
        }
        //计算轮廓的直径宽高
        Rect aRect =boundingRect(contours[i]);
        if ((aRect.width / aRect.height)<whRatio)
        {
            //删除宽高比例小于设定值的轮廓
            contours.erase(contours.begin() + i);
            std::wcout << "delete a unnomalRatio area" << std::endl;
            continue;
        }
    }
    /// Draw contours,彩色轮廓
    dst= Mat::zeros(canny_output.size(), CV_8UC3);
    for (int i = 0; i< contours.size(); i++)
    {
        //随机颜色
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(dst, contours, i, color, 2, 8, hierarchy, 0, Point());
    }
    // Create Window
    namedWindow(source_window);
    imshow(source_window, dst);
    waitKey(25);

    return 0;

}
