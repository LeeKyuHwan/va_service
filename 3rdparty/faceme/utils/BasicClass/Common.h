#pragma once

#include <string>
#include <time.h>
#include <vector>
#include <thread>

#ifdef UNIX_OS

#ifdef _UNICODE
#undef _UNICODE
#endif

#ifdef UNICODE
#undef UNICODE
#endif

#undef _FOR_IOS_DEBUG // default disable for product release 

#include "Common_Unix.h"

#else
#include <direct.h>
#include <wtypes.h>
#include <tchar.h>
#include <shlobj.h>
#include <malloc.h>
#include <atlbase.h>
#include "psapi.h"
#endif

#ifdef _WINRT_METRO
#include "CpuCount.h"
#include "Common_Metro.h"
extern CpuCount g_cpu_count;
#endif

//////////////////////////////////////////////////////////////////////////

#define NEW_NO_EXCEPTION new (std::nothrow)

#define IS_WITHIN(x, min, max)  ((x) >= (min) && (x) < (max))

#define _NEW_PTR(ptr, type)             { if (ptr) delete ptr; ptr = NEW_NO_EXCEPTION type; }
#define _NEW_PTR_EXCEPTION(ptr, type)   { if (ptr) delete ptr; ptr = new type; }
#define _DELETE_PTR(ptr)                { if (ptr) { delete ptr; ptr = NULL; } }

#ifdef UNIX_OS

#if IS_ANDROID
#define _ALIGNED_MALLOC_PTR(ptr, type, size)    { if (ptr) free(ptr); ptr = (type *)memalign(16, sizeof(type) * size); }
#else

#ifdef _FOR_IOS_DEBUG
#define UNSAFE_MALLOC(ptr, type, size) { posix_memalign(&_ptr, 16, sizeof(type) * (size)); }
#else
#define UNSAFE_MALLOC(ptr, type, size) { int error = posix_memalign(&_ptr, 16, sizeof(type) * (size)); \
                                         if (error) {printf("memory alloc fail: %d\n", error);} }
#endif

#define _ALIGNED_MALLOC_PTR(ptr, type, size)    { if (ptr) free(ptr); void* _ptr = NULL; \
                                                  UNSAFE_MALLOC(ptr, type, size) \
                                                  ptr = (type *)_ptr;}
#endif
#define _ALIGNED_FREE_PTR(ptr)                  { if (ptr) { free(ptr); ptr = NULL; } }

#define __align16 __attribute__ ((aligned(16)))

#else

#define _ALIGNED_MALLOC_PTR(ptr, type, size)    { if (ptr) _aligned_free(ptr); ptr = (type *) _aligned_malloc(sizeof(type) * (size), 16); }
#define _ALIGNED_FREE_PTR(ptr)                  { if (ptr) { _aligned_free(ptr); ptr = NULL; } }

#define __align16 __declspec(align(16))

#endif

#define _NEW_PTRS(ptr, type, size)              { if (ptr) delete [] ptr; ptr = NEW_NO_EXCEPTION type[size]; }
#define _NEW_PTRS_EXCEPTION(ptr, type, size)    { if (ptr) delete [] ptr; ptr = new type[size]; }
#define _DELETE_PTRS(ptr)                       { if (ptr) { delete [] ptr; ptr = NULL; } }

#define _RELEASE_COMPTR(x)  { if (x.p) { x.Release(); x = NULL; } }

#define TO_PERCENT(a, b)	(0 == (b) ? 0 : (int)((a) * 100 / (b)))

#define TO_DEGREE(rad)      ((rad) / 3.1415926 * 180.0)
#define TO_RADIAN(deg)      ((deg) / 180.0 * 3.1415926)

#define CLALIGN(x, y) (((x) + (y) - 1) & ~((y) - 1))

#if defined(ALIGN) && (defined(IOS_ARM) || defined(MAC_OSX))
#undef ALIGN
#endif

#ifndef ALIGN
#define ALIGN(x, y) (((x) + (y) - 1) & ~((y) - 1))
#endif

#define FORCE_FALSE (-1 == __LINE__)

#define CLAMP(val, ubound, lbound)  (val > ubound ? ubound : (val < lbound ? lbound : val))

