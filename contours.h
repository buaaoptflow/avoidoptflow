#ifndef CONTOURS_H
#define CONTOURS_H
/*!
 * \brief buaaMatchShapes find the contours of an gray image.
 * \param calc_contours the source image
 * \param contours  contours
 * \param hierarchy hierarchy
 * \param source_window window name
 * \return
 */
int buaaMatchShapes(Mat calc_contours,  //需要计算轮廓的图像
                    vector<vector<Point> >& contours,//轮廓
                    vector<Vec4i>& hierarchy,//层次结构
                    char* source_window//绘制轮廓窗口名称
                    );

#endif // CONTOURS_H

