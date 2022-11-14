#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
//计算两个点之间的距离
#define PI 3.14
double distanceBetweenPoints(cv::Point2f p1, cv::Point2f p2) {
	cv::Vec2f v = p1 - p2;
	return sqrt(v.dot(v));
}
cv::Mat generateValidMat(cv::Size size, cv::Point2d center, double r, double magnification,double divide) {
	double smallR = r / magnification, bigR = r * magnification;

	cv::Mat validMat = cv::Mat::zeros(size, CV_8UC1);
	for (int i = 0; i < size.height; i++)
		for (int j = 0; j < size.width; j++)
		{
			for (int d = 0; d < divide; d++)
			{
				double angle = (PI / int(divide)) * d;
				double k = tanf(angle);
				double distance = distanceBetweenPoints(center, cv::Point2d(j, i));
				if (distance >= smallR && distance <= bigR)
				if (fabs(k * (i - center.x)- (j - center.y)) <10*sqrtf(1+powf(k,2)))
					validMat.at<uchar>(i, j) = 255;
			}
		}
	return validMat;
}
int main()
{
	string path = "circle.png";
	Mat img = imread(path);
	cvtColor(img, img, COLOR_RGB2GRAY);
	cv::Mat sobelX;//水平方向结果
	cv::Mat sobelY;//垂直方向结果
	cv::Mat sobel;
	cv::Sobel(img, sobelX, CV_16S, 1, 0);
	cv::Sobel(img, sobelY, CV_16S, 0, 1);
	sobel = abs(sobelX) + abs(sobelY);//计算L1范数

	Mat validMat = generateValidMat(img.size(), cv::Point2d(175, 175), 75, 2,5);
	bitwise_and(img, validMat, img);
	threshold(img, img, 100, 255, CV_THRESH_BINARY);
	imshow("Image", img);
	imshow("ValidMat", validMat);
	waitKey(0); //显示图片不会一闪而过
	return 0;
}


