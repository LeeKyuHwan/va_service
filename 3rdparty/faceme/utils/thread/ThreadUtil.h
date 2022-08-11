/**
* @file ThreadUtil.h
* Thread utilities
*
* CyberLink FaceMe (R) SDK
* Copyright (C) 2019 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifdef __cplusplus
#include <stdint.h>
#include <cassert>
#include <condition_variable>
#include <mutex>

namespace cl_utility
{
	namespace thread
	{
#define CLUTH_INFINITE            ((int32_t)0xFFFFFFFF)  // Infinite timeout

		/**
		* Critical Section Wrapper
		* @class CLCritSec
		*/
		class CLCritSec
		{
			CLCritSec(const CLCritSec&);
			const CLCritSec& operator=(const CLCritSec&);
		public:
			CLCritSec() {}

			void Lock()
			{
				m_mutex.lock();
			}

			void Unlock()
			{
				m_mutex.unlock();
			}

		private:
			std::recursive_mutex m_mutex;
		};

		/**
		* Simple Session Locker
		*
		* Locks a critical section, and unlocks it automatically when the lock goes out of scope.
		* @class CLAutoLock
		*/
		class CLAutoLock
		{
			CLAutoLock(const CLAutoLock&);
			const CLAutoLock& operator=(const CLAutoLock&);
		public:

			CLAutoLock(CLCritSec *plock)
			{
				assert(plock);
				m_pLock = plock;
				m_pLock->Lock();
			}

			~CLAutoLock()
			{
				assert(m_pLock);
				m_pLock->Unlock();
			}

		private:
			CLCritSec * m_pLock;
		};


		/**
		* Wrapper for event objects
		*
		* Locks a critical section, and unlocks it automatically when the lock goes out of scope.
		* @class CLEvent
		*/
		class CLEvent
		{
			CLEvent(const CLEvent&);
			const CLEvent& operator=(const CLEvent&);
		public:
			typedef std::shared_ptr<CLEvent> SharedPtr;
			

		public:
			CLEvent(bool fManualReset = false, bool bInitialState = false)
				: m_isManualReset(fManualReset)
				, m_eventStatus(bInitialState)
			{
			}

			~CLEvent()
			{
				m_eventStatus = true;
				m_cv.notify_all();
			}

			void Set()
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_eventStatus = true;
				m_cv.notify_all();
			}

			bool Wait(int32_t timeoutMs = CLUTH_INFINITE)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				auto predi = [this] { return m_eventStatus; };
				if (timeoutMs == CLUTH_INFINITE)
				{
					m_cv.wait(lock, predi);
				}
				else {
					bool isTimeout =
						!m_cv.wait_for(lock, std::chrono::milliseconds(timeoutMs), predi);
					if (isTimeout) return false;
				}

				if (!m_isManualReset) m_eventStatus = false;

				return true;
			}

			void Reset()
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_eventStatus = false;
			}

			bool Check()
			{
				return Wait(0);
			}

		private:
			std::mutex m_mutex;
			std::condition_variable m_cv;
			const bool m_isManualReset;
			bool m_eventStatus;
		};
	}
}
#endif
