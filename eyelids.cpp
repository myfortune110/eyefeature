#include "eyelids.h"

/*
* input : binary Canny result image
* output: four corner points in a vector
* these bounding points are simply calculated
* the very first pixel found to be non zero in each direction
*/
vector<Point2d> bounding_points(const Mat m) {
	vector<Point2d> pts(4);
	//topmost point
	for (int i = 0; i < m.rows; i++){
		for (int j = 0; j < m.cols; j++) {
			//cout << "Pixel: " << j << ", "<< i << " "<< (int)m.at<uchar>(i,j) << endl;
			if ((int)m.at<uchar>(i, j) > 0) {
				pts[0].x = j;
				pts[0].y = i;
				break;
			}
		}
	}

	//bottommost point
	for (int i = m.rows - 1; i >= 0; i--) {
		for (int j = 0; j < m.cols; j++) {
			//cout << "Pixel: " << j << ", " << i << " " << (int)m.at<uchar>(i, j) << endl;
			if ((int)m.at<uchar>(i, j) > 0) {
				pts[1].x = j;
				pts[1].y = i;
				break;
			}
		}
	}

	//leftmost point
	for (int j = 0; j < m.cols; j++) {
		for (int i = 0; i < m.rows; i++) {
			//cout << "Pixel: " << j << ", " << i << " " << (int)m.at<uchar>(i, j) << endl;
			if ((int)m.at<uchar>(i, j) > 0) {
				pts[2].x = j;
				pts[2].y = i;
				break;
			}
		}
	}

	//rightmost point
	for (int j = m.cols-1; j >=0; j--) {
		for (int i = 0; i < m.rows; i++) {
			//cout << "Pixel: " << j << ", " << i << " " << (int)m.at<uchar>(i, j) << endl;
			if ((int)m.at<uchar>(i, j) > 0) {
				pts[3].x = j;
				pts[3].y = i;
				break;
			}
		}
	}

	return pts;
}

vector<Point2d> upper_eyelid(const Mat m) {
	int spacing = (int)((float)m.cols/(float)RESERVED_VECTOR_SIZE);
	vector<Point2d> pts(RESERVED_VECTOR_SIZE-1);
	int value=0;
	//fix cases when source image too small
	if (m.cols > spacing) {
		for (int i = 0; i < RESERVED_VECTOR_SIZE-1; i++) {
			for (int j = 0; j < m.rows; j++) {
				if (value = (int)m.at<uchar>(j, (i+1)*spacing)) {
					pts[i].x = (i + 1)*spacing;
					pts[i].y = j;
					break;
				}
			}
			//edge not found on this column
			if(!value){
				pts[i].x = -1;
				pts[i].y = -1;
			}
		}
	}
	return pts;
}

vector<Point2d> lower_eyelid(const Mat m) {
	int spacing = (int)((float)m.cols / (float)RESERVED_VECTOR_SIZE);
	vector<Point2d> pts(RESERVED_VECTOR_SIZE - 1);
	int value = 0;
	//FIXME: cases when source image is too small
	if (m.cols > spacing) {
		for (int i = 0; i < RESERVED_VECTOR_SIZE - 1; i++) {
			for (int j = m.rows -1 ; j >= 0 ; j--) {
				if (value = (int)m.at<uchar>(j, (i + 1)*spacing)) {
					pts[i].x = (i + 1)*spacing;
					pts[i].y = j;
					break;
				}
			}
			//edge not found on this column
			if (!value) {
				pts[i].x = -1;
				pts[i].y = -1;
			}
		}
	}
	return pts;
}

// modified version to get lower side of upper eyelid
vector<Point2d> upper_eyelid_m(const Mat m) {
	int spacing = (int)((float) m.cols / (float) RESERVED_VECTOR_SIZE);
	vector<Point2d> pts(RESERVED_VECTOR_SIZE - 1);
	int value = 0;
	//fix cases when source image too small
	if (m.cols > spacing) {
		for (int i = 0; i < RESERVED_VECTOR_SIZE - 1; i++) {
			for (int j = m.rows / 2 ; j < m.rows; j++) { //scan from middle to top
				if (value = (int)m.at<uchar>(j, (i + 1)*spacing)) {
					pts[i].x = (i + 1)*spacing;
					pts[i].y = j;
					break;
				}
			}
			//edge not found on this column
			if (!value) {
				pts[i].x = -1;
				pts[i].y = -1;
			}
		}
	}
	return pts;
}

