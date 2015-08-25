#include "image_utils.h"

Mat get_channel(const Mat m, int chan) {
	Vec3b pixel;
	//initilize matrix with dimensions, 8bit unsigned char as its type
	Mat h(m.rows, m.cols, CV_8UC1);

	//copy specified channel to h
	for (int i = 0; i < m.cols; i++) {
		for (int j = 0; j < m.rows; j++) {
			pixel = m.at<Vec3b>(j, i);
			h.at<uchar>(j, i) = pixel.val[chan];
		}
	}
	return h;
}

vector<vector<Point>> new_contours(void) {
	vector<vector<Point>> contours;
	vector<Point> contour(1);
	contour.push_back(Point(0, 0));
	contours.reserve(CONTOUR_RESERVE);
	fill(contours.begin(), contours.end(), contour);
	return contours;
}

// input: H channel of the eye image
void get_contours(const Mat m, vector<vector<Point>>& contours){

	//draw_contours(m, contours, "Initialized");

	Mat m_thresh;
	vector<Vec4i> hierarchy;
	
	threshold(m, m_thresh, OPT_CONTOUR_THRESHOLD, 255, THRESH_BINARY);
	findContours(m_thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	//draw_contours(m, contours);
}