#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "iris.h"

/*
 * function : probe three points that lie on the edge of the circle defined by (center, radius)
 * input : center, radius, binary image (H channel)
 * output: a vector of three points
 */
vector<Point2f> scan_for_edge_points(const Mat m, iris ir) {
	vector<Point2f> edge_point(3);
	fill(edge_point.begin(), edge_point.end(), Point2f(0.0, 0.0));

	if (m.cols < ir.radius) return edge_point; //error input
	int radius_threshold = 10;
	int color_threshold = 100;
	float delta = 0.0;
	float avg = 0.0;
	int color = 0;
	int cnt = 0;
	//scan for right edge
	for (int i = (int)ir.center.x; i < (int)ir.center.x + (int)ir.radius + radius_threshold; i++) {
		//if (avg) avg = (float)color;
		color = m.at<uchar>((int)ir.center.y, i);
		//cout << "color: " << color << endl;
		
		delta = abs(avg - color);
		avg = (cnt*avg + color) / (cnt + 1); //update
		cnt++;
		//cout << " Delta : "<< delta << endl;
		if (delta >= color_threshold) {
			edge_point.at(0).x = i;
			edge_point.at(0).y = (int)ir.center.y;
			break;
		}
	}
	avg = 0.0; //reset
	cnt = 0;
	//scan left
	for (int i = ir.center.x; i > ir.center.x - (int)ir.radius - radius_threshold; i--) {
		color = m.at<uchar>((int)ir.center.y, i);
		//cout << "color: " << color << endl;

		delta = abs(avg - color);
		avg = (cnt*avg + color) / (cnt + 1); //update
		cnt++;
		//cout << " Delta : " << delta << endl;
		if (delta >= color_threshold) {
			edge_point.at(1).x = i;
			edge_point.at(1).y = ir.center.y;
			break;
		}
	}
	
	avg = 0.0;
	int move = 3;
	cnt = 0;
	//scan left for the third point to fix the circle (move up the center for a little bit)
	for (int i = ir.center.x; i > ir.center.x - (int)ir.radius - radius_threshold; i--) {
		color = m.at<uchar>(ir.center.y + move, i);
		//cout << "color: " << color << endl;
		delta = abs(avg - color);
		avg = (cnt*avg + color) / (cnt + 1); //update
		cnt++;
		//cout << " Delta : " << delta << endl;
		if (delta >= color_threshold) {
			edge_point.at(2).x = i;
			edge_point.at(2).y = ir.center.y + move;
			break;
		}
	}
	return edge_point;
}

/*
* input should be Canny result, approximate iris descriptor
* output is a group of three points lie on iris circle
*/
vector<Point2f> scan_for_canny_edge_points(const Mat m, iris ir) {
	vector<Point2f> edge_point(4);
	fill(edge_point.begin(), edge_point.end(), Point2f(-1.0, -1.0));
	//int color;
	int color_threshold = EDGE_COLOR_THRESHOLD;
	int cirle_shift = 4;

	//scan right from center
	for (int i = (int)ir.center.x; i < m.cols; i++) {
		if (m.at<uchar>((int)ir.center.y, i) >= color_threshold) {
			edge_point.at(0).x = i;
			edge_point.at(0).y = (int)ir.center.y;
			break;
		}
	}

	//san left from center
	for (int i = (int)ir.center.x; i >= 0; i--) {
		if (m.at<uchar>((int)ir.center.y, i) >= color_threshold) {
			edge_point.at(1).x = i;
			edge_point.at(1).y = (int)ir.center.y;
			break;
		}
	}

	//san top from center
	for (int i = (int)ir.center.y; i >= 0 ; i--) {
		if (m.at<uchar>(i, (int)ir.center.x) >= color_threshold) {
			edge_point.at(2).x = (int)ir.center.x;
			edge_point.at(2).y = i;
			break;
		}
	}

	//scan down from center
	for (int i = (int)ir.center.y; i < m.rows; i++) {
		if (m.at<uchar>(i, (int)ir.center.x) >= color_threshold) {
			edge_point.at(3).x = (int)ir.center.x;
			edge_point.at(3).y = i;
			break;
		}
	}

	return edge_point;
}

