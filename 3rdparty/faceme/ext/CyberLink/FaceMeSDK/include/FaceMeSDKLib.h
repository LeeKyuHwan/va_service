/**
* @file FaceSDKLib.h
* The Dynamic Loader for CyberLink FaceMe SDK.
*
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#ifndef	__FACEME_SDK_LIB_H__
#define __FACEME_SDK_LIB_H__

#include <string>
#include "IFaceMeLicenseManager.h"
#include "IFaceMeRecognizer.h"
#include "IFaceMeDataManager.h"
#include "IFaceMeMotionDetector.h"
#include "IFaceMeLivenessDetector.h"
#include "IFaceMeUserActionDetector.h"
#include "IFaceMeQualityDetector.h"

#if defined(_WIN32)
#include <codecvt>
#include <locale>
#include <string>
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#if defined(USE_FACEMESDK_STATIC_LIB)
#ifdef _WIN32
#define FACEMESDK_API extern "C"
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#define FACEMESDK_API extern "C" __attribute__((visibility("default")))
#elif TARGET_OS_IPHONE
#define FACEMESDK_API extern "C" __attribute__((visibility("default")))
#elif TARGET_OS_MAC
#define FACEMESDK_API extern "C"
#else
#error "Unknown Apple platform"
#endif
#elif defined(__ANDROID__)
#define FACEMESDK_API extern "C" __attribute__((visibility("default")))
#elif __linux__
#define FACEMESDK_API extern "C"
#elif __unix__  // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif

FACEMESDK_API FaceMeSDK::FR_RETURN FaceMeCreateInstance(FaceMeSDK::FR_INSTANCE_TYPE type, void **instance);
#endif

class FaceMeSDKLib
{
private:
	void* m_hModule;
	std::string m_libDirPath;

#if defined(_WIN32)
#if (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP)
	void* Load(const wchar_t* szModulePath = L"FaceMeSDK.dll")
#else
	void* Load(const char* szModulePath = "FaceMeSDK.dll")
#endif
#elif defined(__ANDROID__)
	void* Load(const char* szModulePath = "libFaceMeSDK.so")
#else
	void* Load(const char* szModulePath = "./libFaceMeSDK.so")
#endif
	{
#if defined(USE_FACEMESDK_STATIC_LIB)
		return nullptr;
#else
		if (!szModulePath)
		{
			if (m_hModule)
			{
#if defined(_WIN32)
				FreeLibrary((HMODULE)m_hModule);
#else
				dlclose(m_hModule);
#endif
				m_hModule = nullptr;
			}

			return nullptr;
		}

		if (!m_hModule)
		{
#ifdef _WIN32
			const char SEPARATOR = '\\';
#else
			const char SEPARATOR = '/';
#endif

			std::string modulePath;
			const size_t lastSlash = m_libDirPath.find_last_of(separators());
			if ((lastSlash != std::string::npos && (lastSlash + 1) >= m_libDirPath.size())
				|| m_libDirPath.empty())
			{
				modulePath = m_libDirPath + szModulePath;
			}
			else
			{
				modulePath = m_libDirPath + SEPARATOR + szModulePath;
			}

#if defined(_WIN32)
			// convert from utf-8 to usc-2 string for windows file path.
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			std::wstring wmodulePath = converter.from_bytes(modulePath);
#if (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP)
			m_hModule = LoadPackagedLibrary(wmodulePath.c_str(), 0);
#else
			m_hModule = LoadLibraryExW(wmodulePath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
#endif
#elif defined(__ANDROID__)
			m_hModule = dlopen(szModulePath, RTLD_LAZY | RTLD_GLOBAL);
#else
			m_hModule = dlopen(modulePath.c_str(), RTLD_LAZY | RTLD_LOCAL);
#endif
			if (!m_hModule)
			{
				return nullptr;
			}
		}

		return m_hModule;
#endif
	}

	const char* separators()
	{
#ifdef _WIN32
		return "\\/";
#else
		return "/";
#endif
	}
public:
	FaceMeSDKLib(const char *libDirPath = nullptr)
		: m_hModule(nullptr)
		, m_libDirPath((libDirPath != nullptr) ? libDirPath : "")
	{

	}

	~FaceMeSDKLib()
	{
		CleanUp();
	}

	bool CreateInstance(FaceMeSDK::FR_INSTANCE_TYPE type, void **ppInstance)
	{
#if defined(USE_FACEMESDK_STATIC_LIB)
		return FR_SUCCEEDED(FaceMeCreateInstance(type, ppInstance));
#else
		void* hModule = nullptr;
		hModule = Load();
		if (hModule)
		{
			FaceMeSDK::FnFaceMeCreateInstance fCreateInstance = nullptr;
#if defined(_WIN32)
			fCreateInstance = (FaceMeSDK::FnFaceMeCreateInstance)GetProcAddress((HMODULE)hModule, "FaceMeCreateInstance");
#else
			fCreateInstance = (FaceMeSDK::FnFaceMeCreateInstance)dlsym(hModule, "FaceMeCreateInstance");
#endif
			if (fCreateInstance)
			{
				return FR_SUCCEEDED(fCreateInstance(type, ppInstance));
			}
		}

		ppInstance = nullptr;

		return false;
#endif
	}

	bool CreateFaceMeLicenseManager(FaceMeSDK::IFaceMeLicenseManager **ppInstance)
	{
		return CreateInstance(FaceMeSDK::FR_INSTANCE_LICENSE_MANAGER, reinterpret_cast<void **>(ppInstance));
	}

	bool CreateFaceMeLicenseManager(FaceMeSDK::IFaceMeLicenseManagerEx **ppInstance)
	{
		return CreateInstance(FaceMeSDK::FR_INSTANCE_LICENSE_MANAGER, reinterpret_cast<void **>(ppInstance));
	}

	bool CreateFaceMeRecognizer(FaceMeSDK::IFaceMeRecognizer **ppInstance)
	{
		return CreateInstance(FaceMeSDK::FR_INSTANCE_FACE_RECOGNIZER, reinterpret_cast<void **>(ppInstance));
	}

	bool CreateFaceMeDataManager(FaceMeSDK::IFaceMeDataManager **ppInstance)
	{
		return CreateInstance(FaceMeSDK::FR_INSTANCE_LOCAL_FACE_DATA_MANAGER, reinterpret_cast<void **>(ppInstance));
	}

    bool CreateFaceMeLivenessDetector(FaceMeSDK::IFaceMeLivenessDetector **ppInstance)
    {
        return CreateInstance(FaceMeSDK::FR_INSTANCE_LIVENESS_DETECTOR, reinterpret_cast<void **>(ppInstance));
    }

    bool CreateFaceMeUserActionDetector(FaceMeSDK::IFaceMeUserActionDetector **ppInstance)
    {
        return CreateInstance(FaceMeSDK::FR_INSTANCE_USER_ACTION_DETECTOR, reinterpret_cast<void **>(ppInstance));
    }

	bool CreateFaceMeMotionDetector(FaceMeSDK::IFaceMeMotionDetector **ppInstance)
	{
		return CreateInstance(FaceMeSDK::FR_INSTANCE_MOTION_DETECTOR, reinterpret_cast<void **>(ppInstance));
	}

	bool CreateFaceMeODBCDataManager(FaceMeSDK::IFaceMeDataManager **ppInstance)
	{
		return CreateInstance(FaceMeSDK::FR_INSTANCE_ODBC_DATA_MANAGER, reinterpret_cast<void **>(ppInstance));
	}

    bool CreateFaceMeQualityDetector(FaceMeSDK::IFaceMeQualityDetector **ppInstance)
    {
        return CreateInstance(FaceMeSDK::FR_INSTANCE_QUALITY_DETECTOR, reinterpret_cast<void **>(ppInstance));
    }


	inline void CleanUp()
	{
		Load(nullptr);
	}

};

#endif // __FACEME_SDK_LIB_H__