vector<Point2d> lower_eyelid_m(const Mat m) {
	int spacing = (int)((float)m.cols / (float)RESERVED_VECTOR_SIZE);
	vector<Point2d> pts(RESERVED_VECTOR_SIZE - 1);
	int value = 0;
	//FIXME: cases when source image is too small
	if (m.cols > spacing) {
		for (int i = 0; i < RESERVED_VECTOR_SIZE - 1; i++) {
			for (int j = m.rows/2 - 1; j >= 0; j--) { //scan from middle to bottom
				if (value = (int)m.at<uchar>(j, (i + 1)*spacing)) {
					pts[i].x = (i + 1)*spacing;
					pts[i].y = j;
					break;
				}
			}
			//edge not found on this column
			if (!value) {
				pts[i].x = -1;
				pts[i].y = -1;
			}
		}
	}
	return pts;
}

complex_1d_array poly_crossing(barycentricinterpolant b1, barycentricinterpolant b2) {
	real_1d_array pow_coeff_1, pow_coeff_2;
	real_1d_array pow_equation;
	
	polynomialbar2pow(b1, pow_coeff_1);
	polynomialbar2pow(b2, pow_coeff_2);

	pow_equation.setlength(pow_coeff_1.length());

	//create equation by equaling y
	for (int i = 0; i < pow_coeff_1.length(); i++) {
		pow_equation[i] = pow_coeff_1[i] - pow_coeff_2[i];
		//cout << "The element " << i << " is : " << pow_equation[i] << endl;
	}

	complex_1d_array x;
	polynomialsolverreport rep;
	//cout << "coefficient length " << pow_coeff_1.length() << endl;

	//caution : polynomial degree is lower than its length
	polynomialsolve(pow_equation, pow_coeff_1.length()-1, x, rep);
	
	//should be all real since intersection do exists for upper lids and lower lids
	//it might fail if the fitting parabola is incorrect so that there is no intersection between the two
	cout << "The abscissas of points of intersection are at " << x.tostring(4).c_str() << endl;
	return x;
}

void print_points(vector<Point2d> v) {
	for (vector<Point2d>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it->x >= 0 && it->y >= 0) {
			cout << it->x << ", " << it->y << endl;
		}
	}
}

vector<Point2d> get_eyelid_corners(const Mat m) {
	Mat m_gray, m_canny;
	cvtColor(m, m_gray, CV_BGR2GRAY);
	m_canny = createCanny(m_gray);

	//pull out rough lids points from Canny result
	//vector<Point2d> upperlids = upper_eyelid_m(m_canny);
	//vector<Point2d> lowerlids = lower_eyelid_m(m_canny);
	vector<Point2d> upperlids = upper_eyelid(m_canny);
	vector<Point2d> lowerlids = lower_eyelid(m_canny);

	//print_points(lowerlids);

	draw_points(m, upperlids, Scalar(255, 0, 0), "upperlid");
	draw_points(m, lowerlids, Scalar(255, 255, 0), "lowerlid");

	//filter out points that are invalid (not found, marked as -1)
	vector<Point2d> pts_upper = filter_points(upperlids);
	vector<Point2d> pts_lower = filter_points(lowerlids);

	//polynomial fitting (parabola)
	barycentricinterpolant bi_u = poly_fit(pts_upper);
	barycentricinterpolant bi_l = poly_fit(pts_lower);

	//filter again by its offset from parabola
	vector<Point2d> pts_upper_new = filter_points_by_offset(pts_upper, bi_u);
	vector<Point2d> pts_lower_new = filter_points_by_offset(pts_lower, bi_l);

	//curve fitting again
	barycentricinterpolant bi_u_new = poly_fit(pts_upper_new);
	barycentricinterpolant bi_l_new = poly_fit(pts_lower_new);

	draw_polynomials(m, pts_upper_new, pts_lower_new, bi_u_new, bi_l_new);

	//calculate point of intersection
	complex_1d_array crossing = poly_crossing(bi_u_new, bi_l_new);

	vector<Point2d> crossing_v;
	crossing_v.reserve(RESERVED_VECTOR_SIZE);
	//calculate crossing y value by its x
	for (int i = 0; i < crossing.length(); i++) {
		Point2d c(crossing[i].x, barycentriccalc(bi_u_new, crossing[i].x));
		crossing_v.push_back(c);
	}

	return crossing_v;
}