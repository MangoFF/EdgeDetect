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


int thresh_otsu(Mat input,Mat mask)
{
	int histogram[256] = { 0 };
	double global_mean = 0;
	int num_valid = 0;
	for (int i = 1; i < input.rows; i++)
	{
		uchar *data = input.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++)
		{
			if (mask.at<uchar>(i, j) > 127)
			{
				num_valid++;
				histogram[data[j]]++;
				global_mean += data[j];
			}
		}
	}
	//global_mean /= (input.rows * input.cols);
	global_mean /= (num_valid);
	int sum = 0;
	double p1 = 0, p2 = 0, m1 = 0, m2 = 0;
	double sg = 0;
	double temp_sg = -1;
	int k = 0;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j <= i; j++)
		{
				p2 += histogram[j];
				m2 += (histogram[j] * j);
		}
		m2 /= p2;
		for (int j = i + 1; j < 256; j++)
		{
			p1 += histogram[j];
			m1 += (histogram[j] * j);
		}
		m1 /= p1;
		p2 /= (num_valid);
		p1 = 1 - p2;
		sg = p1 * p2*(m1 - m2)*(m1 - m2);
		if (sg > temp_sg)
		{
			temp_sg = sg;
			k = i;
		}
		p1 = 0, p2 = 0, m1 = 0, m2 = 0;
	}
	//    cout<<"k = "<<k<<endl;
	return k;
}
Mat generateValidMat(cv::Size size, cv::Point2d center, double r, double magnification,double divide) {
	if (divide > 30)
	{
		divide = 30;
	}
	double smallR = r / magnification, bigR = r * magnification;
	Mat res=Mat::zeros(size, CV_8UC1);
	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{
			for (int d = 0; d < divide; d++)
			{
				double angle = (PI / int(divide)) * d;
				double k = tanf(angle);
				double distance = distanceBetweenPoints(center, cv::Point2d(j, i));
				if (distance >= smallR && distance <= bigR)
					if (fabs(k * (i - center.y) - (j - center.x)) < 100/ divide * sqrtf(1 + powf(k, 2)))
						res.at<uchar>(i, j) = 255;
			}
		}
	}
	return res;
}
vector<Point2d> getCircle(Mat &src_org, cv::Point2d center, double r, double magnification, double divide)
{
	Mat valid = generateValidMat(src_org.size(), center, r, magnification, divide);
	imshow("Image_valid", valid);
	Mat src_gray;
	cvtColor(src_org, src_gray, COLOR_RGB2GRAY);
	bitwise_and(src_gray, valid, src_gray);
	int thresh = thresh_otsu(src_gray, valid);
	threshold(src_gray, src_gray, thresh, 255, CV_THRESH_BINARY);
	imshow("Image_otsu", src_gray);


	int g_nStructElementSize = 1;    //结构元素（内核矩阵）的尺寸
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * g_nStructElementSize + 1, 2 * g_nStructElementSize + 1), Point(g_nStructElementSize, g_nStructElementSize));
	erode(src_gray, src_gray, element);
	dilate(src_gray, src_gray, element);
	imshow("Image_otsu_erode_dialate", src_gray);

	// find contours
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	findContours(src_gray, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE); //提取轮廓

	vector<Point2d> res;
	// draw find result                                                 
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (contourArea(contours[i]) > 25)
		{
			//绘制第i条轮廓
			drawContours(src_org, contours, (int)i, Scalar(0, 0, 255), 1, 8, hierarchy, 0);
			int min_p = 0;
			int min_dis = 10000000000;
			for (int j = 0; j < contours[i].size(); j++)
			{
				if (distanceBetweenPoints(center, cv::Point2f(contours[i][j].x, contours[i][j].y)) < min_dis)
				{
					min_dis = distanceBetweenPoints(center, cv::Point2f(contours[i][j].x, contours[i][j].y));
					min_p = j;
				}
			}
			circle(src_org, contours[i][min_p], 5, Scalar(0, 255, 0));
			res.push_back(contours[i][min_p]);
		}

	}
	return res;
}
int main()
{
	string path = "circle.png";
	Mat img_org = imread(path);
	imshow("Image_org", img_org);
	getCircle(img_org, cv::Point2d(415, 150), 75, 2, 5);
	imshow("Img_org", img_org);
	waitKey(0); //显示图片不会一闪而过
	return 0;
}


