#ifndef FILTER_H
#define FILTER_H

#include <cstdlib>
#include <cmath>
#include <opencv2/core/core.hpp>
#include "alglib/ap.h"
#include "alglib/interpolation.h"

using namespace cv;
using namespace alglib;
using namespace std;

#define RESERVED_VECTOR_SIZE 20 //20 points for curve fitting

//FIXME: some noisy points (i.e. x,y<0) need to be removed before further operation
template< typename T> vector<T> filter_points(vector<T> v) {
	vector<T> v_out;
	v_out.reserve(RESERVED_VECTOR_SIZE);
	for (vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it -> x >= 0 && it -> y >= 0) {
			v_out.push_back(*it);
		}
	}
	/* output
	for (vector<T>::iterator it = v_out.begin(); it != v_out.end(); ++it) {
		cout << "it->x: " << it->x << " " << "it->y:" << it->y <<endl;
	}
	*/
	return v_out;
}
vector<Point2d> filter_points_by_offset(vector<Point2d> v, barycentricinterpolant bi);

#endif