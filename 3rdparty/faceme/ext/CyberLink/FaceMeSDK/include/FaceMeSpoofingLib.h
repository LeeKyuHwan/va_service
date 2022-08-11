#ifndef	__SPOOFING_LIB_H__
#define __SPOOFING_LIB_H__

#include <string>
#include "IFaceMeSpoofingManager.h"

#if defined(_WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#if defined(USE_FACEMESDK_STATIC_LIB)
#ifdef _WIN32
#define SPOOFING_API extern "C"
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#define SPOOFING_API extern "C" __attribute__((visibility("default")))
#elif TARGET_OS_IPHONE
#define SPOOFING_API extern "C" __attribute__((visibility("default")))
#elif TARGET_OS_MAC
#define SPOOFING_API extern "C"
#else
#error "Unknown Apple platform"
#endif
#elif defined(__ANDROID__)
#define SPOOFING_API extern "C" __attribute__((visibility("default")))
#elif __linux__
#define SPOOFING_API extern "C"
#elif __unix__  // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif

SPOOFING_API FaceMeSDK::FR_RETURN FaceMeCreateInstance(FaceMeSDK::FR_INSTANCE_TYPE type, void **instance);
#endif

class SpoofingLib
{
private:
	void* m_hModule;
	std::string m_libDirPath;

#if defined(_WIN32)
#if (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP)
	void* Load(const wchar_t* szModulePath = L"SpoofingManager.dll")
#else
	void* Load(const char* szModulePath = "SpoofingManager.dll")
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
#if (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP)
			m_hModule = LoadPackagedLibrary(modulePath.c_str(), 0);
#else
			m_hModule = LoadLibraryExA(modulePath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
#endif
#elif defined(__ANDROID__)
			m_hModule = dlopen(modulePath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
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
	SpoofingLib(const char *libDirPath = nullptr)
		: m_hModule(nullptr)
		, m_libDirPath((libDirPath != nullptr) ? libDirPath : "")
	{

	}

	~SpoofingLib()
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

	bool CreateSpoofingManager(FaceMeSDK::IFaceMeSpoofingManager **ppInstance)
	{
		return CreateInstance(FaceMeSDK::FR_INSTANCE_SPOOFING_MANAGER, reinterpret_cast<void **>(ppInstance));
	}

	inline void CleanUp()
	{
		Load(nullptr);
	}

};

#endif // __FACEME_SDK_LIB_H__
