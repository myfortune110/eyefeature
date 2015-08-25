#ifndef FITTING_H
#define FITTING_H

#include <opencv2/core/core.hpp>
#include "alglib/ap.h"
#include "alglib/interpolation.h"
#include "alglibopencv.h"

using namespace cv;
using namespace alglib;

lsfitreport linear_fit(vector<Point2d> v);
barycentricinterpolant poly_fit(vector<Point2d> v);
barycentricinterpolant interpolate_polynomials(vector<Point2d> v);

#endif