bool validate_iris(const iris approx, const iris calib) {
	float delta = abs(calib.radius - approx.radius);
	if (delta / approx.radius <= 0.5) {
		return true;
	}
	else {
		cout << "The new calculated circle is way larger than the approximate one: " << delta/approx.radius <<  endl;
		return false;
	}
}

/*
 * input : set of contours, original color image
 * output: approximate iris (selected because it would make the biggest contour)
 */
 
iris get_approx_iris(const vector<vector<Point>> contours) {
	double radius_max = 0.0;
	Point2f center_max(0.0, 0.0);
	iris approx_iris;
	approx_iris.center = center_max;
	approx_iris.radius = radius_max;
	
	if (contours.size() <= 0) return approx_iris;
	
	/// Approximate contours to polygons + get bounding circles
	vector<vector<Point>> contours_poly(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}

	//find the biggest contour and it is most likely the eye	
	for (size_t i = 0; i < radius.size(); i++) {
		if (!radius_max) {
			radius_max = radius[i];
			center_max = center[i];
		}
		if (radius_max < radius[i]) {
			radius_max = radius[i];
			center_max = center[i];
		}
	}

	approx_iris.radius = radius_max;
	approx_iris.center = center_max;

	cout << "The approximate eye center is at " << (int)center_max.x << ", " << (int)center_max.y << " with the radius of " << radius_max << endl;
	//_CrtDumpMemoryLeaks();
	return approx_iris;
}

// input : three points lie on the iris
iris solve_circle(vector<Point2f> vp) {
	double radius = 0.0;
	Point2f center(0.0, 0.0);
	iris ir(center, radius);
	if (vp.size() < 3) {
		return ir; 
	}
	
	float x1, x2, x3, y1, y2, y3;
	x1 = vp.at(0).x;
	x2 = vp.at(1).x;
	x3 = vp.at(2).x;
	y1 = vp.at(0).y;
	y2 = vp.at(1).y;
	y3 = vp.at(2).y;

	// Test if two points coincide
	if (((x1 == x2) && (y1 == y2)) || ((x1 == x3) && (y1 == y3)) || ((x2 == x3) && (y2 == y3)))
		return ir;  

	// Test if they're on hor/ver line
	if (((x1 == x2) && (x1 == x3)) || ((y1 == y2) && (y2 == y3)))
		return ir;  

	// Test if they're colinear
	if ((y2 - y1) / (x2 - x1) == (y3 - y2) / (x3 - x2))
		return ir;  


	float k1=0.0, b1=0.0, k2=0.0, b2=0.0;
	int flag_horizontal_1 = 0, flag_horizontal_2 = 0;

	// Negative reciprocal, Point-slope on the midpoint of AB
	if (y1 != y2) {
		k1 = (x1 - x2) / (y2 - y1);  
		b1 = (y1 + y2) / 2.0 - k1 * (x1 + x2) / 2.0; 
	}
	else {
		flag_horizontal_1 = 1;
		b1 = y1;
	}

	if (y2 != y3) {
		k2 = (x2 - x3) / (y3 - y2);
		b2 = (y2 + y3) / 2.0 - k2 * (x2 + x3) / 2.0;
	}
	else {
		flag_horizontal_2 = 1;
		b2 = y2;
	}
	
	//crossing of two lines
	if (flag_horizontal_1 || flag_horizontal_2) {
		center.x = flag_horizontal_1 * (x1 + x2) / 2 + flag_horizontal_2 * (x2 + x3);
		center.y = flag_horizontal_1 * (k1 * center.x + b1) + flag_horizontal_2 * (k2 * center.x + b2);
	} else {
		center.x = (b2 - b1) / (k1 - k2); 
		center.y = k1 * center.x + b1;
	}

	radius = sqrt((center.x - x1) * (center.x - x1) + (center.y - y1) * (center.y - y1));
	
	ir.center = center;
	ir.radius = radius;
	return ir;
}

