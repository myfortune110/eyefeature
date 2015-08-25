#include "draw.h"

void draw(const Mat m, const char* window){
	namedWindow(window, WINDOW_AUTOSIZE);
	imshow(window, m);
	waitKey(0);
	destroyWindow(window);
}

//use window name as file name
void save(const Mat m, const char* window){
	char file_name[100];
	strcpy_s(file_name, window);
	strcat_s(file_name, ".jpg");
	imwrite(file_name, m);
}

void draw_points(const Mat m, vector<Point2d> v, Scalar color, const char* window) {
	Mat m_copy = m.clone();
	for (vector<Point2d>::iterator it = v.begin(); it != v.end(); ++it) {
		//cout << "The "<< window << " point is at: " << it -> x << ", " << it -> y << endl;
		//border point is found
		if ( it -> x && it -> y) {
			circle(m_copy, *it, 3, color, 1, CV_AA, 0);
		}
	}
	
	draw(m_copy, window);
	save(m_copy, window);
}

void draw_circle(const Mat m, Point2d center, float radius, const char* window) {
	Mat m_copy = m.clone();
	//center
	circle(m_copy, center, 3, Scalar(181, 209, 71), -1, CV_AA, 0);
	//radius
	circle(m_copy, center, radius, Scalar(71, 209, 121), 1, CV_AA, 0);

	draw(m_copy, window);
	save(m_copy, window);
}

void draw_poly(Mat& image, vector<Point2d> p, const barycentricinterpolant bi) {
	Point2d c (0,0);

	//calculate all y values of the polynomial
	for (int i = 0; i < image.cols; i++) {
		c.x = i;
		c.y = barycentriccalc(bi, i);
		if (c.y >= 0 && c.y < image.rows) {
			circle(image, c, 1, Scalar(255, 255, 255), 1, 8, 0);
		}
	}

	//also put original points in pink
	for (vector<Point2d>::iterator it = p.begin(); it != p.end(); ++it) {
		circle(image, *it, 2, Scalar(255, 0, 255), -1, CV_AA, 0);
	}
}

void draw_polynomial(const Mat image, vector<Point2d> p, barycentricinterpolant bi, const char* window) {
	Mat image_out = image.clone();
	
	draw_poly(image_out, p, bi);	
	draw(image_out, window);
	save(image_out, window);
}

void draw_polynomials(const Mat image, vector<Point2d> p_u, vector<Point2d> p_l, barycentricinterpolant bi_u, barycentricinterpolant bi_l, const char* window) {
	Mat image_out = image.clone();
	draw_poly(image_out, p_u, bi_u);
	draw_poly(image_out, p_l, bi_l);
	draw(image_out, window);
	save(image_out, window);
}

void draw_contours(const Mat m, const vector<vector<Point>> contours, const char* window) {
	if (contours.size() <= 0) { return; }

	RNG rng(12345); //random number generation for color code
	//Mat drawing = Mat::zeros(m.size(), CV_8UC3);
	Mat drawing = m.clone();

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	//calculate
	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}

	//draw
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		circle(drawing, center[i], (int)radius[i], color, 2, CV_AA, 0);
	}

	draw(drawing, window);
	save(drawing, window);
}