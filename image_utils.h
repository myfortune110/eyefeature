#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "draw.h"

#define OPT_CONTOUR_THRESHOLD 130
#define CONTOUR_RESERVE 50

using namespace cv;
using namespace std;

Mat get_channel(const Mat m, int chan);
vector<vector<Point>> new_contours(void);
void get_contours(const Mat m, vector<vector<Point>>& contours);

#endif