#ifndef UNIX_OS
#ifndef NAN
static const unsigned int nan__[2] = {0xffffffff, 0x7fffffff};
#define NAN (*(const float *)nan__)
#endif
#endif

#if defined(UNIX_OS)
#define CH_DIR_SEPARATOR '/'
#else
#define CH_DIR_SEPARATOR '\\'
#endif

//////////////////////////////////////////////////////////////////////////

extern CHAR szErrorMessage[];

#define _COM_CALL(_CALL_FUNC_)	\
    if(FAILED((hr = _CALL_FUNC_))) { \
    sprintf_s(szErrorMessage, _countof(szErrorMessage), "%s line %d. reason: 0x%08X", __FILE__, __LINE__, hr); \
    ::MessageBoxA(NULL, szErrorMessage, "COM error", MB_OK); \
    exit(-1); } 

//////////////////////////////////////////////////////////////////////////

#define _DBG_MSG_PREFIX_TAG _T("[CLT-Magic]")
int ch_dprintf(LPCTSTR fmt, ...);

#ifndef _DPRINTF
#if defined(_DEBUG) || defined(_DBG_MSG) || defined(_DBG_REL)
#if defined(_DBG_MSG) || defined(_DBG_REL)
#pragma message("Warning! _DPRINTF defined in release mode!") 
#endif
#ifdef UNIX_OS
#define _DPRINTF(argument)      ch_dprintf argument
#else
#define _DPRINTF(argument)      ch_dprintf##argument
#endif
#else
#define _DPRINTF(argument)
#endif
#endif

#ifndef _MYASSERT
    #if defined(_lint)      // for PC-Lint checking
        #define _MYASSERT(argument) ASSERT(argument)
    #elif defined(_DBG_REL) || defined(_DEBUG) // for debug purpose
        #ifdef UNIX_OS
            #define _MYASSERT(argument) \
            if(!(argument)) { \
                _ftprintf(stderr, _T("\n!!!Abnormal Exit on %s line %d!!!\n\n"), __FILE__, __LINE__); \
                _DPRINTF((_T("%s line %d\n"), __FILE__, __LINE__)); \
                exit(-1); \
            }
        #elif !defined(_WINRT_METRO)
            #define _MYASSERT(argument) \
            if(!(argument)) { \
                char errMsg[MAX_PATH]; \
                sprintf_s(errMsg, _countof(errMsg), "%s line %d", __FILE__, __LINE__); \
                ::MessageBoxA(NULL, errMsg, "Assertion fail", MB_OK); \
                exit(-1); \
            }
        #elif defined(_WIN32)
            #define _MYASSERT(argument) \
            if(!(argument)) { \
                char errMsg[MAX_PATH]; \
                sprintf_s(errMsg, _countof(errMsg), "%s line %d", __FILE__, __LINE__); \
                OutputDebugStringA(errMsg); \
                exit(-1); \
            }
        #endif
    #else                   // for release mode
        #define _MYASSERT(argument) ((void)0)
    #endif
#endif

#define _DISABLE_CLASS_COPY(class_name) \
    class_name(const class_name &); \
    class_name& operator =(const class_name &);

