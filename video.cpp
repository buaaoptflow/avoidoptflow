#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <time.h>

#include "navigation.h"
#include "video.h"

using namespace cv;
using namespace std;

void imgVideo(ImgFunType funtype, int strategic, String inputfile){
	
	CvCapture *input_video = cvCreateFileCapture(inputfile.c_str());
	if (input_video == NULL)
	{
		printf("Error: Can't open video.\n");
		exit(-1);
	}
	IplImage *img = NULL, *img_1 = NULL, *img_2 = NULL;

	CvVideoWriter* writer = cvCreateVideoWriter("../video/flow_img.avi", CV_FOURCC('P', 'I', 'M', '1'),20,cvSize(WIDTH, HEIGHT),1);
	VideoWriter writercolor("../video/color_img.avi", CV_FOURCC('M', 'J', 'P', 'G'),20,cvSize(WIDTH, HEIGHT),true);
	VideoWriter writergray("../video/gray_img.avi", CV_FOURCC('M', 'J', 'P', 'G'),20,cvSize(WIDTH, HEIGHT),false);

	while(true)
	{
		img = cvQueryFrame( input_video );
		if (img == NULL)
		{
			printf("video over.\n");
			break;;
		}
		img_1 = (IplImage *) cvClone(img);
		img = cvQueryFrame( input_video );
		if (img == NULL)
		{
			printf("video over.\n");
			break;
		}
        img_2 = (IplImage *) cvClone(img);

		if (IS_CALIBRATE)
		{
			calibrate(img_1);
			calibrate(img_2);
		}
		Mat color, gray;
		color.create(HEIGHT, WIDTH, CV_8UC3);
		gray.create(HEIGHT, WIDTH, CV_8UC1);

		IplImage *img_dst = cvCreateImage(cvSize(WIDTH, HEIGHT), img_2->depth , img_2->nChannels);
		cvResize(img_2, img_dst);
		
		float result = imgStrategic(funtype, img_1, img_2, img_dst, color, gray, strategic)/INT_FLOAT;
		//printf("result : %f\n", result);
		
		cvNamedWindow ("flow", 1);
		cvShowImage ("flow", img_dst);
		imshow("color",color);
		imshow("gray", gray);
		cvWriteFrame(writer, img_dst);
		writercolor.write(color);
		writergray.write(gray);

		int key_pressed = waitKey(1);
		if ( (char)key_pressed ==  27 )
		{
			break;
		}
	}
	cvDestroyAllWindows();
	cvReleaseImage(&img_1);
	cvReleaseImage(&img_2);
	cvReleaseCapture(&input_video);
	cvReleaseVideoWriter(&writer);
	writercolor.release();
	writergray.release();
}

void matVideo(MatFunType funtype, bool issf, int strategic, String inputfile){
	VideoCapture capture(inputfile.c_str());
	if (!capture.isOpened())
	{
		printf("Error: Can't open video.\n");
		exit(-1);
	}
	Mat frame_1, frame_2;

	VideoWriter writer("../video/flow_mat.avi",CV_FOURCC('M', 'J', 'P', 'G'),20,cvSize(WIDTH, HEIGHT),true);
	VideoWriter writercolor("../video/color_mat.avi",CV_FOURCC('M', 'J', 'P', 'G'),20,cvSize(WIDTH, HEIGHT),true);
	VideoWriter writergray("../video/gray_mat.avi",CV_FOURCC('M', 'J', 'P', 'G'),20,cvSize(WIDTH, HEIGHT),false); //false表示为灰度图

	while(true)
	{
		capture.read(frame_1);
		if (frame_1.empty())
		{
			printf("video over.\n");
			break;
		}
		capture.read(frame_2);
		if (frame_2.empty())
		{
			printf("video over.\n");
			break;
		}
		if (IS_CALIBRATE)
		{
			frame_1 = calibrate(frame_1);
			frame_2 = calibrate(frame_2);
		}

		Mat frame_dst, color, gray;
		resize(frame_2,frame_dst,Size(WIDTH,HEIGHT),0,0,1);
		color.create(HEIGHT, WIDTH, CV_8UC3);
		gray.create(HEIGHT, WIDTH, CV_8UC1);

		float result = matStrategic(funtype, frame_1, frame_2, frame_dst, color, gray, strategic, issf)/INT_FLOAT;
		//printf("result: %f\n", result);

		imshow("flow", frame_dst);
		imshow("color",color);
		imshow("gray",gray);
		writer.write(frame_dst);
		writercolor.write(color);
		writergray.write(gray);
		
        int key_pressed = waitKey(50);
		if ( (char)key_pressed ==  27 )
		{
			frame_dst.release();
			color.release();
			gray.release();
			break;
		}
        if ( (char)key_pressed ==  'p' )
        {
            waitKey(0);
        }
		frame_dst.release();
		color.release();
		gray.release();
	}
	capture.release();
	writer.release();
	writercolor.release();
	writergray.release();
	waitKey(0);
}

