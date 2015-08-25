#ifndef IRIS_H
#define IRIS_H
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "draw.h"
#include "image_utils.h"
#include "edge_detector.h"
#include "filter.h"

#define EDGE_COLOR_THRESHOLD 60

using namespace std;
using namespace cv;

class iris {
	public:
		Point2f center;
		double radius;
		iris() {}
		iris(Point2f pt, double r) : center(pt), radius(r){}
		iris(float x, float y, double r) {
			center.x = x;
			center.y = y;
			radius = r;
		} 
		void operator= (const iris& ir) {
			center = ir.center;
			radius = ir.radius;
		}
};

vector<Point2f> scan_for_edge_points(const Mat m, iris ir); //pull out points on the iris border
vector<Point2f> scan_for_canny_edge_points(const Mat m, iris ir);
iris get_approx_iris(const vector<vector<Point>> contours); //circle of the biggest contour
iris solve_circle(vector<Point2f> vp);
iris calc_accurate_iris(vector<Point2f> edge_pts); //circle that covers the three points
iris get_iris(const Mat& m);
bool validate_iris(const iris approx, const iris calib);
#endif