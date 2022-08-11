/**
* @file CLThreadMgr.h
* Thread Manager
*
* CyberLink FaceMe (R) SDK
* Copyright (C) 2019 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef __CLTHREAD_MGR_H__
#define __CLTHREAD_MGR_H__

#include <memory>
#include <thread>
#include <string>

//#include <process.h>

#include "ThreadUtil.h"

namespace cl_utility
{
	namespace thread
	{
		//#define DEF_KILL_THREAD_TIMEOUT 10000
		//#define DEF_KILL_THREAD_POOL_TIMEOUT 100000
#define DEF_KILL_THREAD_TIMEOUT CLUTH_INFINITE
#define DEF_KILL_THREAD_POOL_TIMEOUT CLUTH_INFINITE

#define MAX_THREAD_COUNT 1000
		class CLThread
		{
			CLThread(const CLThread&);
			const CLThread& operator=(const CLThread&);
		public:
			typedef std::shared_ptr<CLThread> SharedPtr;

		public:
			CLThread();
			CLThread(const char *szThreadName);
			virtual ~CLThread();

			static void Sleep(uint32_t ms);
			static void TryYield();
			static uint32_t CurrentThreadId();

			//
			// Start thread running  - error if already running
			//
			bool	StartThread();

			//
			// Stop the thread
			//
			bool	StopThread();

			//
			// Invoke stop the thread signal
			//
			void InvokeExitThread();

			//
			// ThreadExists
			//
			bool	ThreadExists() const;

			void ThreadEntrance();

		protected:
			CLEvent	m_evExitThread;
			std::thread m_thread;
			std::string m_strThreadName;

			///////////////////////////////////////////////////////////////////////////////

			//
			// Thread will run this function on startup
			//
			virtual void ThreadProc() = 0;

			//
			//	Get Thread ID
			//
			uint32_t GetThreadID();

			//
			// Check thread exit
			//
			bool CheckExitThreadEvent();

		private:

			void SetThreadName(const char* threadName);
			void SetThreadName(std::thread* thread, const char* threadName);
		};
	}
}
#endif //__CLTHREAD_MGR_H__