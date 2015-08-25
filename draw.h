#ifndef DRAW_H
#define DRAW_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "alglib/ap.h"
#include "alglib/interpolation.h"

using namespace cv;
using namespace std;
using namespace alglib;

/* Note that variable names and its default value is set within header other than its source */
void draw(const Mat m, const char* window);
void save(const Mat m, const char* window);
void draw_points(const Mat m, vector<Point2d> v, Scalar color = Scalar(255,255,255), const char* window="Points");
void draw_circle(const Mat image, Point2d center, float radius, const char* window="Circle");
void draw_poly(Mat& image, vector<Point2d> p, const barycentricinterpolant bi);
void draw_polynomial(const Mat image, vector<Point2d> p, barycentricinterpolant bi, const char* window = "Polynomial");
void draw_polynomials(const Mat image, vector<Point2d> p_u, vector<Point2d> p_l, barycentricinterpolant bi_u, barycentricinterpolant bi_l, const char* window = "Intersection");
void draw_contours(const Mat m, const vector<vector<Point>> contours, const char* window="Contours");
#endif