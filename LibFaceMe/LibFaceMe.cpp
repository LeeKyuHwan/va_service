// LibFaceMe.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"

#include <iostream>
#include <memory>
#include <vector>

#include "LibFaceMe.h"
#include "shlobj.h"

#include "IFaceMeLicenseManager.h"
#include "IFaceMeRecognizer.h"
#include "IBaseObj.h"
#include "FaceMeBaseDef.h"
#include "FaceMeSDKLib.h"
#include "BaseObjUtils.h"

#define FR_INIT_STRUCT(ptr, class_name) { std::memset((uint8_t *)ptr, 0, sizeof(class_name)); (*ptr).sizeOfStructure = sizeof(class_name); }
#define HAS_OPTION(name, option) (((name) & (int32_t)(option)) == (int32_t)(option))

using namespace FaceMeSDK;
using namespace std;

uint8_t license_key[] = "HjX5sxIhPjy1LprIAyeV30A3omo7ws6jAyQrjPST";
FaceMeSDKLib* facemesdk_library = nullptr;
IFaceMeLicenseManager* license_manager = nullptr;
IFaceMeRecognizer* recognizer = nullptr;

string module_path = "YOUR_EXE_FOLDER_PATH"; // Fill in your exe file folder path.
string app_data_path = "C:\\ProgramData"; // Fill in your database saving path.
string app_cache_path = "C:\\Users\\username\\AppData\\Local"; // Fill in your local AppData path for cache files.

int init()
{
	char path[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);
	string fullPath = string(path);
	module_path = fullPath.substr(0, fullPath.rfind('\\'));
	cout << "module_path : " << module_path << endl;

	SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path);
	app_data_path = string(path);
	cout << "app_data_path : " << app_data_path << endl;

	SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path);
	fullPath = string(path);
	app_cache_path = fullPath.substr(0, fullPath.rfind('\\') + 1).append("Local");
	cout << "app_cache_path : " << app_cache_path << endl;

	// License verification.
	FR_RETURN fr_return = FR_RETURN_OK;
	FR_LicenseManagerConfig license_config;
	FR_INIT_STRUCT(&license_config, FR_LicenseManagerConfig);
	license_config.appBundlePath = module_path.c_str();
	license_config.appDataPath = app_data_path.c_str();
	license_config.appCachePath = app_cache_path.c_str();
	size_t key_length = strlen((char *)license_key);
	
	facemesdk_library = new FaceMeSDKLib();
	facemesdk_library->CreateFaceMeLicenseManager(&license_manager);
	fr_return = license_manager->Initialize(&license_config, license_key, (uint32_t)key_length);
	
	if (fr_return != FR_RETURN_OK)
		return fr_return;
	
	fr_return = license_manager->RegisterLicense(nullptr, nullptr);
	if (fr_return != FR_RETURN_OK)
		return fr_return;
	
	FR_RecognizerConfig recognizer_config;
	FR_INIT_STRUCT(&recognizer_config, FR_RecognizerConfig);
	recognizer_config.appBundlePath = module_path.c_str();
	recognizer_config.appDataPath = app_data_path.c_str();
	recognizer_config.appCachePath = app_cache_path.c_str();
	recognizer_config.preference = PREFER_NONE;
	recognizer_config.detectionModelSpeedLevel = FR_DETECTION_MODEL_SPEED_LEVEL_DEFAULT;
	recognizer_config.extractionModelSpeedLevel = FR_EXTRACTION_MODEL_SPEED_LEVEL_DEFAULT;
	recognizer_config.maxDetectionThreads = 0;
	recognizer_config.maxExtractionThreads = 0;
	recognizer_config.maxFrameWidth = 1920; // Depends on input image resolution
	recognizer_config.maxFrameHeight = 1080; // Depends on input image resolution
	recognizer_config.minFaceWidth = 60;
	recognizer_config.detectOutputOrder = FR_DETECTION_OUTPUT_ORDER_FACE_WIDTH;
	recognizer_config.preferredDetectionBatchSize = 4;
	recognizer_config.preferredExtractionBatchSize = 4;
	
	facemesdk_library->CreateFaceMeRecognizer(&recognizer);
	fr_return = recognizer->Initialize(&recognizer_config, license_key, (uint32_t)key_length);
	if (fr_return != FR_RETURN_OK)
		return fr_return;
	
	// Set up extraction options.
	uint32_t min_face_width = 60;
	recognizer->SetExtractionOption(FR_EXTRACTION_OPTION_MINFACEWIDTH, min_face_width);
	recognizer->SetExtractionOption(FR_EXTRACTION_OPTION_DETECTION_OUTPUT_ORDER, FR_DETECTION_OUTPUT_ORDER_FACE_WIDTH);

	return fr_return;
}

void destroy()
{
	if (facemesdk_library != nullptr)
	{
		delete facemesdk_library;
		facemesdk_library = nullptr;
	}
}

int enroll(void* bgr24, int width, int height, void* oFir)
{
	FR_Image fr_image;
	FR_INIT_STRUCT(&fr_image, FR_Image);
	fr_image.pixelFormat = FR_PIXEL_FORMAT_BGR;
	fr_image.channel = 3;
	fr_image.width = width;
	fr_image.height = height;
	fr_image.stride = fr_image.width * fr_image.channel;
	fr_image.data = bgr24;

	// Set up extraction configurations.
	FR_ExtractConfig extract_config;
	FR_INIT_STRUCT(&extract_config, FR_ExtractConfig);
	extract_config.extractOptions = FR_OPT(FR_FEATURE_OPTION_FULL_FEATURE);

	std::vector<uint32_t> face_numbers(1);
	FR_RETURN fr_return = recognizer->ExtractFace(&extract_config, &fr_image, 1, face_numbers.data());
	if (fr_return != FR_RETURN_OK)
		return 0;

	if (face_numbers[0] == 0)
		return 0;

	vector<FR_FaceFeature> face_recognized_features(face_numbers[0]);
	FR_INIT_STRUCT(&face_recognized_features[0], FR_FaceFeature);
	fr_return = recognizer->GetFaceFeature(0, 0, &face_recognized_features[0]);
	
	uint32_t firSize = face_recognized_features[0].featureData.size * sizeof(float);
	memcpy_s(oFir, firSize, (void*)face_recognized_features[0].featureData.data, firSize);

	return (int)firSize;
}

float verify(void* fir1, void* fir2, int firSize)
{
	FR_FaceFeature firA, firB;
	FR_INIT_STRUCT(&firA, FR_FaceFeature);
	FR_INIT_STRUCT(&firB, FR_FaceFeature);

	firA.featureType = 8;
	firA.featureSubType = 2;
	firA.featureData.size = (uint32_t)firSize / sizeof(float);
	memcpy_s((void*)firA.featureData.data, firSize, fir1, firSize);

	firB.featureType = 8;
	firB.featureSubType = 2;
	firB.featureData.size = (uint32_t)firSize / sizeof(float);
	memcpy_s((void*)firB.featureData.data, firSize, fir2, firSize);

	float score = 0.0f;
	recognizer->CompareFaceFeature(&firA, &firB, &score);

	return score;
}