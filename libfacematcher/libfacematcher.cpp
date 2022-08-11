// libfacematcher.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "libfacematcher.h"
#include "faceDetector.h"
#include "faceMatcher.h"

faceMatcher* fm = NULL;
faceDetector* fd = NULL;

bool init()
{
	try
	{
		fm = new faceMatcher();
		fd = new faceDetector();

		if (fm->init() == false)
		{
			printf("facematcher init failed\n");
			return false;
		}
		
		if (fd->init() == false)
		{
			printf("facedetector init failed\n");
			return false;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

int getFeatureDimension()
{
	return FEATURE_DIMENSION;
}

void destroy()
{
	try
	{
		fm->destroy();
		delete fm;
		fm = NULL;

		fd->destroy();
		delete fd;
		fd = NULL;
	}
	catch (...)
	{
	}
}

int detect(void* data, int width, int height)
{
	try
	{
		cv::Mat image = cv::Mat(height, width, CV_8UC3, data);
		IplImage img = image;
		int faces = fd->faceDetect(&img);
		image.release();

		return faces;
	}
	catch (...)
	{
		return 0;
	}
}

void getFaceInfo(int index, void* pBox)
{
	*((BBOX*)pBox) = fd->GetFeatureInfo(index).bbox;
}

void featExtractFromIndex(int index, float vec[])
{
	try
	{
		cout << 1 << endl;
		cv::Mat roi = fd->GetFaceImage(index, false);
		cout << 2 << endl;
		fm->featExtract(roi, vec);
		cout << 3 << endl;
		roi.release();
		cout << 4 << endl;
	}
	catch (...)
	{
	}
}

float matching(float enroll[], float query[])
{
	try
	{
		return fm->matching(enroll, query, FEATURE_DIMENSION) / 100.0f;
	}
	catch (...)
	{
		return 0;
	}
}