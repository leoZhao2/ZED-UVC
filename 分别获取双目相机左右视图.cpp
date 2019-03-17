

#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<stdio.h>
using namespace cv;
using namespace std;
/*
int main(int argc, char* argv)
{
	Mat frame, left, right;
	frame = imread("orign.png");
	// Extract left and right images from side-by-side
	Mat left_image = frame(cv::Rect(0, 0, frame.cols / 2, frame.rows));
	Mat right_image = frame(cv::Rect(frame.cols / 2, 0, frame.cols / 2, frame.rows));
	// Display images    
	imshow("left", left_image);
	imshow("right", right_image);
	imwrite("left.png", left_image);//图片保存到本工程目录中
	imwrite("right.png", right_image);//图片保存到本工程目录中
	waitKey(0);
	return 0;
}


*/

//     USB zed捕获
int main(int argc, char* argv)
{
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -2;

	// Set the video resolution to HD1080
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 2560);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1440);
	char leftname[200];
	char rightname[200];
	int count = 0;
	while (1)
	{
		Mat frame, left, right;
		// Get a new frame from camera
		cap >> frame;
		// Extract left and right images from side-by-side
		Mat left_image = frame(cv::Rect(0, 0, frame.cols / 2, frame.rows));
		Mat right_image = frame(cv::Rect(frame.cols / 2, 0, frame.cols / 2, frame.rows));
		// Display images    
		//imshow("frame", frame);
		imshow("left", left_image);
		imshow("right", right_image);
		if (waitKey(50) == 27)
			break;//按ESC键退出程序
		if (waitKey(50) == 32)//按空格键进行拍照
		{
			sprintf_s(leftname, "L%d.jpg", count);
			sprintf_s(rightname, "R%d.jpg", count);
			imwrite(leftname,left_image);//图片保存到本工程目录中
			imwrite(rightname, right_image);//图片保存到本工程目录中
			count++;
		}
	}
	return 0;
}