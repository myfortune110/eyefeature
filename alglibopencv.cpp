#include "alglibopencv.h"

//output a vector of points into a string format like [x1,x2,x3,...]
string vector_to_real_1d_string(vector<Point2d> v, int dimension) {
	if (dimension != 0 && dimension != 1) {
		return "Dimension error";
	}
	ostringstream out;
	out << "[";
	for (vector<Point2d>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin()) {
			out << ",";
		}

		// x
		if (dimension == 0) {
			out << it->x;
		}

		//y
		if (dimension == 1) {
			out << it->y;
		}
	}
	out << "]";
	return out.str();
}

// output format as [[x1],[x2],[x3],...]
string vector_to_real_2d_string(vector<Point2d> v, int dimension) {
	if (dimension != 0 && dimension != 1) {
		return "Dimension error";
	}
	ostringstream out;
	out << "[";
	for (vector<Point2d>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin()) {
			out << ",";
		}
		out << "[";
		// x
		if (dimension == 0) {			
			out << it->x;			
		}

		//y
		if (dimension == 1) {
			out << it->y;			
		}
		out << "]";
	}
	out << "]";
	return out.str();
}

real_1d_array vector_p2d_to_real_1d_array(vector<Point2d> pts, int dimension) {
	real_1d_array r1a(vector_to_real_1d_string(pts, dimension).c_str());
	return r1a;
}

real_2d_array vector_p2d_to_real_2d_array(vector<Point2d> pts, int dimension) {
	real_2d_array r2a(vector_to_real_2d_string(pts, dimension).c_str());
	return r2a;
}