#ifdef UNIX_OS
inline bool PathFolderExists(LPCTSTR p_dir)
{
    DIR* dir = opendir("mydir");
    return dir || ENOENT != errno;
}
#else
#ifndef _WINRT_METRO
inline bool PathFolderExists(LPCTSTR p_dir)
{
    DWORD file_type = GetFileAttributes(p_dir);
    return (file_type != INVALID_FILE_ATTRIBUTES) && 
           (file_type & FILE_ATTRIBUTE_DIRECTORY);
}
#else
inline bool PathFolderExists(LPCTSTR p_dir)
{
	_WIN32_FILE_ATTRIBUTE_DATA  data;
    BOOL succeed = GetFileAttributesEx(p_dir, GetFileExInfoStandard, &data);
    return (succeed > 0) &&
		(data.dwFileAttributes != INVALID_FILE_ATTRIBUTES) && 
		(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}
#endif
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef UNIX_OS
typedef void * ThreadFunctionReturnType;
#else
typedef DWORD ThreadFunctionReturnType;
#endif

//////////////////////////////////////////////////////////////////////////

class CMyConfig
{
public:
    DWORD m_dump_info;
    CMyConfig();
    ~CMyConfig();
};

extern CMyConfig g_my_config;

#if defined(_DEBUG) || defined(_DBG_MSG) || defined(_DBG_REL)
#ifdef UNIX_OS
#define _REG_DUMP(argument)  do { if(g_my_config.m_dump_info) ch_dprintf argument; } while (0)
#define _REG_DUMP_EX(level, argument) do { if (g_my_config.m_dump_info >= (level)) ch_dprintf argument; } while (0);
#else
#define _REG_DUMP(argument)  do { if(g_my_config.m_dump_info) ch_dprintf##argument; } while (0)
#define _REG_DUMP_EX(level, argument) do { if (g_my_config.m_dump_info >= (level)) ch_dprintf##argument; } while (0);
#endif
#else
#define _REG_DUMP(argument)
#define _REG_DUMP_EX(level, argument)
#endif

#ifndef UNIX_OS
std::string unicode_to_utf8(const wchar_t *p_wide_string);
std::string unicodeToAnsi(const wchar_t *p_wide_string);
std::string unicode_to_ansi(const wchar_t *p_wide_string);
std::wstring utf8_to_unicode(const char *p_string);
std::wstring ansi_to_unicode(const char *p_string);
std::wstring AnsiToUnicode(const char *p_string);

#if defined(_UNICODE) || defined(UNICODE)
typedef std::wstring std_tstring;
#define std_to_tstring(x) std::to_wstring(x)
#define tstring_to_utf8(x) unicode_to_utf8((x).c_str())
#define tstring_to_ansi(x) unicode_to_ansi((x).c_str())
#define utf8_to_tstring(x) utf8_to_unicode((x).c_str())
#define ansi_to_tstring(x) ansi_to_unicode((x).c_str())
#else
typedef std::string std_tstring;
#define std_to_tstring(x) std::to_string(x)

#define tstring_to_utf8(x) (x)
#define tstring_to_ansi(x) (x)
#define utf8_to_tstring(x) (x)
#define ansi_to_tstring(x) (x)
#endif

#endif

inline int GetLogicalCPUCount()
{
#ifndef UNIX_OS
    #pragma warning (disable: 4799)

    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
#else
#if defined(MAC_OSX)
    int core_num;
    size_t buffer_legnth = sizeof(core_num);
    sysctlbyname("machdep.cpu.thread_count", &core_num, &buffer_legnth, NULL, NULL);
    return core_num;
#elif IS_ANDROID
#if defined(ANDROID_ARM) && defined(ENABLE_ARM_PROFILER)
    return 1; // The profiler only works for single thread.
#else
    return android_getCpuCount();
#endif
#elif defined(IOS_ARM)
    size_t length;
    
    int logical_cpu_number = 0;
    length = sizeof(logical_cpu_number);
    sysctlbyname("hw.logicalcpu", &logical_cpu_number, &length, NULL, 0);
    
    return logical_cpu_number;
#endif
#endif
    
	int thread_num = std::thread::hardware_concurrency();
	if (thread_num > 0) return thread_num;

    // Not supported.
    return -1;
}

//////////////////////////////////////////////////////////////////////////

enum ChAutoPtr_Allocator
{
    CHAP_New = 0, 
    CHAP_Malloc, 
};

template<typename _Type, ChAutoPtr_Allocator _Alloctor = CHAP_New>
class ChAutoPtr
{
    _Type *m_ptr;

    ChAutoPtr(const ChAutoPtr& _p);
    void operator=(const ChAutoPtr& _p);

public:
    ChAutoPtr()
    {
        m_ptr = NULL;
    }
    ChAutoPtr(_Type *p)
    {
        m_ptr = p;
    }
    ChAutoPtr(const int nElem)
    {
        m_ptr = NULL;
        Alloc(nElem);
    }
    ~ChAutoPtr()
    {
        if(m_ptr)
            Free();
    }

    _Type *Alloc(int nElem)
    {
        if(_Alloctor == CHAP_New)
        {
            m_ptr = new _Type[nElem];
        }
        else if(_Alloctor == CHAP_Malloc)
        {
            _ALIGNED_MALLOC_PTR(m_ptr, _Type, nElem);
        }
        return m_ptr;
    }
    void Free()
    {
        if(_Alloctor == CHAP_New)
        {
            delete [] m_ptr;
        }
        else if(_Alloctor == CHAP_Malloc)
        {
            _ALIGNED_FREE_PTR(m_ptr);
        }
        m_ptr = NULL;
    }
    operator const _Type *() const
    {
        return m_ptr;
    }
    operator _Type *()
    {
        return m_ptr;
    }
    void operator=(_Type *p)
    {
        m_ptr = p;
    }
    const _Type& operator[](const int idx) const 
    {
        return m_ptr[idx];
    }
    _Type& operator[](const int idx)
    {
        return m_ptr[idx];
    }
};

template<typename _Type>
class ChSmartPtr
{
    _Type *m_ptr;
    unsigned int m_size;

    ChSmartPtr(const ChSmartPtr& _p);
    void operator=(const ChSmartPtr& _p);

public:
    ChSmartPtr()
    {
        m_ptr = NULL;
        m_size = 0;
    }
    ChSmartPtr(const unsigned int element_size)
    {
        m_ptr = NULL;
        m_size = 0;
        smartptr_alloc(element_size);
    }
    ~ChSmartPtr()
    {
        smartptr_free();
    }

    _Type* smartptr_alloc(const unsigned int element_size)
    {
        if (element_size <= m_size)
            return m_ptr;

        smartptr_free();

        m_ptr = NEW_NO_EXCEPTION _Type[element_size];
        if (m_ptr != NULL)
            m_size = element_size;
        return m_ptr;
    }
    void smartptr_free()
    {
        if (m_ptr)
        {
            delete[] m_ptr;
            m_ptr = NULL;
            m_size = 0;
        }
    }
    void attach_pointer(_Type *ptr, const unsigned int element_size)
    {
        smartptr_free();

        m_ptr = ptr;
        m_size = element_size;
    }
    bool is_null()
    {
        return m_ptr == NULL;
    }

    operator const _Type *() const
    {
        return m_ptr;
    }
    operator _Type *()
    {
        return m_ptr;
    }
    const _Type& operator[](const int idx) const 
    {
        _MYASSERT(m_ptr);

        return m_ptr[idx];
    }
    _Type& operator[](const int idx)
    {
        _MYASSERT(m_ptr);

        return m_ptr[idx];
    }
};

//////////////////////////////////////////////////////////////////////////

class ChPoint
{
public:
    int x;
    int y;

    ChPoint() { x = y = 0; }
    ChPoint(const int vx, const int vy)       
    { 
        Set(vx, vy); 
    }
    ChPoint(const ChPoint& p)
    { 
        *this = p; 
    }
    ~ChPoint() { }

    void Set(const int vx, const int vy)    
    { 
        x = vx;
        y = vy; 
    }

    ChPoint& operator=(const ChPoint& p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }
    ChPoint operator-()
    {
        ChPoint t(-x, -y);
        return t;
    }
    ChPoint& operator+=(const ChPoint& p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }
    ChPoint& operator-=(const ChPoint& p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    template<class _Type>
    ChPoint operator*(const _Type v) const
    {
        ChPoint t;
        t.x = x * v;
        t.y = y * v;
        return t;
    }
    template<class _Type>
    ChPoint operator/(const _Type v) const
    {
        ChPoint t;
        t.x = x / v;
        t.y = y / v;
        return t;
    }
    template<class _Type>
    ChPoint& operator*=(const _Type v)
    {
        x *= v;
        y *= v;
        return *this;
    }
    template<class _Type>
    ChPoint& operator/=(const _Type v)
    {
        x /= v;
        y /= v;
        return *this;
    }
    ChPoint operator>>(const int v) const 
    {
        ChPoint t;
        t.x = x >> v;
        t.y = y >> v;
        return t;
    }
    ChPoint& operator>>=(const int v)
    {
        x >>= v;
        y >>= v;
        return *this;
    }
    ChPoint operator<<(const int v) const
    {
        ChPoint t;
        t.x = x << v;
        t.y = y << v;
        return t;
    }
    ChPoint& operator<<=(const int v)
    {
        x <<= v;
        y <<= v;
        return *this;
    }
    bool operator==(const ChPoint& p) const
    {
        return (x == p.x) && (y == p.y);
    }
    bool operator!=(const ChPoint& p) const
    {
        return (x != p.x) && (y != p.y);
    }
    int distance(const ChPoint& p) const 
    {
        return ((p.x - x) * (p.x - x)) + ((p.y - y) * (p.y - y));
    }
    int norm1() const
    {
        return abs(x) + abs(y);
    }
    int norm2() const
    {
        return (x * x) + (y * y);
    }
    friend ChPoint operator+(const ChPoint& a, const ChPoint& b);
    friend ChPoint operator-(const ChPoint& a, const ChPoint& b);
};

ChPoint operator+(const ChPoint& a, const ChPoint& b);
ChPoint operator-(const ChPoint& a, const ChPoint& b);

struct ChRect
{
    int left;
    int top;
    int right;
    int bottom;

    ChRect() { left = top = right = bottom = 0; }
    ChRect(int l, int t, int r, int b) { left = l; top = t; right = r; bottom = b; }

    ChPoint LeftTop() { return ChPoint(left, top); }
    ChPoint RightBottom() { return ChPoint(right, bottom); }
    void SetRect(int l, int t, int r, int b) { left = l; top = t; right = r; bottom = b; }
};

//////////////////////////////////////////////////////////////////////////

int ch_FileExist(LPCTSTR fname);
int ch_CopyFile(LPCTSTR src_name, LPCTSTR dest_name);
int ch_GetFileSize(LPCTSTR fname);
LPTSTR ch_TimeFormat(LONGLONG tmCode, LPTSTR ch_TimeFormat, int lenStr);
void ch_GetNowTimeStr(LPTSTR strTime, int lenStr);
int ch_NeedScaleFix(int& new_x, int& new_y, int width, int height);
bool ch_NeedScale(int& new_x, int& new_y, int width, int height);
#ifndef UNIX_OS
int ch_ExploreFolder(LPCTSTR filter, LPCTSTR path, std::vector<std_tstring>& file_list, bool bDirOnly = false, bool bRecursive = false);
int ch_GetSelectedFolder(HWND hWnd, LPTSTR szFolderOut, int nLenFolderOut);
#endif

void *ch_GetMem1D(int size_of_element, int d1);
void *ch_GetMem2D(int size_of_element, int d1, int d2);
void *ch_GetMem3D(int size_of_element, int d1, int d2, int d3);
void ch_FreeMem(void *p);

//////////////////////////////////////////////////////////////////////////

template<class _T>
inline const _T ch_Min(const _T& a, const _T& b)
{
    return a < b ? a : b;
}

template<class _T>
inline const _T ch_Max(const _T& a, const _T& b)
{
    return a > b ? a : b;
}

template<class _T>
inline const _T ch_Min3(const _T& a, const _T& b, const _T& c)
{
    return ch_Min(ch_Min(a, b), c);
}

template<class _T>
inline const _T ch_Min4(const _T& a, const _T& b, const _T& c, const _T& d)
{
    return ch_Min(ch_Min(ch_Min(a, b), c), d);
}

template<class _T>
inline const _T ch_Max3(const _T& a, const _T& b, const _T& c)
{
    return ch_Max(ch_Max(a, b), c);
}

template<class _T>
inline const _T ch_Max4(const _T& a, const _T& b, const _T& c, const _T& d)
{
    return ch_Max(ch_Max(ch_Max(a, b), c), d);
}

template<class _T>
inline const int ch_Sign(const _T& a)
{
    return a < 0 ? -1 : 1;
}

template<class _T>
inline const _T ch_Abs(const _T& a)
{
    return a < 0 ? -a : a;
}

template<class _T>
inline void ch_Swap(_T& a, _T& b)
{
    _T t;
    t = a;
    a = b;
    b = t;
}

template<class _T>
inline const _T ch_Square(const _T& a)
{
    return a * a;
}

template<class _T>
inline const _T FitInRange(const _T& value, const _T& min, const _T& max)
{
    _T result = value;
    if (result < min)
        result = min;
    else if (result > max)
        result = max;

    return result;
}

#if defined(ANDROID_ARM) || defined(IOS_ARM)

inline int ch_Round(float x)
{
    if (x >= 0.0f)
        return (int)(x + 0.5f);
    else
        return (int)(x - 0.5f);
}

inline int ch_Round(double x)
{
    if (x >= 0)
        return (int)(x + 0.5);
    else
        return (int)(x - 0.5);
}

#else

// SSE2 is default supported for x86
#if !defined(_M_ARM ) && !defined(_M_SOFT )
#include <emmintrin.h>
#endif

inline int ch_Round(float x)
{
#if !defined(_M_ARM ) && !defined(_M_SOFT )
    return _mm_cvtss_si32(_mm_load_ss(&x));
#else
    return (int)(x + 0.5f);
#endif
}

inline int ch_Round(double x)
{
#if !defined(_M_ARM ) && !defined(_M_SOFT )
    return _mm_cvtsd_si32(_mm_load_sd(&x));
#else
    return (int)(x + 0.5);
#endif
}

#endif

inline __int64 ch_RoundToInt64(float x)
{
    if(x >= 0.0f)
        return (__int64)(x + 0.5f);
    else
        return (__int64)(x - 0.5f);
}

inline __int64 ch_RoundToInt64(double x)
{
    if(x >= 0)
        return (__int64)(x + 0.5);
    else
        return (__int64)(x - 0.5);
}

// Portable float value checking
inline bool ch_IsValidNumber(float f)
{
    // Not NAN or infinity: Exponent != 0xff

    unsigned int value = *((unsigned int *)&f);
    return ((value & 0x7f800000) != 0x7f800000);
}

inline bool ch_IsInfinity(float f)
{
    // infinity: Exponent = 0xff, Significand = 0

    unsigned int value = *((unsigned int *)&f);
    return ((value & 0x7fffffff) == 0x7f800000);
}

inline bool ch_IsNan(float f)
{
    // NAN: Exponent = 0xff, Significand != 0

    unsigned int value = *((unsigned int *)&f);
    return ((value & 0x7f800000) == 0x7f800000) && ((value & 0x7fffffff) != 0x7f800000);
}

inline std_tstring ch_GetFilePath(LPCTSTR _path) {
    _TCHAR drive[16];
    _TCHAR dir[256];
    _TCHAR fname[256];
    _TCHAR ext[16];
#ifdef UNIX_OS
    _tsplitpath(_path, drive, dir, fname, ext);
    _TCHAR ret_str[256];
    _stprintf(ret_str, _T("%s%s"), drive, dir);
#else
    _tsplitpath_s(_path, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext));
    _TCHAR ret_str[256];
    _stprintf_s(ret_str, _countof(ret_str), _T("%s%s"), drive, dir);
#endif
    return std_tstring(ret_str);
}

inline std_tstring ch_GetFileBaseName(LPCTSTR _path) {
    _TCHAR drive[16];
    _TCHAR dir[256];
    _TCHAR fname[256];
    _TCHAR ext[16];
#ifdef UNIX_OS
    _tsplitpath(_path, drive, dir, fname, ext);
#else
    _tsplitpath_s(_path, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext));
#endif
    return std_tstring(fname);
}

