#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <string>

using namespace cv;

/*
* This class extracts the square ROI of the given palm print image
*/


// Struct for holding the keypoints (valley points between fingers)
struct Keypoints {
	Point centerPoint;
	Point keypoint1;
	Point keypoint2;
	Point keypoint3;
	bool success;
};

class RoiExtraction
{

private:

	/*Cropped image size from the center of the original image*/
	const int CENTER_SIZE = 20;

	/*Original image*/
	Mat inputImage;

	/*Cropped image from the center of the original image*/	
	Mat centerOfImage;
		
	/*
	* Crops the center of the original image (using @CENTER_SIZE range)
	* and convert it to YCbCr color model
	*/
	Mat cropCenterOfInputImage();

	/* Apply region growing algorithm */
	Mat applyRegionGrowing();

	/* Get keypoints (valley points between fingers) from the segmented image */
	Keypoints findKeypoints(const Mat &segmentedImage);

	/* Calculate and draw the square ROI based on the keypoints */
	void calcAndDrawSquareRoi(const Keypoints &keypoints);



public:
	const std::string TAG = "RoiExtraction";
	/* Extracted suqre ROI based on the keypoints */
	Mat squareRoi;
	
	/* Return the extracted square ROI */
	Mat getSquareRoi();

	RoiExtraction(Mat image);

	~RoiExtraction();
};

