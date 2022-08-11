#include "MatchHandler.h"
#include "FaceMeSDKHelper.h"
#include "imageutils.h"

using namespace cl_log;


MatchHandler::MatchHandler()
    : app_bundle_path_("")
    , app_data_path_("")
    , app_cache_path_("")
    , current_threshold_(0.0)
{
	_TCHAR module_name[MAX_PATH] = {};

#ifdef _WIN32
	GetModuleFileName(NULL, module_name, MAX_PATH);
	std_tstring module_path = ch_GetFilePath(module_name);
	_TCHAR short_module_path[MAX_PATH] = {};
	GetShortPathName(module_path.c_str(), short_module_path, _countof(short_module_path));
#else
	readlink("/proc/self/exe", module_name, MAX_PATH);
	std_tstring module_path = ch_GetFilePath(module_name);
	const _TCHAR* short_module_path = module_path.c_str();
#endif

	// Setup root directory path of user data and database.
	Path app_data_path(GetAppDataDir(""));

	// Setup root directory path for license and cache.
	Path app_cache_path(GetUserCacheDir(""));

	app_bundle_path_ = Utf8String(short_module_path);
	app_data_path_ = app_data_path.str();
	app_cache_path_ = app_cache_path.str();
}

MatchHandler::~MatchHandler()
{
	if (spLicenseManager_ != nullptr)
	{
		spLicenseManager_->Finalize();
		spLicenseManager_ = nullptr;
		LOGPRINTF("Finalize license manager successfully.\n");
	}

	if (spRecognizer_ != nullptr)
	{
		spRecognizer_->Finalize();
		spRecognizer_ = nullptr;
		LOGPRINTF("Finalize recognizer successfully.\n");
	}

	spFaceMeSDKLib_.reset();
}

FR_RETURN MatchHandler::Initialize()
{
	ParseArg args;

	is_masked_feature_ = iequals(args.extract_feature, "MASK");
    cl_log::log_flag = args.is_log;
	LOGPRINTF("App bundle path: %s\n", app_bundle_path_.c_str());
	LOGPRINTF("App data path: %s\n", app_data_path_.c_str());
	LOGPRINTF("App cache path: %s\n", app_cache_path_.c_str());

	FR_RETURN fr_return = FR_RETURN_OK;
	std::string license_key;
	std::string license_path;
	if (!LoadLicenseKey(license_key, license_path))
	{
		LOGPRINTF("Fail to find/load license.\n");
		return FR_RETURN_LICENSE_FAIL;
	}
	uint32_t len = (uint32_t)(license_key.size() * sizeof(license_key[0]));

	std::shared_ptr<FaceMeSDKLib> spFaceMeSDKLib = std::make_shared< FaceMeSDKLib>(app_bundle_path_.c_str());
	if (!spFaceMeSDKLib)
		return FR_RETURN_NOT_INIT;

	// Create new instance of FaceMe License Manager
	BaseObject::scoped_refptr<IFaceMeLicenseManager> spLicenseManager;
	if (!spFaceMeSDKLib->CreateFaceMeLicenseManager(&spLicenseManager.p))
	{
		LOGPRINTF("Fail to create license manager.\n");
		return FR_RETURN_NOT_INIT;
	}

	// Setup related path locations for initialization of FaceMe License Manager. 
	FR_LicenseManagerConfig licenseManager_config;
	FR_INIT_STRUCT(&licenseManager_config, FR_LicenseManagerConfig);
	licenseManager_config.appBundlePath = app_bundle_path_.c_str();
	licenseManager_config.appDataPath = app_data_path_.c_str();
	licenseManager_config.appCachePath = app_cache_path_.c_str();

	// Initialize FaceMe License Manager.
	fr_return = spLicenseManager->Initialize(&licenseManager_config, (const uint8_t *)license_key.c_str(), (uint32_t)license_key.length());
	if (FR_FAILED(fr_return))
	{
		LOGPRINTF("Fail to initialize license manager. Error code: %d\n", fr_return);
		return fr_return;
	}

	fr_return = spLicenseManager->RegisterLicense(nullptr, nullptr);
	if (FR_FAILED(fr_return))
	{
		LOGPRINTF("Fail to register license. Error code: %d\n", fr_return);
		return fr_return;
	}
	// Create new instance of FaceMe Face Recognizer
	BaseObject::scoped_refptr<IFaceMeRecognizer> spRecognizer;
	if (!spFaceMeSDKLib->CreateFaceMeRecognizer(&spRecognizer.p))
	{
		LOGPRINTF("Fail to create FaceMeRecognizer.\n");
		return FR_RETURN_NOT_INIT;
	}

	FR_PREFERENCE preference = PREFER_NONE;
	if (args.chip == "NV")
		preference |= FR_OPT(PREFER_HARDWARE_DETECTION) | FR_OPT(PREFER_HARDWARE_EXTRACTION);
	else if (args.chip == "Movidius")
		preference |= FR_OPT(PREFER_INTEL_MOVIDIUS_VPU_DETECTION) | FR_OPT(PREFER_INTEL_MOVIDIUS_VPU_EXTRACTION);


	// Setup FaceMe Recognizer.
	FR_RecognizerConfig recognizer_config;
	FR_INIT_STRUCT(&recognizer_config, FR_RecognizerConfig);
	recognizer_config.appBundlePath = app_bundle_path_.c_str();
	recognizer_config.appDataPath = app_data_path_.c_str();
	recognizer_config.appCachePath = app_cache_path_.c_str();
	recognizer_config.preference = preference;
	recognizer_config.detectionModelSpeedLevel = ConvertDetectInfo(args.fdm);
	recognizer_config.extractionModelSpeedLevel = ConvertExtractInfo(args.frm);
	recognizer_config.maxDetectionThreads = args.thread;
	recognizer_config.maxExtractionThreads = args.thread;
	recognizer_config.mode = FR_RECOGNIZER_MODE_IMAGE;

	fr_return = spRecognizer->Initialize(&recognizer_config, (const uint8_t *)license_key.c_str(), (uint32_t)license_key.length());
	if (FR_FAILED(fr_return))
	{
		LOGPRINTF("Fail to initialize recognizer. Error code: %d\n", fr_return);
		return fr_return;
	}

	spRecognizer->SetExtractionOption(FR_EXTRACTION_OPTION_MINFACEWIDTH, args.min_face);
	spRecognizer->SetExtractionOption(FR_EXTRACTION_OPTION_DETECTION_OUTPUT_ORDER, FR_DETECTION_OUTPUT_ORDER_FACE_WIDTH);

	spFaceMeSDKLib_ = spFaceMeSDKLib;
	spLicenseManager_ = spLicenseManager;
	spRecognizer_ = spRecognizer;
	
	return FR_RETURN_OK;
}

