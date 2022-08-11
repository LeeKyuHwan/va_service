#pragma once
class faceMatcher
{
public:
	bool init();
	void destroy();	
	void featExtract(cv::Mat src, float vec[]);
	float matching(float enroll[], float query[], int dim);	
};

