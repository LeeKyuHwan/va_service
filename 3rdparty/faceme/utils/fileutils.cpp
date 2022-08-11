/**
* @file fileutils.cpp
* Implementation of file utilities.
*
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#include "fileutils.h"

#include <assert.h>

#ifdef _WIN32
#include <algorithm>
#include <direct.h>
#include <tchar.h>
#include <windows.h>
#include <ShlObj.h>
#include <objbase.h>
#include <Shlwapi.h>

#include "strutils.h"
#else
#include <dirent.h>
#include <pwd.h>
#include <unistd.h>
#include <limits.h>
#include <linux/limits.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <dlfcn.h>
#endif

#include <sys/stat.h>  // To check for directory existence.
#ifndef S_ISDIR        // Not defined in stat.h on Windows.
#define S_ISDIR(mode) (((mode)&S_IFMT) == S_IFDIR)
#endif

#include <cstdio>
#include <cstdlib>
#include <string>

#include <memory>
#include <utility>

namespace cl_utility
{
	namespace fileutils
	{
		// Generate a temporary filename in a safe way.
		// Largely copied from talk/base/{unixfilesystem,win32filesystem}.cc.
		std::string TempFilename(const std::string& dir, const std::string& prefix) 
		{
#ifdef _WIN32
			wchar_t filename[MAX_PATH];
            if (::GetTempFileNameW(USC2String(dir).c_str(), USC2String(prefix).c_str(), 0,
				filename) != 0)
				return Utf8String(filename);
			assert(false);
			return "";
#else
			int len = dir.size() + prefix.size() + 2 + 6;
			std::unique_ptr<char[]> tempname(new char[len]);

			snprintf(tempname.get(), len, "%s/%sXXXXXX", dir.c_str(), prefix.c_str());
			int fd = ::mkstemp(tempname.get());
			if (fd == -1) {
				assert(false);
				return "";
			}
			else {
				::close(fd);
			}
			std::string ret(tempname.get());
			return ret;
#endif
		}

		std::vector<std::string> ReadDirectory(std::string path) 
		{
			if (path.length() == 0) return std::vector<std::string>();

#if defined(_WIN32)
			// Append separator character if needed.
			if (path.back() != '\\') path += '\\';

			// Init.
			WIN32_FIND_DATA data;
#if defined(UNICODE) || defined(_UNICODE)
			HANDLE handle = ::FindFirstFile(USC2String(path + '*').c_str(), &data);
#else
			HANDLE handle = ::FindFirstFile((path + '*').c_str(), &data);
#endif
			if (handle == INVALID_HANDLE_VALUE) return std::vector<std::string>();

			// Populate output.
			std::vector<std::string> found_entries;
			do {
				const std::string name = Utf8String(data.cFileName);
				if (name != "." && name != "..") found_entries.emplace_back(path + name);
			} while (::FindNextFile(handle, &data) == TRUE);

			// Release resources.
			if (handle != INVALID_HANDLE_VALUE) ::FindClose(handle);
#else
			// Append separator character if needed.
			if (path.back() != '/') path += '/';

			// Init.
			DIR* dir = ::opendir(path.c_str());
			if (dir == nullptr) return std::vector<std::string>();

			// Populate output.
			std::vector<std::string> found_entries;
			while (dirent* dirent = readdir(dir)) {
				const std::string& name = dirent->d_name;
				if (name != "." && name != "..") found_entries.emplace_back(path + name);
			}

			// Release resources.
			closedir(dir);
#endif

			return found_entries;
		}

		bool CreateDir(const std::string& directory_name) 
		{
#ifdef _WIN32
			struct _stat64i32 path_info = { 0 };
			// Check if the path exists already:
			if (_wstat(USC2String(directory_name).c_str(), &path_info) == 0)
			{
				if (!S_ISDIR(path_info.st_mode)) {
					fprintf(stderr,
						"Path %s exists but is not a directory! Remove this "
						"file and re-run to create the directory.\n",
						directory_name.c_str());
					return false;
				}
			}
#else
			struct stat path_info = { 0 };
			// Check if the path exists already:
			if (stat(directory_name.c_str(), &path_info) == 0) 
			{
				if (!S_ISDIR(path_info.st_mode)) 
				{
					fprintf(stderr,
						"Path %s exists but is not a directory! Remove this "
						"file and re-run to create the directory.\n",
						directory_name.c_str());
					return false;
				}
			}
#endif
			else 
			{
#ifdef _WIN32
				return _wmkdir(USC2String(directory_name).c_str()) == 0;
#else
				return mkdir(directory_name.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
#endif
			}
			return true;
		}

		bool CreateDirs(const std::string& directory_path) {
			bool ret = CreateDir(directory_path);
			if (!ret)
			{
				uint64_t pos = directory_path.find_last_of('/');
				if (pos == std::string::npos)
#if defined(_WIN32)
					pos = directory_path.find_last_of('\\');
				if (pos == std::string::npos)
#endif
					return false;
				if (!CreateDirs(directory_path.substr(0, pos))) return false;
			}
			return CreateDir(directory_path);
		}

		bool RemoveDir(const std::string& directory_name) 
		{
#ifdef _WIN32
            return RemoveDirectoryW(USC2String(directory_name).c_str()) != FALSE;
#else
			return rmdir(directory_name.c_str()) == 0;
#endif
		}

		bool RemoveFile(const std::string& file_name) 
		{
#ifdef _WIN32
            return DeleteFileW(USC2String(file_name).c_str()) != FALSE;
#else
			return unlink(file_name.c_str()) == 0;
#endif
		}

		// Rename a file.
		bool RenameFile(const std::string& old_file_name, const std::string& new_file_name)
		{
#ifdef _WIN32
			return MoveFileW(USC2String(old_file_name).c_str(), USC2String(new_file_name).c_str()) != FALSE;
#else
			return std::rename(old_file_name.c_str(), new_file_name.c_str()) == 0;
#endif
		}

		size_t GetFileSize(const std::string& filename) 
		{
#ifdef _WIN32
            FILE* f = NULL;
            fopen_s(&f, filename.c_str(), "rb");
#else
            FILE* f = fopen(filename.c_str(), "rb");
#endif
			size_t size = 0;
			if (f != nullptr) {
				if (fseek(f, 0, SEEK_END) == 0) {
					size = ftell(f);
				}
				fclose(f);
			}
			return size;
		}
#ifdef _WIN32
		const char Path::SEPARATOR = '\\';
#else
		const char Path::SEPARATOR = '/';
#endif

		Path::Path()
		{
		}

		Path::Path(const char* path)
			: m_path(path)
		{
			init();
		}

		Path::Path(const std::string &path)
			: m_path(path)
		{
			init();
		}

		Path::operator std::string() const
		{
			return m_path;
		}

        const std::string &Path::str() const
        {
            return m_path;
        }

		std::string Path::basename() const
		{
			const size_t lastSlash = m_path.find_last_of(separators());
			if (lastSlash == std::string::npos)
				return m_path;

			return m_path.substr(lastSlash + 1);
		}

		std::string Path::dirname() const
		{
			const size_t lastSlash = m_path.find_last_of(separators());
			if (lastSlash == std::string::npos)
				return "";

			return m_path.substr(0, lastSlash);
        }

		Path Path::dir() const
		{
			return Path(dirname());
		}

		bool Path::exists() const
		{
#ifdef _WIN32
			struct _stat64i32 info = { 0 };
			return (_wstat(USC2String(m_path).c_str(), &info) == 0);
#else
			struct stat info = { 0 };
			return (stat(m_path.c_str(), &info) == 0);
#endif
		}

		bool Path::dirExists() const
		{
#ifdef _WIN32
			struct _stat64i32 info = { 0 };
			int result = _wstat(USC2String(m_path).c_str(), &info);
			return (result == 0) && S_ISDIR(info.st_mode);
#else
			struct stat info = { 0 };
			int result = stat(m_path.c_str(), &info);
			return (result == 0) && S_ISDIR(info.st_mode);
#endif
		}

		Path Path::operator+(const Path& other) const
		{
			if (m_path.empty())
				return other;
			if (other.m_path.empty())
				return *this;

			return Path(m_path + SEPARATOR + other.m_path);
		}

		Path &Path::operator+=(const Path& other)
		{
			m_path = *this + other;
			return *this;
		}

		const char* Path::separator()
		{
			static const std::string sep(1, SEPARATOR);
			return sep.c_str();
		}

		const char* Path::separators()
		{
#ifdef _WIN32
			return "\\/";
#else
			return "/";
#endif
		}

		void Path::init()
		{
			// Remove trailing separator
			if (endsWith(m_path, separator()))
				replaceLast(m_path, separator(), "");
		}

		bool Path::replaceLast(std::string& str, const std::string& from, const std::string& to) {
			size_t start_pos = str.rfind(from);
			if (start_pos == std::string::npos)
				return false;
			str.replace(start_pos, from.length(), to);
			return true;
		}

		bool Path::endsWith(const std::string &str, const std::string &suffix)
		{
			return str.size() >= suffix.size() &&
				str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
		}

#if defined(_WIN32)
		static std::string GetKnownWindowsFolder(REFKNOWNFOLDERID folderId)
		{
			LPWSTR wszPath = nullptr;
			HRESULT hr;
			hr = SHGetKnownFolderPath(folderId, KF_FLAG_CREATE, nullptr, &wszPath);
			if (FAILED(hr))
			{
				return std::string();
			}

			std::string knownFolderPath = Utf8String(wszPath);
			::CoTaskMemFree(wszPath);
			return knownFolderPath;
		}

		static std::string GetUserDataHome()
		{
			return GetKnownWindowsFolder(FOLDERID_RoamingAppData);
		}

		static std::string GetAppDataHome()
		{
			return GetKnownWindowsFolder(FOLDERID_ProgramData);
		}

		static std::string GetCacheDir()
		{
			return GetKnownWindowsFolder(FOLDERID_LocalAppData);
		}

		std::string GetLibraryPath(void *symbol)
		{
			wchar_t wcSystemPath[MAX_PATH];
			wcSystemPath[0] = '\0';

			HMODULE module;

			if (!GetModuleHandleExA(
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPCSTR)symbol,
				&module)) return std::string();

			DWORD len = GetModuleFileNameW(module, wcSystemPath, MAX_PATH);
			if(len == 0) return std::string();
			std::wstring systemPath(wcSystemPath, len);
			return Utf8String(systemPath);
		}

		std::string GetExecutablePath()
		{
			wchar_t wcExePath[MAX_PATH];

			DWORD len = GetModuleFileNameW(GetModuleHandleW(NULL), wcExePath, MAX_PATH);
			if (len == 0) return std::string();
			std::wstring exePath(wcExePath, len);
			return Utf8String(exePath);
		}
#else
        static std::string GetHome()
        {
            std::string res;
            int uid = getuid();
            const char* homeEnv = std::getenv("HOME");
            if (uid != 0 && homeEnv)
            {
                //We only acknowlegde HOME if not root.
                res = homeEnv;
                return res;
            }
            struct passwd* pw = nullptr;
            struct passwd pwd;
            long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
            if (bufsize < 0)
            {
                bufsize = 16384;
            }
            std::vector<char> buffer;
            buffer.resize(bufsize);
            int error_code = getpwuid_r(uid, &pwd, buffer.data(), buffer.size(), &pw);
            if (error_code)
            {
                return std::string();
            }
            const char* tempRes = pw->pw_dir;
            if (!tempRes)
            {
                return std::string();
            }
            res = tempRes;
            return res;
        }

        static std::string GetDefaultLinuxFolder(const char* envName, const char* defaultRelativePath)
        {
            std::string res;
            const char* tempRes = std::getenv(envName);
            if (tempRes && tempRes[0] == '/')
            {
                res = tempRes;
                return res;
            }

            std::string homeDir = GetHome();
            if (homeDir.empty()) return std::string();

            res = homeDir + "/" + defaultRelativePath;
            if (!CreateDirs(res)) return std::string();
            return res;
        }

        static std::string GetUserDataHome()
        {
            return GetDefaultLinuxFolder("XDG_DATA_HOME", ".local/share");
        }

        static std::string GetAppDataHome()
        {
            return GetUserDataHome();
        }

        static std::string GetCacheDir()
        {
            return GetDefaultLinuxFolder("XDG_CACHE_HOME", ".cache");
        }

        std::string GetLibraryPath(void *symbol)
        {
            if (symbol == nullptr) return std::string();
            Dl_info dlInfo;
            int ret = dladdr(symbol, &dlInfo);
            if (ret <= 0 || dlInfo.dli_fname == nullptr) return std::string();

            return std::string(dlInfo.dli_fname);
        }

        std::string GetExecutablePath()
        {
            char exePath[PATH_MAX];
            int len = readlink("/proc/self/exe", exePath, PATH_MAX);

            if (len <= 0 || len == PATH_MAX) return std::string();
            return std::string(exePath, len);
        }
#endif
		std::string GetAppDataDir(const char *appName)
		{
			std::string dataHome = GetAppDataHome();
			if (dataHome.empty()) return std::string();
			if (appName == nullptr)
			{
				return dataHome;
			}

			std::string appDataDir = dataHome + Path::SEPARATOR + appName;
			if (!CreateDirs(appDataDir)) return std::string();
			return appDataDir;
		}

		std::string GetUserDataDir(const char *appName)
		{
			std::string dataHome = GetUserDataHome();
			if (dataHome.empty()) return std::string();
			if (appName == nullptr)
			{
				return dataHome;
			}

			std::string userDataDir = dataHome + Path::SEPARATOR + appName;
			if (!CreateDirs(userDataDir)) return std::string();
			return userDataDir;
		}

		std::string GetUserCacheDir(const char *appName)
		{
			std::string cacheDir = GetCacheDir();
			if (cacheDir.empty())
			{
				// Using app data directory when no access right at user cache directory.
				cacheDir = GetAppDataDir(appName);
				if (cacheDir.empty()) return std::string();
			}

			if (appName == nullptr)
			{
				return cacheDir;
			}

			std::string userCacheDir = cacheDir + Path::SEPARATOR + appName;
			if (!CreateDirs(userCacheDir)) return std::string();
			return userCacheDir;
		}
	}
}  // namespace cl_utility
