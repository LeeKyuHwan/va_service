#pragma once

extern "C" {
	__declspec(dllexport) int init();
	__declspec(dllexport) void destroy();
	__declspec(dllexport) int enroll(void* imageData, int size, void* oFir);
	__declspec(dllexport) float verify(void* fir1, void* fir2, int firSize);
}