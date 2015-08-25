#include "edge_detector.h"

Mat createCanny(Mat image_gray, char* window_name) {
	Mat dst, detected_edges;
	
	//normalized box filter
	blur(image_gray, detected_edges, Size(3, 3));

	Canny(detected_edges, detected_edges, OPT_CANNY_EDGE_THRESH, OPT_CANNY_EDGE_THRESH * OPT_CANNY_RATIO, OPT_CANNY_KERNEL_SIZE);

	dst = Scalar::all(0);
	image_gray.copyTo(dst, detected_edges);

	draw(dst, window_name);
	save(dst, window_name);
	
	return dst;
}

Mat createHarris(Mat image_gray, char* window_name) {
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(image_gray.size(), CV_32FC1);

	cornerHarris(image_gray, dst, OPT_HARRIS_BLOCK_SIZE, OPT_HARRIS_APERTURE_SIZE, OPT_HARRIS_K, BORDER_DEFAULT);

	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	draw(dst_norm_scaled, window_name);
	save(dst_norm_scaled, window_name);

	return dst_norm_scaled;
}