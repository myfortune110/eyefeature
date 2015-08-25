#include "filter.h"

vector<Point2d> filter_points_by_offset(vector<Point2d> v, barycentricinterpolant bi) {
	vector<Point2d> v_out;
	v_out.reserve(RESERVED_VECTOR_SIZE);
	double offset_percentage = 0.05;
	double offset_threshold = 8.0; //5 pixels seems okay already

	//FIXME: might be better if average distance is calculated and offset percentage is introduced
	// * currently it only drops points offset by threshold pixels
	for (vector<Point2d>::iterator it = v.begin(); it != v.end(); ++it) {
		if (abs(it->y - barycentriccalc(bi, it->x)) <= offset_threshold) {
			v_out.push_back(*it);
		}
	}
	return v_out;
}