inline std_tstring ch_GetFileExtName(LPCTSTR _path) {
    _TCHAR drive[16];
    _TCHAR dir[256];
    _TCHAR fname[256];
    _TCHAR ext[16];
#ifdef UNIX_OS
    _tsplitpath(_path, drive, dir, fname, ext);
#else
    _tsplitpath_s(_path, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext));
#endif
    return std_tstring(ext);
}

inline std_tstring ch_MakeFileFullPath(LPCTSTR _path, LPCTSTR _fullname) {
    if (_path == NULL)
        return std_tstring(_fullname);

    _TCHAR ret_str[256];
#ifdef UNIX_OS
    _stprintf(ret_str, _T("%s%s%s"), _path,
        (_path[_tcslen(_path) - 1] == '\\' || _path[_tcslen(_path) - 1] == '/') ? _T("") : _T("\\"),
        _fullname);
#else
    _stprintf_s(ret_str, _countof(ret_str), _T("%s%s%s"), _path,
        (_path[_tcslen(_path) - 1] == '\\' || _path[_tcslen(_path) - 1] == '/') ? _T("") : _T("\\"),
        _fullname);
#endif
    return std_tstring(ret_str);
}

inline std_tstring ch_GetFileFullName(LPCTSTR _path) {
    _TCHAR drive[16];
    _TCHAR dir[256];
    _TCHAR fname[256];
    _TCHAR ext[16];
#ifdef UNIX_OS
    _tsplitpath(_path, drive, dir, fname, ext);
#else
    _tsplitpath_s(_path, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext));
