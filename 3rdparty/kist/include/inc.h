#pragma once
#include <vector>
#include <string>
#include <math.h>
#include <stdio.h>

#include "opencv2/opencv.hpp"

#pragma comment(lib, "FaceDetector.lib")
#pragma comment(lib, "FaceMatcher.lib")
#pragma comment(lib, "opencv_core2411.lib")
#pragma comment(lib, "opencv_highgui2411.lib")
#pragma comment(lib, "opencv_imgproc2411.lib")

#define FEATURE_DIMENSION	256

using namespace std;

typedef struct _pt{
	int x;
	int y;
}POINT2D;

typedef struct _bbox{
	int x;
	int y;
	int width;
	int height;
}BBOX;

typedef struct _faceinfo{
	POINT2D l_eye;
	POINT2D r_eye;
	POINT2D nose;
	POINT2D l_lip;
	POINT2D r_lip;
	BBOX bbox;
}FEATURE_INFO;
