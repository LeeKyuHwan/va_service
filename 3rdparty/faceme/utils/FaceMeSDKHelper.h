#pragma once
#ifndef __FACEME_SDK_HELPER__
#define __FACEME_SDK_HELPER__

#include <vector>
#include "FaceMeBaseDef.h"
#include "fileutils.h"
#ifdef UNIX_OS
#if IS_ANDROID
#include <android/log.h>
#else
#include <syslog.h>
#endif
#endif

using namespace FaceMeSDK;

#ifndef CheckParameter
#define CheckParameter(c, ret) {if ((c)) return (ret);}
#endif

#ifndef FR_INIT_STRUCT
#define FR_INIT_STRUCT(ptr, className) { memset(ptr, 0, sizeof(className)); (*ptr).sizeOfStructure = sizeof(className); }
#endif

#ifndef FR_RET_ON_FAILED
#define FR_RET_ON_FAILED(Code)  {FaceMeSDK::FR_RETURN _hr_=(Code); if (FR_FAILED(_hr_)) { return _hr_; }}
#endif

#ifndef LIC_OPT
#define LIC_OPT(status) ((int32_t)status)
#endif

#define STR_CYBERLINK       "CyberLink"
#define STR_FACEME_SDK      "FaceMeSDK"
#define STR_DEMO_TOOL       "DemoTool"
#define STR_DEMO_LICENSE_PATH  "License.key"
#define STR_CACHE_PATH  "fmsl.sim"
#define KEY_APNAME L"SOFTWARE\\CyberLink\\FaceMeSDK\\DemoSystem"

namespace cl_log
{
#ifdef UNIX_OS
#if IS_ANDROID
#include <android/log.h>
#else
#include <syslog.h>
#endif
#endif

    static bool log_flag = false;
#define DBG_MSG_PREFIX_TAG "[CLT-Sample] "
#define DBG_MSG_BUFFER_SIZE 4096
#define LOGPRINTF ((log_flag) ? (LogPrintf) : (LogOffPrintf))

