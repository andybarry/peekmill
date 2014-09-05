#include "path.hpp"


Path::Path(){
	coords_.push_back(Point2f(0,000));
	coords_.push_back(Point2f(1,100));
	coords_.push_back(Point2f(2,200));
	coords_.push_back(Point2f(3,200));
	coords_.push_back(Point2f(4,200));
	coords_.push_back(Point2f(5,200));

}

Mat Path::Draw(Mat image_in){
	Mat image_out;
	image_in.copyTo(image_out);

	for(unsigned int i=0; i<coords_.size()-1;i++){
		cout << "woo" << endl;
		line(image_out, coords_.at(i), coords_.at(i+1), Scalar(156,0,255));
	}
	return image_out;
}