#endif
    std_tstring ret_str = fname;
    ret_str += ext;
    return std_tstring(ret_str);
}

inline std_tstring ch_ReplacePath(LPCTSTR _fullpath, LPCTSTR _newpath)
{
    if (_newpath == NULL)
        return std_tstring(_fullpath);

    _TCHAR drive[16];
    _TCHAR dir[256];
    _TCHAR fname[256];
    _TCHAR ext[16];
#ifdef UNIX_OS
    _tsplitpath(_fullpath, drive, dir, fname, ext);
    _TCHAR ext_fn[256];
    _TCHAR lastch = _newpath[_tcslen(_newpath) - 1];
    _stprintf(ext_fn, _T("%s%s%s%s"), _newpath, (lastch == '\\' || lastch == '/') ? _T("") : _T("\\"), fname, ext);
#else
    _tsplitpath_s(_fullpath, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext));
    _TCHAR ext_fn[256];
    _TCHAR lastch = _newpath[_tcslen(_newpath) - 1];
    _stprintf_s(ext_fn, _countof(ext_fn), _T("%s%s%s%s"), _newpath, (lastch == '\\' || lastch == '/') ? _T("") : _T("\\"), fname, ext);
#endif
    return std_tstring(ext_fn);
}

inline std_tstring ch_ExtendFileName(LPCTSTR _path, LPCTSTR _tag, LPCTSTR _ext) {
    _TCHAR drive[16];
    _TCHAR dir[256];
    _TCHAR fname[256];
    _TCHAR ext[16];
#ifdef UNIX_OS
    _tsplitpath(_path, drive, dir, fname, ext);
    _TCHAR ext_fn[256];
    _stprintf(ext_fn, _T("%s%s%s%s%s%s.%s"), drive, dir,
        (dir[_tcslen(dir) - 1] == '\\' || dir[_tcslen(dir) - 1] == '/') ? _T("") : _T("\\"),
        fname,
        _tag == NULL ? _T("") : _T("-"),
        _tag == NULL ? _T("") : _tag,
        _ext == NULL ? ext : _ext);
#else
    _tsplitpath_s(_path, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext));
    _TCHAR ext_fn[256];
    _stprintf_s(ext_fn, _countof(ext_fn), _T("%s%s%s%s%s%s.%s"), drive, dir,
        (dir[_tcslen(dir) - 1] == '\\' || dir[_tcslen(dir) - 1] == '/') ? _T("") : _T("\\"),
        fname,
        _tag == NULL ? _T("") : _T("-"),
        _tag == NULL ? _T("") : _tag,
        _ext == NULL ? ext : _ext);
