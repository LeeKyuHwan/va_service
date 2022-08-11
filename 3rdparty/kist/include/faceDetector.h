#pragma once
#include "inc.h"

class faceDetector
{
public:		
	bool init();
	void destroy();	
	int faceDetect(IplImage* src);
	FEATURE_INFO GetFeatureInfo(int faceIdx);
	cv::Mat GetFaceImage(int faceIdx, bool alignment);
};