iris calc_accurate_iris(vector<Point2f> edge_pts) {
	iris ir;
	float final_radius = 0.0;
	Point2f final_center(0.0, 0.0);

	//print edge points coordinates
	for (vector<Point2f>::iterator it = edge_pts.begin(); it != edge_pts.end(); ++it) {
		cout << "X: " << it->x << ", Y: " << it->y << endl;
	}

	//get circle out of three points
	minEnclosingCircle(edge_pts, final_center, final_radius);
	ir.center = final_center;
	ir.radius = final_radius;

	return ir;
}

iris get_iris(const Mat& m) {
	//convert to hsv model and pull out H channel
	Mat m_hsv, m_gray, m_h, m_canny, m_thresh, m_harris;
	cvtColor(m, m_hsv, CV_BGR2HSV);
	cvtColor(m, m_gray, CV_BGR2GRAY);
	m_h = get_channel(m_hsv, 0);
	blur(m_h, m_h, Size(3, 3));

	m_harris = createHarris(m_gray);

	iris ir (0.0, 0.0, 0.0);

	vector<vector<Point>> contours = new_contours();

	//get_contours(m_h, contours);
	vector<Vec4i> hierarchy;

	threshold(m_h, m_thresh, OPT_CONTOUR_THRESHOLD, 255, THRESH_BINARY);
	findContours(m_thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	if (contours.size() <= 0) {
		cerr << "No contours found, exit program" << endl;
		return ir;
	}

	double radius_max = 0.0;
	Point2f center_max(0.0, 0.0);
	iris approx_iris(center_max, radius_max);

	/// Approximate contours to polygons + get bounding circles
	vector<vector<Point>> contours_poly(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}

	//find the biggest contour and it is most likely the eye	
	for (size_t i = 0; i < radius.size(); i++) {
		if (!radius_max) {
			radius_max = radius[i];
			center_max = center[i];
		}
		if (radius_max < radius[i]) {
			radius_max = radius[i];
			center_max = center[i];
		}
	}

	approx_iris.radius = radius_max;
	approx_iris.center = center_max;

	cout << "The approximate eye center is at " << (int)center_max.x << ", " << (int)center_max.y << " with the radius of " << radius_max << endl;
	draw_circle(m, center_max, radius_max, "Approximate Circle");

	float final_radius = 0.0;
	Point2f final_center(0.0, 0.0);

	//draw(threshold_output, "Threshold binary");
	//save(threshold_output, "Threshold binary");

	m_canny = createCanny(m_gray);
	vector<Point2f> edge_pts = scan_for_canny_edge_points(m_canny, approx_iris);
	edge_pts = filter_points(edge_pts);
	vector<Point2d> edge_pts_d(edge_pts.size());

	//print edge points coordinates (converting to int?)
	int i = 0;
	for (vector<Point2f>::iterator it = edge_pts.begin(); it != edge_pts.end(); ++it) {
		cout << "Edge points found at X: " << it->x << ", Y: " << it->y << endl;
		edge_pts_d[i].x = (int) it->x;
		edge_pts_d[i].y = (int)it->y;
		i++;
	}
	
	//get circle out of three points
	if(edge_pts.size()>=3) {
		//ir = solve_circle(edge_pts);
		minEnclosingCircle(edge_pts, final_center, final_radius);
		ir.center = final_center;
		ir.radius = final_radius;
		validate_iris(approx_iris, ir);
		cout << "The final eye center is at " << (int)ir.center.x << ", " << (int)ir.center.y << " with the radius of " << ir.radius << endl;
		draw_circle(m, ir.center, ir.radius, "Iris");
	}
	else {
		cout << "We need at least three points to draw an accurate circle." << endl;
	}
	
	draw(m_h, "H channel");
	save(m_h, "H channel");
	draw_points(m, edge_pts_d, Scalar(37, 250, 253), "Probed edge points");
	draw_contours(m, contours);
	return ir;
}