#endif
    return std_tstring(ext_fn);
}

#if !defined (UNIX_OS) && !defined (_WINRT_METRO)
inline std_tstring ch_GetSpecialFolder(int nFolder, LPCTSTR pSubFolder = NULL)
{
    _TCHAR szFolder[MAX_PATH];
    SHGetSpecialFolderPath(NULL, szFolder, nFolder, TRUE);
    if(pSubFolder) {
        _tcscat_s(szFolder, _countof(szFolder), _T("\\"));
        _tcscat_s(szFolder, _countof(szFolder), pSubFolder);
        _tmkdir(szFolder);
    }
    return std_tstring(szFolder);
}
#endif

class CTimeCost
{
    clock_t tm_start;
    clock_t tm_end;
public:
    CTimeCost() { }
    ~CTimeCost() { }

    void start()
    {
        tm_start = clock();
    }
    void end()
    {
        tm_end = clock();
    }
    clock_t peek_clocks()
    {
        return clock() - tm_start;
    }
    clock_t get_clocks()
    {
        return tm_end - tm_start;
    }
    float get_seconds()
    {
        return float(tm_end - tm_start) / CLOCKS_PER_SEC;
    }
};

template<typename _Type>
class FixQueue
{
    int m_nSize;
    _Type *m_pVec;

public:
    FixQueue(const int size)
    {
        m_nSize = size;
        m_pVec = NULL;
        _NEW_PTRS(m_pVec, _Type, m_nSize);
    }
    ~FixQueue()
    {
        _DELETE_PTRS(m_pVec);
    }

