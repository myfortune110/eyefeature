/*
* library for conversion between OpenCV data structures and alglib datastures 
*/

#ifndef ALGLIBOPENCV_H
#define ALGLIBOPENCV_H

#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include "alglib/ap.h"
#include "alglib/interpolation.h"
#include "alglib/stdafx.h"

using namespace alglib;
using namespace cv;
using namespace std;

string vector_to_real_1d_string(vector<Point2d> pts, int dimension);
string vector_to_real_2d_string(vector<Point2d> pts, int dimension);


//1D Mat (x) to real_1d_array 
real_1d_array vector_p2d_to_real_1d_array(vector<Point2d> pts, int dimension);

//2D Mat (x,y) to real_1d_array - get only x
real_2d_array vector_p2d_to_real_2d_array(vector<Point2d> pts, int dimension);


#endif