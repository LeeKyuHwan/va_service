#pragma once

extern "C" {
	__declspec(dllexport) int init();
	__declspec(dllexport) void destroy();
	__declspec(dllexport) int enroll(void* bgr24, int width, int height, void* oFir);
	__declspec(dllexport) float verify(void* fir1, void* fir2, int firSize);
}