    int size() { return m_nSize; }
    void reset(const _Type& v = _Type(0))
    {
        int i;
        for(i = 0; i < m_nSize; i++)
            m_pVec[i] = v;
    }
    void push_back(const _Type& elem)
    {
        int i;
        for(i = 0; i < m_nSize - 1; i++)
            m_pVec[i] = m_pVec[i + 1];
        m_pVec[m_nSize - 1] = elem;
    }
    _Type& operator[](const int idx)
    {
        return m_pVec[idx];
    }
    const _Type operator[](const int idx) const
    {
        return m_pVec[idx];
    }
    const _Type average() const
    {
        _Type avg = _Type(0);
        int i;
        for(i = 0; i < m_nSize; i++)
            avg += m_pVec[i];
        return avg / m_nSize;
    }
};

typedef ChAutoPtr_Allocator AdaptiveBufferAllocator;
template<typename _Type, AdaptiveBufferAllocator allocator = CHAP_New>
class AdaptiveBuffer
{
    AdaptiveBuffer(const AdaptiveBuffer &);
    AdaptiveBuffer& operator=(const AdaptiveBuffer &);
public:
    _Type *mp_data;
    int m_size;

    AdaptiveBuffer()
    {
        mp_data = NULL;
        m_size = 0;
    }

    AdaptiveBuffer(const int size)
    {
        mp_data = NULL;
        m_size = 0;

        RequestBuffer(size);
    }