float MatchHandler::Match(ParseArg args)
{
	float confidence = 0.0f;
	FR_RETURN fr_return = Match(args.path, args.path2, confidence);

	return fr_return == FR_RETURN_OK ? confidence : 0.0f;
}

float MatchHandler::Verify(void* fir1, void* fir2, int firSize)
{
	float confidence = 0.0f;
	FR_RETURN fr_return = FR_RETURN_FAIL;

	FR_FaceFeature firA, firB;
	FR_INIT_STRUCT(&firA, FR_FaceFeature);
	FR_INIT_STRUCT(&firB, FR_FaceFeature);

	firA.featureType = firB.featureType = 8;
	firA.featureSubType = firB.featureSubType = 2;
	firA.featureData.size = firB.featureData.size = (uint32_t)firSize / sizeof(float);
	
	memcpy_s((void*)firA.featureData.data, firSize, fir1, firSize);
	memcpy_s((void*)firB.featureData.data, firSize, fir2, firSize);

	fr_return = spRecognizer_->CompareFaceFeature(&firA, &firB, &confidence);

	return fr_return == FR_RETURN_OK ? confidence : 0.0f;
}

int MatchHandler::Enroll(void* imageData, int size, void* oFir)
{
	cv::Mat image_mat = cv::imdecode(cv::Mat(1, size, CV_8UC1, imageData), cv::IMREAD_COLOR);

	FR_Image image;
	ToFrImage(image, image_mat);
	uint32_t image_num = 1;
	uint32_t face_num = 1;
	FR_RETURN fr_return = FR_RETURN_OK;
	FR_ExtractConfig extract_config;
	FR_INIT_STRUCT(&extract_config, FR_ExtractConfig);
	extract_config.extractOptions = (!is_masked_feature_) ? FR_FEATURE_OPTION_FULL_FEATURE : FR_FEATURE_OPTION_MASKED_FEATURE;
	spRecognizer_->SetExtractionOption(FR_EXTRACTION_OPTION_DETECTION_OUTPUT_ORDER, FR_DETECTION_OUTPUT_ORDER_FACE_WIDTH);
	while (1)
	{
		fr_return = spRecognizer_->ExtractFace(&extract_config, &image, image_num, &face_num);
		if (fr_return != FR_RETURN_RECOGNIZE_IMAGE_PROCESSING_EXCEEDED)
			break;
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (FR_FAILED(fr_return))
	{
		printf("ExtractFace Failed. Error code: %d\n", fr_return);
		return fr_return;
	}

	FR_FaceFeature feature;
	FR_INIT_STRUCT(&feature, FR_FaceFeature);

	fr_return = spRecognizer_->GetFaceFeature(0, 0, &feature);
	if (FR_FAILED(fr_return))
	{
		printf("GetFaceFeature Failed. Error code: %d\n", fr_return);
		return fr_return;
	}

	void* pFir = feature.featureData.data;
	int firSize = feature.featureData.size * sizeof(float);

	memcpy_s(oFir, firSize, pFir, firSize);

	return firSize;
}

// private

FR_RETURN MatchHandler::GetFeatureByPath(std::string path, FR_FaceFeature& feature)
{
	CheckParameter(spRecognizer_ == nullptr, FR_RETURN_NOT_INIT);

	cv::Mat image_mat;
	if (!DecodeImage(path, image_mat))
	{
		printf("Open file failed\n");
		return FR_RETURN_NOT_SUPPORT;
	}

	CheckParameter(image_mat.data == nullptr, FR_RETURN_NOT_SUPPORT);

    FR_Image image;
    ToFrImage(image, image_mat);
    uint32_t image_num = 1;
    uint32_t face_num = 1;
	FR_RETURN fr_return = FR_RETURN_OK;
    FR_ExtractConfig extract_config;
    FR_INIT_STRUCT(&extract_config, FR_ExtractConfig);
	extract_config.extractOptions = (!is_masked_feature_) ? FR_FEATURE_OPTION_FULL_FEATURE : FR_FEATURE_OPTION_MASKED_FEATURE;
    spRecognizer_->SetExtractionOption(FR_EXTRACTION_OPTION_DETECTION_OUTPUT_ORDER, FR_DETECTION_OUTPUT_ORDER_FACE_WIDTH);
	while (1)
	{
		fr_return = spRecognizer_->ExtractFace(&extract_config, &image, image_num, &face_num);
		if (fr_return != FR_RETURN_RECOGNIZE_IMAGE_PROCESSING_EXCEEDED)
			break;
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (FR_FAILED(fr_return))
	{
		printf("ExtractFace Failed. Error code: %d\n", fr_return);
		return fr_return;
	}

    fr_return = spRecognizer_->GetFaceFeature(0, 0, &feature);
	if (FR_FAILED(fr_return))
	{
		printf("GetFaceFeature Failed. Error code: %d\n", fr_return);
		return fr_return;
	}
    return FR_RETURN_OK;
}

FR_RETURN MatchHandler::Match(std::string path, std::string path2, float& confidence)
{
    CheckParameter(spRecognizer_ == nullptr, FR_RETURN_NOT_INIT);

    FR_RETURN fr_return = FR_RETURN_OK;
    
    FR_FaceFeature feature1;
    FR_INIT_STRUCT(&feature1, FR_FaceFeature);
    FR_RET_ON_FAILED(GetFeatureByPath(path, feature1));

    FR_FaceFeature feature2;
    FR_INIT_STRUCT(&feature2, FR_FaceFeature);
    FR_RET_ON_FAILED(GetFeatureByPath(path2, feature2));
    
	fr_return = spRecognizer_->CompareFaceFeature(&feature1, &feature2, &confidence);
	
	if (FR_FAILED(fr_return)) {
		LOGPRINTF("Recognizer CompareFaceFeature failed. Error code: %d\n", fr_return);
		return fr_return;
	}
    
    return FR_RETURN_OK;
}

bool MatchHandler::DecodeImage(std::string path, cv::Mat& image_mat)
{
	// imread can't read unicode, so using file to open then decode into cv::mat
#if defined(_UNICODE) || defined(UNICODE)
	std::ifstream file(utf8_to_unicode(path.c_str()), std::ios::binary);
#else
	std::ifstream file(path.c_str(), std::ios::binary);
#endif
	file.seekg(0, std::ios::end);
	int bufferLength = (int)file.tellg();
	file.seekg(0, std::ios::beg);
	if (file.fail())
	{
		return false;
	}
	char *buffer = new char[bufferLength];
	file.read(buffer, bufferLength);

	//Decode data into Mat 
	image_mat = cv::imdecode(cv::Mat(1, bufferLength, CV_8UC1, buffer), cv::IMREAD_COLOR);

	file.close();
	delete [] buffer;
	buffer = NULL;
	return true;
}

std::string MatchHandler::GetErrorMsg(FR_RETURN hr)
{
    return ToErrorMsg(hr);
}