#pragma once

#include <stdio.h>
#include <cstring>
#include "FaceMeSDKLib.h"
#include "fileutils.h"
#include "strutils.h"
#include "opencv2/opencv.hpp"
#include "Common.h"
#include "BaseObjUtils.h"

using namespace FaceMeSDK;
using namespace cl_utility;
using namespace cl_utility::fileutils;

typedef struct ParseArg
{
	bool        is_id				= false;
    std::string path				= "C:\\images\\a.jpg";
    std::string path2				= "C:\\images\\a1.jpg";
	std::string fdm					= "DEFAULT";
	std::string frm					= "DEFAULT";
	bool        is_log				= false;
	uint32_t    thread				= 0;
	uint32_t    min_face			= 112;
	std::string far_word			= "1E-5";
	std::string chip				= "NV";
	std::string extract_feature		= "FULL";

} ParseArg;

class MatchHandler
{
public:
	MatchHandler();
	~MatchHandler();

	FR_RETURN Initialize();
	float Verify(void* fir1, void* fir2, int firSize);
	float Match(ParseArg args);
	int Enroll(void* image, int size, void* oFir);
    std::string GetErrorMsg(FR_RETURN hr);

private:
    FR_RETURN GetFeatureByPath(std::string path, FR_FaceFeature& feature);

    FR_RETURN Match(std::string path, std::string path2, float& confidence);
	bool DecodeImage(std::string path, cv::Mat& image_mat);

	std::string app_bundle_path_;
	std::string app_data_path_;
	std::string app_cache_path_;
	float current_threshold_;
	bool is_masked_feature_;

	std::shared_ptr<FaceMeSDKLib> spFaceMeSDKLib_;
	FaceMeSDK::BaseObject::scoped_refptr<FaceMeSDK::IFaceMeLicenseManager> spLicenseManager_;
	FaceMeSDK::BaseObject::scoped_refptr<FaceMeSDK::IFaceMeRecognizer> spRecognizer_;
};