    // If --enable-log is selected, this function will print log into the log file.
    void LogPrintf(LPCSTR log, ...)
    {
        static bool is_first = true;
        static Path log_path;
        if (is_first)
        {
            // Setup root directory path of user data and database.
            Path app_data_path(cl_utility::fileutils::GetAppDataDir(""));

            // Setup root directory path for logs.
            std::time_t t_now = std::time(nullptr);
#ifdef UNIX_OS
            std::tm& tm = *std::localtime(&t_now);
#else
            std::tm tm;
            localtime_s(&tm, &t_now);
#endif
            char str_now[MAX_PATH] = {};
            snprintf(str_now, MAX_PATH, "%04d%02d%02d-%02d%02d%02d.log", 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            log_path = app_data_path + Path(STR_CYBERLINK) + Path(STR_FACEME_SDK) + Path("Sample") + Path("Logs");
            CreateDirs(log_path);
            log_path += Path(str_now);
            is_first = false;
        }

        static int logcount = 0;
        logcount++;

        char buffer[DBG_MSG_BUFFER_SIZE] = DBG_MSG_PREFIX_TAG;
        uint64_t len = strlen(buffer);
        va_list marker;
        va_start(marker, log);
        vsnprintf(buffer + len, DBG_MSG_BUFFER_SIZE - len, log, marker);

#ifdef _WIN32
        OutputDebugStringA(buffer);
#else
        syslog(LOG_NOTICE, "%s", buffer);
#endif

        std::time_t t_now = std::time(nullptr);
#ifdef UNIX_OS
        std::tm& tm = *std::localtime(&t_now);
#else
        std::tm tm;
        localtime_s(&tm, &t_now);
#endif
        char str_now[MAX_PATH];
        snprintf(str_now, MAX_PATH, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

        std::fstream stream;
        stream.open(log_path, std::fstream::in | std::fstream::out | std::fstream::app);
        stream << std::to_string(logcount) << "	" << str_now << "	" << buffer;
        stream.close();

        va_end(marker);
    }

    void LogOffPrintf(LPCSTR log, ...) 
    {
        // Print log that can be check by Dbgview.
        /*
        char buffer[DBG_MSG_BUFFER_SIZE] = DBG_MSG_PREFIX_TAG;
        int32_t len = strlen(buffer);
        va_list marker;
        va_start(marker, log);
        vsnprintf(buffer + len, DBG_MSG_BUFFER_SIZE - len, log, marker);
#ifdef _WIN32
        OutputDebugStringA(buffer);
#else
        syslog(LOG_NOTICE, "%s", buffer);
#endif
        va_end(marker);
        */
    }
}

int32_t ConvertExtractInfo(std::string& model)
{
    if (iequals(model, "UH"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_ULTRA_HIGH;
    else if (iequals(model, "UH3"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_UH3;
    else if (iequals(model, "UH3_M"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_UH3_M;
	else if (iequals(model, "UH5"))
		return FR_EXTRACTION_MODEL_SPEED_LEVEL_UH5;
	else if (iequals(model, "UH5_M"))
		return FR_EXTRACTION_MODEL_SPEED_LEVEL_UH5_M;
    else if (iequals(model, "H1"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_HIGH;
    else if (iequals(model, "H2"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_HIGH_ASIAN;
    else if (iequals(model, "H3"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_H3;
	else if (iequals(model, "H5"))
		return FR_EXTRACTION_MODEL_SPEED_LEVEL_H5;
    else if (iequals(model, "VH"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_VERY_HIGH;
    else if (iequals(model, "VH_M"))
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_VH_M;
	else if (iequals(model, "VH5"))
		return FR_EXTRACTION_MODEL_SPEED_LEVEL_VH5;
	else if (iequals(model, "VH5_M"))
		return FR_EXTRACTION_MODEL_SPEED_LEVEL_VH5_M;
	else if (iequals(model, "DEFAULT"))
		return FR_EXTRACTION_MODEL_SPEED_LEVEL_DEFAULT;
    else
    {
        model = "DEFAULT";
        return FR_EXTRACTION_MODEL_SPEED_LEVEL_DEFAULT;
    }
}

int32_t ConvertDetectInfo(std::string& model)
{
    if (iequals(model, "DNN-X"))
        return FR_DETECTION_MODEL_SPEED_LEVEL_ULTRA_HIGH;
    else if (iequals(model, "DNN"))
        return FR_DETECTION_MODEL_SPEED_LEVEL_HIGH;
	else if (iequals(model, "DEFAULT"))
		return FR_DETECTION_MODEL_SPEED_LEVEL_DEFAULT;
    else
    {
        model = "DEFAULT";
        return FR_DETECTION_MODEL_SPEED_LEVEL_DEFAULT;
    }
}

bool LoadLicenseKey(std::string& license_key, std::string& license_path)
{
    Path facemeAppdataPath(cl_utility::fileutils::GetAppDataDir(""));
	facemeAppdataPath += Path(STR_CYBERLINK);
	facemeAppdataPath += Path(STR_FACEME_SDK);

	Path licensePath(facemeAppdataPath);
	licensePath += Path(STR_DEMO_TOOL);
	licensePath += Path(STR_DEMO_LICENSE_PATH);
	bool readSuccessful = false;
	std::string licenseInfoPath;
	if (licensePath.exists())
	{
		licenseInfoPath = licensePath.str();
		std::ifstream ifs(licenseInfoPath.c_str());
		if (ifs.good())
		{
			ifs >> license_key;
			if (!ifs.fail())
			{
				readSuccessful = true;
				license_path = licenseInfoPath;
			}
		}
	}
	if (!readSuccessful)
	{
		licensePath = facemeAppdataPath;
		licensePath += Path(STR_DEMO_LICENSE_PATH);
		if (licensePath.exists())
		{
			licenseInfoPath = licensePath.str();
			std::ifstream ifs(licenseInfoPath.c_str());
			if (ifs.good())
			{
				ifs >> license_key;
				if (!ifs.fail())
				{
					readSuccessful = true;
					license_path = licenseInfoPath;
				}
			}
		}
	}
#ifdef _WIN32
		if (!readSuccessful)
		{
			CRegKey regKey;
			if (regKey.Open(HKEY_CURRENT_USER, KEY_APNAME) != ERROR_SUCCESS)
				regKey.Create(HKEY_CURRENT_USER, KEY_APNAME);
			ULONG szlen;
			WCHAR szReturnedString[MAX_PATH] = { 0 };
			szReturnedString[0] = '\0';
			szlen = MAX_PATH;
			if (regKey.QueryStringValue(L"LicenseKey", szReturnedString, &szlen) == ERROR_SUCCESS)
			{
				readSuccessful = true;
				license_key = Utf8String(szReturnedString);
				license_path = std::string();
			}
			regKey.Close();
		}
#endif
	return readSuccessful;
}

bool SaveLicenseKey(std::string license_key)
{
	Path facemeAppdataPath(cl_utility::fileutils::GetAppDataDir(""));
	facemeAppdataPath += Path(STR_CYBERLINK);
	facemeAppdataPath += Path(STR_FACEME_SDK);
	CheckParameter(!CreateDirs(facemeAppdataPath), false);

	Path licensePath(facemeAppdataPath);
	licensePath += Path(STR_DEMO_TOOL);
	CheckParameter(!CreateDirs(licensePath), false);

	Path cacheInfoPath(cl_utility::fileutils::GetAppDataDir(""));
	cacheInfoPath += Path(STR_CYBERLINK);
	cacheInfoPath += Path(STR_FACEME_SDK);
	CheckParameter(!CreateDirs(cacheInfoPath), false);

	licensePath += Path(STR_DEMO_LICENSE_PATH);
	cacheInfoPath += Path(STR_CACHE_PATH);
	// Remove cache
	if (cacheInfoPath.exists())
		RemoveFile(cacheInfoPath);
	if (licensePath.exists())
		RemoveFile(licensePath);
	bool savesuccessful = false;
	std::string licenseInfoPath = licensePath.str();
	std::ofstream ofs;
	ofs.open(licenseInfoPath.c_str(), std::ios::out | std::ios::trunc);
	if (ofs.good())
	{
		ofs << license_key.c_str();
		if (ofs.good())
		{
			savesuccessful = true;
		}

	}
	ofs.close();

	if (!savesuccessful || !licensePath.exists())
	{
		licensePath = facemeAppdataPath;
		licensePath += Path(STR_DEMO_LICENSE_PATH);
		licenseInfoPath = licensePath.str();
		std::ofstream ofs;
		ofs.open(licenseInfoPath.c_str(), std::ios::out | std::ios::trunc);
		if (ofs.good())
		{
			ofs << license_key.c_str();
			if (ofs.good())
			{
				savesuccessful = true;
			}

		}
		ofs.close();
	}
#ifdef _WIN32
	if (!savesuccessful || !licensePath.exists())
	{
		CRegKey regKey;
		if (regKey.Open(HKEY_CURRENT_USER, KEY_APNAME) != ERROR_SUCCESS)
			regKey.Create(HKEY_CURRENT_USER, KEY_APNAME);
		regKey.SetStringValue(L"LicenseKey", USC2String(license_key).c_str());
		regKey.Close();
		savesuccessful = true;
	}
#endif
	return savesuccessful;
}

bool RemoveDemoToolLicenseKey()
{
	std::string licensePath;
	std::string licenseKey;
	bool result = LoadLicenseKey(licenseKey, licensePath);
	if (result && !licenseKey.empty())
	{
		if (!licensePath.empty())
			return RemoveFile(licensePath);
		else
		{
#ifdef _WIN32
			CRegKey regKey;
			HKEY hReg;
			LONG oresult;
			oresult = RegOpenKeyEx(HKEY_CURRENT_USER, KEY_APNAME, 0, KEY_ALL_ACCESS, &hReg);
			if (oresult == ERROR_SUCCESS)
				RegDeleteValue(hReg, TEXT("LicenseKey"));
			RegCloseKey(hReg);
#endif
		}
	}
	return true;
}

inline std::string ToErrorMsg(FR_RETURN hr)
{
    std::string errorMsg;
    switch (hr)
    {
    case FR_RETURN_FALSE: errorMsg = "False."; break;
    case FR_RETURN_OK: errorMsg = "Succeeded."; break;
    case FR_RETURN_FAIL: errorMsg = "Failed."; break;
    case FR_RETURN_OUT_OF_MEMORY: errorMsg = "Out of memory error."; break;
    case FR_RETURN_NOT_IMPLEMENT: errorMsg = "Not implement error."; break;
    case FR_RETURN_INVALID_ARGUMENT: errorMsg = "Invalid argument error."; break;
    case FR_RETURN_INDEX_OUT_OF_RANGE: errorMsg = "Index out of range error."; break;
    case FR_RETURN_UNEXPECTED: errorMsg = "Unexpected error."; break;
    case FR_RETURN_INCONSISTENT_VERSION: errorMsg = "Inconsistent data structure version error."; break;
    case FR_RETURN_NOT_INIT: errorMsg = "Not initialize error."; break;
    case FR_RETURN_CONFIG_ERROR: errorMsg = "Configuration error."; break;
    case FR_RETURN_NOT_FOUND: errorMsg = "No any face found."; break;
    case FR_RETURN_NOT_SUPPORT: errorMsg = "Not support error."; break;

    case FR_RETURN_FEATURE_SIZE_MISMATCH: errorMsg = "Feature size mismatch."; break;
    case FR_RETURN_DATABASE_ERROR: errorMsg = "Database access error."; break;
    case FR_RETURN_FEATURE_INCOMPLETE: errorMsg = "Feature extract incomplete for add."; break;
	case FR_RETURN_DATABASE_COLLECTION_EXCEEDED: errorMsg = "Collection count in database is exceeded."; break;

    case FR_RETURN_UNKNOWN_HOST:
    case FR_RETURN_NETWORK_ERROR:
        errorMsg = "Unable to connect to FaceMe license server.\n";
        errorMsg += "Please check if you can access to below URL correctly on your device.\n";
        errorMsg += "Make sure the firewall or network setting allows this device reaching to this domain.\n";
        errorMsg += "https ://faceme-sdk-api.cyberlink.com/";
        break;
    case FR_RETURN_LICENSE_FAIL: errorMsg = "License is failed."; break;
    case FR_RETURN_LICENSE_EXPIRED: errorMsg = "License is expired."; break;
    case FR_RETURN_LICENSE_ACTIVATION_EXCEEDED: errorMsg = "License activation is exceeded."; break;
    case FR_RETURN_LICENSE_NOT_SUPPORT: errorMsg = "License feature is not support."; break;
    case FR_RETURN_LICENSE_CONSTRAIN_HARDWARE: errorMsg = "License hardware is constrained."; break;
    case FR_RETURN_LICENSE_SERVER_ERROR: errorMsg = "License server error."; break;
    case FR_RETURN_LICENSE_INVALID_PACKAGE_NAME: errorMsg = "License is restricted with specific package name for Android, or bundleId for iOS."; break;
    case FR_RETURN_LICENSE_INCORRECT_KEY: errorMsg = "License key is incorrect."; break;

    case FR_RETURN_RUNTIME_FILE_NOT_COMPLETE: errorMsg = "Runtime files are not complete."; break;
    case FR_RETURN_INFERENCE_ENGINE_NOT_SUPPORT: errorMsg = "Inference engine not support."; break;

    case FR_RETURN_LICENSE_INCORRECT_PLATFORM: errorMsg = "License is not authorized on current platform."; break;
	case FR_RETURN_RECOGNIZE_IMAGE_PROCESSING_EXCEEDED: errorMsg = "Recognize fps is exceeded."; break;
	default: errorMsg = "Unknown error"; break;
    }

    return errorMsg;
}

#endif //__FACEME_SDK_HELPER__