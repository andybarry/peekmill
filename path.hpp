#ifndef PATH_HPP
#define PATH_HPP
#include <opencv2/core/core.hpp>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;

class Path{
	private:
		Point2f path_start_;
		float units_;
		vector<Point2f> coords_; //like java generics
	public:
		Point2f GetPathStart(){return path_start_;}
		float GetUnits(){return units_;}
		Mat Draw(Mat image_in);
		
		Path();//add parameters

	
};


#endif
