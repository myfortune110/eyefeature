#ifndef EYELIDS_H
#define EYELIDS_H

#include <iostream>
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "alglib/ap.h"
#include "alglib/interpolation.h"

#include "filter.h"
#include "edge_detector.h"
#include "fitting.h"
#include "draw.h"

using namespace cv;
using namespace alglib;
using namespace std;

vector<Point2d> bounding_points(const Mat m);
vector<Point2d> upper_eyelid(const Mat m);
vector<Point2d> lower_eyelid(const Mat m);
complex_1d_array poly_crossing(barycentricinterpolant b1, barycentricinterpolant b2); //intersection of two polynomials
vector<Point2d> get_eyelid_corners(const Mat m); //pull out two canthi

#endif