    ~AdaptiveBuffer()
    {
        FreeBuffer();
    }

    void RequestBuffer(const int size)
    {
        if (size > m_size)
        {
            if (allocator == CHAP_New)
            {
                _NEW_PTRS(mp_data, _Type, size);
            }
            else 
            {
                _ALIGNED_MALLOC_PTR(mp_data, _Type, size);
            }
            m_size = size;
        }

        // else, do nothing and m_data still holds m_size elements
    }

    void FreeBuffer()
    {
        if (allocator == CHAP_New)
        {
            _DELETE_PTRS(mp_data);
        }
        else
        {
            _ALIGNED_FREE_PTR(mp_data);
        }
        m_size = 0;
    }

    const _Type& operator[](const int index) const 
    {
        return mp_data[index];
    }

    _Type& operator[](const int index)
    {
        return mp_data[index];
    }
};

typedef AdaptiveBuffer<BYTE, CHAP_Malloc> AlignedByteBuffer;
typedef AdaptiveBuffer<int, CHAP_Malloc> AlignedIntBuffer;
typedef AdaptiveBuffer<float, CHAP_Malloc> AlignedFloatBuffer;
typedef AdaptiveBuffer<short, CHAP_Malloc> AlignedShortBuffer;

struct ImageScale
{
    float x;
    float y;

    ImageScale()
    {
        Reset();
    }

    ImageScale(const float _x, const float _y)
    {
        x = _x;
        y = _y;
    }

    void Reset()
    {
        x = 1.0f;
        y = 1.0f;
    }

    bool IsNotScale() const
    {
        return (x == 1.0f && y == 1.0f);
    }
};

void ShowMemoryUsage(LPCTSTR title = NULL);

inline float frand()
{
    float rand_value = (float)rand();
    rand_value /= (float)RAND_MAX;

    return rand_value;
}

template<class _TYPE>
void VectorToPointer(_TYPE* p_data, const std::vector<_TYPE> &vector_data)
{
    if (p_data == NULL)
        return;

    for (int i = 0; i < (int)vector_data.size(); i++)
    {
        p_data[i] = vector_data[i];
    }
}

template<class _TYPE>
void PointerToVector(std::vector<_TYPE> &vector_data, const _TYPE* p_data, const int size)
{
    if (p_data == NULL)
        return;

    vector_data.clear();
    for (int i = 0; i < size; i++)
    {
        vector_data.push_back(p_data[i]);
    }
}

#ifdef UNIX_OS
#include "PThreadTool.h"
#else

#ifdef _WINRT_METRO
#include "ThreadTool_Metro.h"
#else
#include "ThreadTool.h"
#endif

#endif
