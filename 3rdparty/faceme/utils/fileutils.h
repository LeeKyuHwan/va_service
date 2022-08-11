/**
* @file fileutils.h
* File utilities
*
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef _FILEUTILS_H_
#define _FILEUTILS_H_

#include <string>
#include <vector>

namespace cl_utility
{
	namespace fileutils
	{
		// Generates an empty file with a unique name in the specified directory and
		// returns the file name and path.
		std::string TempFilename(const std::string& dir, const std::string& prefix);

		// Reads the content of a directory and, in case of success, returns a vector
		// of strings with one element for each found file or directory. Each element is
		// a path created by prepending |dir| to the file/directory name. "." and ".."
		// are never added in the returned vector.
		std::vector<std::string> ReadDirectory(std::string path);

		// Creates a directory if it not already exists.
		// Returns true if successful. Will print an error message to stderr and return
		// false if a file with the same name already exists.
		bool CreateDir(const std::string& directory_name);

		bool CreateDirs(const std::string& directory_path);

		// Removes a directory, which must already be empty.
		bool RemoveDir(const std::string& directory_name);

		// Removes a file.
		bool RemoveFile(const std::string& file_name);

		// File size of the supplied file in bytes. Will return 0 if the file is
		// empty or if the file does not exist/is readable.
		size_t GetFileSize(const std::string& filename);

		class Path
		{
		public:
			Path();
			Path(const char* path);
			Path(const std::string &path);

            operator std::string() const;

			std::string basename() const;
            std::string dirname() const;
            const std::string &str() const;

			Path dir() const;
			bool exists() const;
			bool dirExists() const;

			Path operator+(const Path& other) const;
			Path &operator+=(const Path& other);

			static const char* separator();
			static const char* separators();

		public:
			static const char SEPARATOR;

		private:
			void init();

			static bool replaceLast(std::string& str, const std::string& from, const std::string& to);
			static bool endsWith(const std::string &str, const std::string &suffix);

		private:
			std::string m_path;
		};

		std::string GetAppDataDir(const char *appName);
		std::string GetUserDataDir(const char *appName);
		std::string GetUserCacheDir(const char *appName);
		std::string GetLibraryPath(void *symbol);
		std::string GetExecutablePath();
	}	// namespace fileutils
}  // namespace cl_utility

#endif  // _FILEUTILS_H_
