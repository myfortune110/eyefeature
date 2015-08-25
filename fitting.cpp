#include "fitting.h"

lsfitreport linear_fit(vector<Point2d> v) {
	lsfitreport rep;
	ae_int_t info;
	real_1d_array c;
	real_2d_array fmatrix = vector_p2d_to_real_2d_array(v, 0);
	real_1d_array y = vector_p2d_to_real_1d_array(v, 1);

	lsfitlinear(y, fmatrix, info, c, rep);

	/*
	printf("%d\n", int(info)); // EXPECTED: 1 - task solved
	printf("%s\n", c.tostring(4).c_str()); 
	*/
	return rep;
}

barycentricinterpolant poly_fit(vector<Point2d> v) {
	ae_int_t m = 3; //parabola
	ae_int_t info;
	barycentricinterpolant p;
	polynomialfitreport rep;

	real_1d_array x, y;
	x = vector_p2d_to_real_1d_array(v, 0);
	y = vector_p2d_to_real_1d_array(v, 1);

	polynomialfit(x, y, m, info, p, rep);
	return p;
}

// interpolation on general grids
// output : barycentric interpolant to calculate polynomial value
barycentricinterpolant interpolate_polynomials(vector<Point2d> v) {
	barycentricinterpolant bi;

	real_1d_array x, y; //initialize general grids
	x = vector_p2d_to_real_1d_array(v, 0);
	y = vector_p2d_to_real_1d_array(v, 1);
	
	polynomialbuild(x, y, bi);
	return bi;
}