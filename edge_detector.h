#ifndef EDGE_DETECTOR_H
#define EDGE_DETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "draw.h"

#define OPT_CANNY_EDGE_THRESH 	70
#define OPT_CANNY_RATIO 		2
#define OPT_CANNY_KERNEL_SIZE	3

#define OPT_HARRIS_BLOCK_SIZE 2
#define OPT_HARRIS_APERTURE_SIZE 3
#define OPT_HARRIS_K 0.04

Mat createCanny(Mat image_gray, char* window_name = "Canny");
Mat createHarris(Mat image_gray, char* window_name = "Harris");
#endif