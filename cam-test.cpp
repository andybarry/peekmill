#include "FlyCapture2.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include "peek-mill-utils.hpp"

using namespace FlyCapture2;

static void onMouse(int event, int x, int y, int nothing, void* nothing2)
{
    // we're writing a mouse handler! so cool!
    if (event != EVENT_LBUTTONDOWN)
        return;
    cout << "you clicked the mouse! (" << x << ", " << y << ")!" << endl;
    //obvs this needs to be bound to main at some point to do something actually useful

}

int main() {
    
    // load camera parameters
    
    OpenCvCalibration calibration;
    
    LoadCalibration(&calibration);
    
    cout << calibration.M1 << calibration.D1 << endl;
    
    Error error;
    Camera camera;
    CameraInfo camInfo;

    // Connect the camera
    error = camera.Connect( 0 );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to connect to camera" << std::endl;     
        return false;
    }
    
    // Get the camera info and print it out
    error = camera.GetCameraInfo( &camInfo );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to get camera info from camera" << std::endl;     
        return false;
    }
    std::cout << camInfo.vendorName << " "
              << camInfo.modelName << " " 
              << camInfo.serialNumber << std::endl;
    
    error = camera.StartCapture();
    if ( error == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED )
    {
        std::cout << "Bandwidth exceeded" << std::endl;     
        return false;
    }
    else if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to start image capture" << std::endl;     
        return false;
    } 
    
    // capture loop
    char key = 0;
    namedWindow("image_ud");
    setMouseCallback("image_ud", onMouse, 0);

    while(key != 'q')
    {
        // Get the image
        Image rawImage;
        Error error = camera.RetrieveBuffer( &rawImage );
        if ( error != PGRERROR_OK )
        {
            std::cout << "capture error" << std::endl;
            continue;
        }
        
        // convert to rgb
        Image rgbImage;
        rawImage.Convert( FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage );
       
        // convert to OpenCV Mat
        unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();       
        cv::Mat image = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(),rowBytes);
        
        // use calibration to undistort image
        Mat image_ud;
        undistort(image, image_ud, calibration.M1, calibration.D1);

	// nadya starts writing stuff maybe this is what is broken
	vector<Point3f> world_points;
	Point3f xyz(1,2,3);
	world_points.push_back(xyz);
	world_points.push_back(Point3f(1,2,3));

	vector<Point3f> rvec;
	Point3f rvec_points(0,0,0);
	rvec.push_back(rvec_points);
	// note we're using this for both rvec and tvec bc we're lazy

	vector<Point2f> image_points;

	projectPoints(world_points, rvec, rvec, calibration.M1, calibration.D1, image_points);
	cout << image_points << endl;
	
	// here we are going to start drawing stuff in the image
	circle(image_ud, image_points.at(0),100, Scalar(156,0,255));
        
        
        //cv::Mat flipped_image;
        
        //cv::flip(image_ud, flipped_image, -1);
        
        cv::imshow("image", image);
        cv::imshow("image_ud", image_ud);
        key = cv::waitKey(30);        
    }
    
    error = camera.StopCapture();
    if ( error != PGRERROR_OK )
    {
        // This may fail when the camera was removed, so don't show 
        // an error message
    }  
    
    camera.Disconnect();
    
    return 0;
}
