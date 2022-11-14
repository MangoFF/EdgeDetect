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
			for (int d = 1; d < divide; d++)
			{
				double angle = (PI / int(divide)) * d;
				double k = tanf(angle);
				//double distance = distanceBetweenPoints(center, cv::Point2d(j, i));
				//if (distance >= smallR && distance <= bigR)
				if (fabs(k * (i - center.x)- (j - center.y)) < 50*k/sqrtf(1+powf(k,2)))
					validMat.at<uchar>(i, j) = 255;
			}
		}
	return validMat;
}
int main()
{
	string path = "line4.png";
	Mat img = imread(path);
	Mat validMat = generateValidMat(img.size(), cv::Point2d(100, 100), 20, 2,5);
	imshow("Image", img);
	imshow("ValidMat", validMat);
	waitKey(0); //显示图片不会一闪而过
	return 0;
}

