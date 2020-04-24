
#include "opencv2/opencv.hpp"
#include "CvxText.h" 

using namespace cv;
 
int main(int argc, char *argv[])
{
	//Mat img = imread("lena.jpg", 0);
	IplImage *pstImg = cvLoadImage("lena.jpg", 0);
	CvxText text("msyh.ttf");	//载入希望输入的字体，可在目录C:\WINDOWS\Fonts中选择相应字体文件，并复制到工程下。
	 
	Scalar size(15, 0.5, 0.1, 0);	//设置字体的大小/空白比例/间隔比较/旋转角度
	float p = 1.f; //设置字体的透明度
	const char *msg = "@OpenCV中文输出测试！";
	 
	text.setFont(NULL, &size, NULL, &p);
	text.putText(img, msg, Point(140, 250), CV_RGB(0,255,0));
	 
	
	Mat img = pstImg;  /* IplImage转化为Mat */
	imwrite("1.bmp", img);
 
	return 0;
}