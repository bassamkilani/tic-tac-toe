#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;

int main() {
	Mat original;
	//original = imread("C:\\Users\\bassa\\OneDrive\\Image Processing\\hw-3\\Test Cases\\board-perfect.png");
	//original = imread("C:\\Users\\bassa\\OneDrive\\Image Processing\\hw-3\\Test Cases\\Test-1.jpg");
	original = imread("C:\\Users\\bassa\\OneDrive\\Image Processing\\hw-3\\Test Cases\\Test-2.jpg");
	//original = imread("C:\\Users\\bassa\\OneDrive\\Image Processing\\hw-3\\Test Cases\\Test-3.jpg");
	Mat gray_image;
	Mat thresholded;
	Mat blurred;
	Mat eroded;
	Mat dilated;
	resize(original, original, Size(), 0.35, 0.35);
	//imwrite("C:\\Users\\bassa\\OneDrive\\Image Processing\\hw-3\\Test Cases\\resized.jpg", original);
	cvtColor(original, gray_image, CV_BGR2GRAY);
	GaussianBlur(gray_image, blurred, Size(1,1), 0,0,BORDER_DEFAULT);
	//medianBlur(blurred, blurred, 3);
	//adaptiveThreshold(gray_image, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 255, 0);
	threshold(blurred, thresholded, 70, 255, 0);
	GaussianBlur(thresholded, thresholded, Size(1, 1), 0, 0, BORDER_DEFAULT);
	adaptiveThreshold(thresholded, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 255, 0);
	bitwise_not(thresholded, thresholded, noArray());
	erode(thresholded, eroded, Mat(), Point(-1, -1), 1, 1, BORDER_DEFAULT);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(thresholded, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); // finds contours on bin image

	vector<cv::Point> approx;
	Scalar color(0, 0, 255);
	Point p;
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;
	double area = 0;
	double ar;
	Point pArea;
	Point pArea2;
	for (int i = 0; i< contours.size(); i++) // to find largest area. 
	{
		pArea = contours[i][0];
		drawContours(original, contours, i, color, CV_FILLED, 8, hierarchy);
		if (contourArea(contours[i], false) > area) {
			area = contourArea(contours[i], false);
			pArea2 = contours[i][0];
		}
	}

	Point p1, p2, p3, p4;
	for (int i = 0; i< contours.size(); i++) // iterate through each contour. 
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		if ((contourArea(contours[i], false)) == area && approx.size() == 4) {
			p1 = approx[0];
			p2 = approx[1];
			p3 = approx[2];
			p4.x = approx[3].x;
			p4.y = approx[3].y;
			//drawContours(original, contours, i, color, CV_FILLED, 8, hierarchy);
		}
	}
	double tmpy, tmpx, tmpyh, tmpxh;
	tmpy = p1.y;
	if (p2.y < tmpy)
		tmpy = p2.y;
	if (p3.y < tmpy)
		tmpy = p3.y;
	if (p4.y < tmpy)
		tmpy = p4.y;

	tmpx = p1.x;
	if (p2.y < tmpx)
		tmpx = p2.x;
	if (p3.x < tmpx)
		tmpx = p3.x;
	if (p4.x < tmpx)
		tmpx = p4.x;

	tmpyh = p1.y;
	if (p2.y > tmpyh)
		tmpyh = p2.y;
	if (p3.y > tmpyh)
		tmpyh = p3.y;
	if (p4.y > tmpyh)
		tmpyh = p4.y;

	tmpxh = p1.x;
	if (p2.y > tmpxh)
		tmpxh = p2.x;
	if (p3.x > tmpxh)
		tmpxh = p3.x;
	if (p4.x > tmpxh)
		tmpxh = p4.x;

	double h = abs(tmpy - tmpyh);
	double w = abs(tmpx - tmpxh);
	gray_image = gray_image(Rect(tmpx, tmpy, w, h));
	//imwrite("C:\\Users\\bassa\\OneDrive\\Image Processing\\hw-3\\Test Cases\\sliced.jpg", gray_image);
	GaussianBlur(gray_image, blurred, Size(3,3), 0,0,BORDER_DEFAULT);
	medianBlur(blurred, blurred, 3);
	adaptiveThreshold(blurred, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 555, 0);
	bitwise_not(thresholded, thresholded, noArray());
	erode(thresholded, thresholded, Mat(), Point(-1, -1), 1, 1, BORDER_DEFAULT);
	//cvtColor(gray_image, gray_image, CV_GRAY2BGR);

	vector<Vec3f> circles;
	double tmp = gray_image.rows / 8;
	/// Apply the Hough Transform to find the circles
	HoughCircles(gray_image, circles, CV_HOUGH_GRADIENT, 1, gray_image.rows / 8, 50, 50, 0, 0);
	cvtColor(gray_image, gray_image, CV_GRAY2BGR);
	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		putText(gray_image, "Circle", center, fontface, scale, CV_RGB(255, 0, 0), thickness, 8);
		//// circle center
		//circle(thresholded, center, 3, Scalar(255, 255, 255), -1, 8, 0);
		//// circle outline
		circle(thresholded, center, radius + 15, Scalar(0, 0, 0), CV_FILLED, 8, 0);
	}

	//erode(thresholded, thresholded, Mat(), Point(-1, -1), 1, 1, BORDER_DEFAULT);

	double rr;
	Point ppp;
	findContours(thresholded, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); // finds contours on bin image
	for (int i = 0; i< contours.size(); i++) // to find largest area. 
	{
		Point tmpP = contours[i][0];
		rr = contourArea(contours[i], false);
		if (contourArea(contours[i], false) < area && contourArea(contours[i], false) > 50 && tmpP.x > 0 && tmpP.y > 0) {
			area = contourArea(contours[i], false);
			ppp = contours[i][0];
		}
	}

	for (int i = 0; i < contours.size(); i++) {
		if ((contourArea(contours[i], false)) < area + 1200 && (contourArea(contours[i], false)) > area - 1200 && (contourArea(contours[i], false)) > 100 && (contourArea(contours[i], false)) < 10000) {
		drawContours(gray_image, contours, i, color, CV_FILLED, 8, hierarchy);
		Point pp = contours[i][0];
		if (pp.x > 0 && pp.y > 0)
			putText(gray_image, "Cross", pp, fontface, scale, CV_RGB(0, 0, 255), thickness, 8);
	}
	}

	//imshow("Thresholded", thresholded);
	imshow("Tic-Tac-Toe", gray_image);
	//imshow("]", original);
	waitKey(0);
	return 0;
}