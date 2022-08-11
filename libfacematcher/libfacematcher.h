#pragma once

extern "C" {
	__declspec(dllexport) bool init();
	__declspec(dllexport) void destroy();
	__declspec(dllexport) int getFeatureDimension();
	__declspec(dllexport) int detect(void* data, int width, int height);
	__declspec(dllexport) void getFaceInfo(int index, void* pBox);
	__declspec(dllexport) void featExtractFromIndex(int index, float vec[]);
	__declspec(dllexport) float matching(float enroll[], float query[]);
}
