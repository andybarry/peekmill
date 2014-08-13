/*
 * Utility functions for opencv-stereo
 *
 * Copyright 2013, Andrew Barry <abarry@csail.mit.edu>
 *
 */
 
#ifndef PEEK_MILL_UTIL
#define PEEK_MILL_UTIL

#include "opencv2/legacy/legacy.hpp"
#include "opencv2/opencv.hpp"

#include <stdio.h>


using namespace std;
using namespace cv;


struct OpenCvCalibration
{
    Mat M1;
    Mat D1;
};

bool LoadCalibration(OpenCvCalibration *calibration);

//void Draw3DPointsOnImage(Mat camera_image, vector<Point3f> *points_list_in, Mat cam_mat_m, Mat cam_mat_d, Mat cam_mat_r, int color = 128, float x=0, float y=0, float z=0);

#endif
