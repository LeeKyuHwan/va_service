// LibFaceMe.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"

#include <iostream>
#include "LibFaceMeSDK.h"
#include "MatchHandler.h"

using namespace std;

MatchHandler* match = nullptr;

int init()
{
	match = new MatchHandler();
	return match->Initialize();
}

void destroy()
{
	if (match != nullptr)
	{
		delete match;
		match = nullptr;
	}
}

int enroll(void* imageData, int size, void* oFir)
{
	return match->Enroll(imageData, size, oFir);
}

float verify(void* fir1, void* fir2, int firSize)
{
	return match->Verify(fir1, fir2, firSize);
}