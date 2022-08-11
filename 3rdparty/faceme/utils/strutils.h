/**
* @file strutils.h
* String utilities
*
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#include <codecvt>
#include <locale>
#include <string>
#include <cctype>
#include <cwctype>

namespace cl_utility
{
	/**
	Transcode UTF-8 string to UTF-16 string.
	*/
    inline std::u16string Utf16String(std::string const & src)
	{
// https://stackoverflow.com/questions/32055357/visual-studio-c-2015-stdcodecvt-with-char16-t-or-char32-t
// https://blog.csdn.net/zrufo747/article/details/80233764
#if (_MSC_VER >= 1900 /* VS 2015*/) && (_MSC_VER <= 1919 /* VS 2017 */)
        std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
            auto p = reinterpret_cast<const char *>(src.data());
            auto str = convert.from_bytes(p, p + src.size());
            std::u16string u16_str(str.begin(),str.end());
            return u16_str;
#else
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
        return converter.from_bytes(src);
#endif
	}

    inline std::u16string Utf16String(const char *src)
	{
		return Utf16String(std::string((src != nullptr) ? src : ""));
	}

	/**
	Non transcode UTF-16 string pass through.
	*/
    inline std::u16string Utf16String(std::u16string const & src)
	{
		return src;
	}

    inline std::u16string Utf16String(const char16_t *src)
	{
        return std::u16string((src != nullptr) ? src : u"");
	}

    /**
    Transcode UTF-8 string to USC2 string.
    */
    inline std::wstring USC2String(std::string const & src)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.from_bytes(src);
    }

    inline std::wstring USC2String(const char *src)
    {
        return USC2String(std::string((src != nullptr) ? src : ""));
    }

    /**
    Non transcode USC2 string pass through.
    */
    inline std::wstring USC2String(std::wstring const & src)
    {
        return src;
    }

    inline std::wstring USC2String(const wchar_t *src)
    {
        return std::wstring((src != nullptr) ? src : L"");
    }

	/**
	Transcode UTF-16 string to UTF-8 string.
	*/
    inline std::string Utf8String(std::u16string const & src)
	{
        // https://stackoverflow.com/questions/32055357/visual-studio-c-2015-stdcodecvt-with-char16-t-or-char32-t
        // https://blog.csdn.net/zrufo747/article/details/80233764
#if (_MSC_VER >= 1900 /* VS 2015*/) && (_MSC_VER <= 1915 /* VS 2017 */)
        std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> converter;
        auto p = reinterpret_cast<const int16_t *>(src.data());
        return converter.to_bytes(p, p + src.size());
#else
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
		return converter.to_bytes(src);
#endif
	}

    /**
    Transcode USC2 string to UTF-8 string.
    */
    inline std::string Utf8String(std::wstring const & src)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(src);
    }

    inline std::string Utf8String(const char16_t *src)
	{
        return Utf8String(std::u16string((src != nullptr) ? src : u""));
	}

    inline std::string Utf8String(const wchar_t *src)
    {
        return Utf8String(std::wstring((src != nullptr) ? src : L""));
    }

	/**
	Non Transcode UTF-8 string pass through.
	*/
	inline std::string Utf8String(std::string const & src)
	{
		return src;
	}

	inline std::string Utf8String(const char *src)
	{
		return std::string((src != nullptr) ? src : "");
	}

	static bool compareChar(const char & c1, const char & c2)
	{
		if (c1 == c2)
			return true;
		else if (std::toupper(c1) == std::toupper(c2))
			return true;
		return false;
	}

	/*
	* Case Insensitive String Comparision
	*/
	inline bool iequals(const std::string & str1, const std::string &str2)
	{
		return ((str1.size() == str2.size()) &&
			std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar));
	}

	static bool compareWChar(const wchar_t & c1, const wchar_t & c2)
	{
		if (c1 == c2)
			return true;
		else if (std::towupper(c1) == std::towupper(c2))
			return true;
		return false;
	}

	/*
	* Case Insensitive Wide String Comparision
	*/
	inline bool iequals(const std::wstring & str1, const std::wstring &str2)
	{
		return ((str1.size() == str2.size()) &&
			std::equal(str1.begin(), str1.end(), str2.begin(), &compareWChar));
	}

}  // namespace cl_utility

#endif  // _STRUTILS_H_
