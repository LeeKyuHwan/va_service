/**
* @file CLThreadMgr.cpp
* Implementation of thread manager
*
* CyberLink FaceMe (R) SDK
* Copyright (C) 2019 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#include "CLThreadMgr.h"

namespace cl_utility
{
	namespace thread
	{

#ifdef _WIN32
#include <windows.h> 
#include <objbase.h>

		static HRESULT CoInitializeHelper()
		{
			// call CoInitializeEx and tell OLE not to create a window (this
			// thread probably won't dispatch messages and will hang on
			// broadcast msgs o/w).
			//
			// If CoInitEx is not available, threads that don't call CoCreate
			// aren't affected. Threads that do will have to handle the
			// failure. Perhaps we should fall back to CoInitialize and risk
			// hanging?
			//

			// older versions of ole32.dll don't have CoInitializeEx

			HRESULT hr = E_FAIL;
			HINSTANCE hOle = GetModuleHandle(TEXT("ole32.dll"));
			if (hOle)
			{
				typedef bool (STDAPICALLTYPE *PCoInitializeEx)(
					LPVOID pvReserved, uint32_t dwCoInit);
				PCoInitializeEx pCoInitializeEx =
					(PCoInitializeEx)(GetProcAddress(hOle, "CoInitializeEx"));
				if (pCoInitializeEx)
				{
					hr = (*pCoInitializeEx)(0, COINIT_DISABLE_OLE1DDE);
				}
			}
			else
			{
				// caller must load ole32.dll
			}

			return hr;
		}

		const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType; // Must be 0x1000.
			LPCSTR szName; // Pointer to name (in user addr space).
			DWORD dwThreadID; // Thread ID (-1=caller thread).
			DWORD dwFlags; // Reserved for future use, must be zero.
		} THREADNAME_INFO;
#pragma pack(pop)


		static void SetThreadNameById(uint32_t dwThreadID, const char* threadName)
		{
			THREADNAME_INFO info;
			info.dwType = 0x1000;
			info.szName = threadName;
			info.dwThreadID = dwThreadID;
			info.dwFlags = 0;

			__try
			{
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
			}
		}

		void  CLThread::SetThreadName(const char* threadName)
		{
			SetThreadNameById(GetCurrentThreadId(), threadName);
		}

		void  CLThread::SetThreadName(std::thread* thread, const char* threadName)
		{
			DWORD threadId = GetThreadId(static_cast<HANDLE>(thread->native_handle()));
			SetThreadNameById(threadId, threadName);
		}

#else
#include <pthread.h>
		void CLThread::SetThreadName(std::thread* thread, const char* threadName)
		{
#ifdef __APPLE__
            pthread_setname_np(threadName);
#else
            auto handle = thread->native_handle();
			pthread_setname_np(handle, threadName);
#endif
		}

#ifdef __APPLE__
        void CLThread::SetThreadName(const char* threadName)
        {
            pthread_setname_np(threadName);
        }
#else
#include <sys/prctl.h>
		void CLThread::SetThreadName(const char* threadName)
		{
			prctl(PR_SET_NAME, threadName, 0, 0, 0);
		}
#endif
        
#endif

		void CLThread::Sleep(uint32_t ms)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}

		void CLThread::TryYield() 
		{ 
			std::this_thread::yield(); 
		}

		uint32_t CLThread::CurrentThreadId() 
		{
#ifdef _WIN32
			return static_cast<uint32_t>(GetCurrentThreadId());
#else
			return static_cast<uint32_t>(
				std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
		}

		//---------------------------------------------------------------------------//
		// Construction/Destruction
		//---------------------------------------------------------------------------//
		CLThread::CLThread()
			: m_evExitThread(true)
		{

		}

		CLThread::CLThread(const char * szThreadName)
			: m_evExitThread(true)
			, m_strThreadName(szThreadName)
		{
		}

		CLThread::~CLThread()
		{
			StopThread();
		}

		bool CLThread::ThreadExists() const
		{
			return m_thread.joinable();
		}

		bool CLThread::StartThread()
		{
			m_evExitThread.Reset();
			m_thread = std::thread(&CLThread::ThreadEntrance, this);

			if (!m_strThreadName.empty())
			{
				SetThreadName(&m_thread, m_strThreadName.c_str());
			}

			return true;
		}

		bool CLThread::StopThread()
		{
			if (m_thread.joinable())
			{
				InvokeExitThread();

				m_thread.join();
			}

			return true;
		}

		void CLThread::ThreadEntrance()
		{
#ifdef _WIN32
			HRESULT hrCoInit = CoInitializeHelper();
			_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
#endif

#ifdef __APPLE__
            if (!m_strThreadName.empty())
            {
                SetThreadName(&m_thread, m_strThreadName.c_str());
            }
#endif
			ThreadProc();
			
#ifdef _WIN32
			if (SUCCEEDED(hrCoInit))
			{
				CoUninitialize();
			}
#endif
		}

		//
		//	Get Thread ID
		//
		uint32_t CLThread::GetThreadID()
		{
#ifdef _WIN32
			return (uint32_t)GetThreadId(static_cast<HANDLE>(m_thread.native_handle()));
#else
			return static_cast<uint32_t>(
				std::hash<std::thread::id>()(m_thread.get_id()));
#endif
		}

		//
		// Check thread exit
		//
		bool CLThread::CheckExitThreadEvent()
		{
			return m_evExitThread.Check();
		}

		void CLThread::InvokeExitThread()
		{
			m_evExitThread.Set();
		}
	}
}