void imgFeatureVideo(ImgFeatureFunType funtype, int strategic, String inputfile){
	CvCapture *input_video = cvCreateFileCapture(inputfile.c_str());
	if (input_video == NULL)
	{
		printf("Error: Can't open video.\n");
		exit(-1);
	}
	IplImage *img = NULL, *img_1 = NULL, *img_2 = NULL;

	CvVideoWriter* writer = cvCreateVideoWriter("../video/flow_feature.avi",CV_FOURCC('P', 'I', 'M', '1'),20,cvSize(WIDTH, HEIGHT),1);
	VideoWriter writercolor("../video/color_feature.avi",CV_FOURCC('M', 'J', 'P', 'G'),20,cvSize(WIDTH, HEIGHT),true);

	while(true)
	{
		img = cvQueryFrame( input_video );
		if (img == NULL)
		{
		    printf("video over.\n");
			exit(-1);
		}
		img_1 = (IplImage *) cvClone(img);
		img = cvQueryFrame( input_video );
		if (img == NULL)
		{
			printf("video over.\n");
			exit(-1);
		}
		img_2 = (IplImage *) cvClone(img);
		IplImage *img_dst = cvCreateImage(cvSize(WIDTH, HEIGHT), img_2->depth , img_2->nChannels);
		cvResize(img_2, img_dst);

		Mat color;
		color.create(HEIGHT, WIDTH, CV_8UC3);

		float result = imgFeatureStrategic(funtype, img_1, img_2, img_dst, color, strategic)/INT_FLOAT;
		//printf("result: %f\n", result);
 
		cvNamedWindow ("flow", 1);
		cvShowImage ("flow", img_dst);
		imshow("color",color);
		cvWriteFrame(writer, img_dst);
		writercolor.write(color);

		int key_pressed = waitKey(1);
		if ( (char)key_pressed ==  27 )
		{
			cvReleaseImage(&img_dst);
			break;
		}
		cvReleaseImage(&img_dst);
	}
	cvDestroyAllWindows();
	cvReleaseImage(&img_1);
	cvReleaseImage(&img_2);
	cvReleaseCapture(&input_video);
	cvReleaseVideoWriter(&writer);
	writercolor.release();
}

void imgFlow(ImgFunType funtype,String imgprev, String imgcurr){
	IplImage* imgA=cvLoadImage(imgprev.c_str(), 1);
	IplImage* imgB=cvLoadImage(imgcurr.c_str(), 1);

	if (imgA == NULL)
	{
		printf("Image %s cannot be read\n", imgprev.c_str());
		exit(-1);
	}
	if (imgB == NULL)
	{
		printf("Image %s cannot be read\n", imgcurr.c_str());
		exit(1);
	}
	//imgBalance(funtype, imgA, imgB, imgA);
}

void matFlow(MatFunType funtype, String frameprev, String framecurr, bool issf){
	Mat frameA = imread(frameprev.c_str());
	Mat frameB = imread(framecurr.c_str());

	if (frameA.empty())
	{
		printf("Image %s cannot be read\n", frameprev.c_str());
		exit(-1);
	}
	if (frameB.empty())
	{
		printf("Image %s cannot be read\n", framecurr.c_str());
		exit(-1);
	}
	//matBalance(funtype, frameA, frameB, frameA, issf);
}

void imgFeatureFlow(ImgFeatureFunType funtype, String imgprev, String imgcurr){
	IplImage* imgA=cvLoadImage(imgprev.c_str(), 1);
	IplImage* imgB=cvLoadImage(imgcurr.c_str(), 1);

	if (imgA == NULL)
	{
		printf("Image %s cannot be read\n", imgprev.c_str());
		exit(-1);
	}
	if (imgB == NULL)
	{
		printf("Image %s cannot be read\n", imgcurr.c_str());
		exit(1);
	}
//	imgFeatureBalance(funtype, imgA, imgB, imgA);
}

void avoidMain(int argc, char* argv[]){
	if (argc == 2)
	{
		if (!strcmp(argv[1], "LK"))
		{
			imgVideo(Lucaskanade);
		}else if (!strcmp(argv[1], "HS"))
		{
			imgVideo(HornSchunck);
		}
		else if (!strcmp(argv[1], "BM"))
		{
			imgVideo(BlockMatch);
		}else if (!strcmp(argv[1], "PyrLK"))
		{
			imgFeatureVideo(PyrLK);
		}else if (!strcmp(argv[1], "FB")){
			matVideo(FarneBack);
		}else if (!strcmp(argv[1], "SF"))
		{
			matVideo(SimpleFlow, true);
		}
	}else if (argc == 3)
	{
		int strategic = atoi(argv[2]);
		if (!strcmp(argv[1], "LK"))
		{
			imgVideo(Lucaskanade, strategic);
		}else if (!strcmp(argv[1], "HS"))
		{
			imgVideo(HornSchunck, strategic);
		}
		else if (!strcmp(argv[1], "BM"))
		{
			imgVideo(BlockMatch, strategic);
		}else if (!strcmp(argv[1], "PyrLK"))
		{
			imgFeatureVideo(PyrLK, strategic);
		}else if (!strcmp(argv[1], "FB"))
		{
			matVideo(FarneBack, false, strategic);
		}else if (!strcmp(argv[1], "SF"))
		{
			matVideo(SimpleFlow, true, strategic);
		}
	}else if (argc == 4)
	{
		String path = "../video/";
		path += argv[3];
		int strategic = atoi(argv[2]);
		if (!strcmp(argv[1], "LK"))
		{
			imgVideo(Lucaskanade, strategic, path.c_str());
		}else if (!strcmp(argv[1], "HS"))
		{
			imgVideo(HornSchunck, strategic, path.c_str());
		}
		else if (!strcmp(argv[1], "BM"))
		{
			imgVideo(BlockMatch, strategic, path.c_str());
		}else if (!strcmp(argv[1], "PyrLK"))
		{
			imgFeatureVideo(PyrLK, strategic, path.c_str());
		}else if (!strcmp(argv[1], "FB"))
		{
			matVideo(FarneBack, false, strategic, path.c_str());
		}else if (!strcmp(argv[1], "SF"))
		{
			matVideo(SimpleFlow, true, strategic, path.c_str());
		}
	}
}
