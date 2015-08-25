#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "eyelids.h"
#include "iris.h"
#include "draw.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
	if (argc != 2)
	{
		cout << " Usage: eye_feature.exe path_to_img" << endl;
		return -1;
	}

	Mat image, image_gray, image_hsv, image_res, image_gray_gauss;

	image = imread(argv[1], IMREAD_COLOR); // color images are stored in BRG order

	if (!image.data)
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}	

	vector<Point2d> corners = get_eyelid_corners(image);
	
	draw_points(image, corners, Scalar(255, 0, 0), "Canthus");

	iris ir = get_iris(image);

	return 0;
}