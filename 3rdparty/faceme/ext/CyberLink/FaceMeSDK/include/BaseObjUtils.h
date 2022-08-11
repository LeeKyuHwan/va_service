/**
* @file BaseObjUtils.h
* The utilities for basic object interface of FaceMe SDK
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef _FACEME_BASE_OBJ_UTILS_H_
#define _FACEME_BASE_OBJ_UTILS_H_

#ifdef _WIN32
#include <windows.h>
#include <atlbase.h>
#else
//#include <atomic>
//#include <stdatomic.h>

#ifndef __cplusplus
# include <stdatomic.h>
#else
# include <atomic>
# define _Atomic(X) std::atomic< X >
#endif
#endif

#include "IBaseObj.h"

namespace FaceMeSDK
{
	namespace BaseObject
	{
		template <class T>
		class scoped_refptr
		{
		public:
			scoped_refptr() : p(nullptr) {}

			scoped_refptr(T* ptr) : p(ptr)
			{
				if (p)
					p->AddRef();
			}

			scoped_refptr(const scoped_refptr<T>& r) : p(r.p)
			{
				if (p)
					p->AddRef();
			}

			template <typename U>
			scoped_refptr(const scoped_refptr<U>& r) : p(r.get())
			{
				if (p)
					p->AddRef();
			}

			scoped_refptr(scoped_refptr<T>&& r) : p(r.release())
			{
			}

			template <typename U>
			scoped_refptr(scoped_refptr<U>&& r) : p(r.release()) {}

			~scoped_refptr()
			{
				if (p)
					p->Release();
			}

			T* get() const
			{
				return p;
			}

			operator T*() const
			{
				return p;
			}

			bool operator!() const
			{
				return (p == nullptr);
			}

			T* operator->() const
			{
				return p;
			}

			T* release()
			{
				T* retVal = p;
				p = nullptr;
				return retVal;
			}

			scoped_refptr<T>& operator=(T* ptr)
			{
				// AddRef first so that self assignment should work
				if (ptr)
					ptr->AddRef();
				if (p)
					p->Release();
				p = ptr;
				return *this;
			}

			scoped_refptr<T>& operator=(const scoped_refptr<T>& r)
			{
				return *this = r.p;
			}

			template <typename U>
			scoped_refptr<T>& operator=(const scoped_refptr<U>& r)
			{
				return *this = r.get();
			}

			scoped_refptr<T>& operator=(scoped_refptr<T>&& r)
			{
				scoped_refptr<T>(std::move(r)).swap(*this);
				return *this;
			}

			template <typename U>
			scoped_refptr<T>& operator=(scoped_refptr<U>&& r)
			{
				scoped_refptr<T>(std::move(r)).swap(*this);
				return *this;
			}

			void swap(T** pp)
			{
				T* ptr = p;
				p = *pp;
				*pp = ptr;
			}

			void swap(scoped_refptr<T>& r)
			{
				swap(&r.p);
			}

		public:
			T * p;
		};

		template <typename T, typename ...T1>
		class RefCountedObject : public T, public T1...
		{
		public:
			RefCountedObject() : m_refCount(0) {}

			int32_t AddRef() const override
			{
				//return std::atomic_fetch_add_explicit(&m_refCount, 1, std::memory_order_relaxed)+1;
#ifdef _WIN32
				return ::InterlockedIncrement(reinterpret_cast<volatile LONG*>(&m_refCount));
#else
				return __sync_add_and_fetch(&m_refCount, 1);
#endif
			}

			int32_t Release() const override
			{
				//auto cnt = std::atomic_fetch_sub_explicit(&m_refCount, 1, std::memory_order_release)-1;
#ifdef _WIN32
				auto cnt = (int32_t)::InterlockedDecrement(reinterpret_cast<volatile LONG*>(&m_refCount));
#else
				auto cnt = (int32_t)__sync_sub_and_fetch(&m_refCount, 1);
#endif
				if (cnt == 0)
				{
					//std::atomic_thread_fence(std::memory_order_acquire);
					delete this;
				}

				return cnt;
			}
			
			virtual bool QueryInterface(REFOID iid, void **ppv) override
			{
				if (iid == OID_RefCountedObject)
				{
					return GetInterface(static_cast<IRefCountedObject *>(static_cast<T *>(this)), ppv);
				}

				return false;
			}

		protected:
			virtual ~RefCountedObject() {}

			bool GetInterface(IRefCountedObject *pObj, void **ppv)
			{
				if (ppv == nullptr || pObj == nullptr)
					return false;

				*ppv = pObj;
				pObj->AddRef();
				return true;
			}

			//std::atomic_int m_refCount = {0};
			mutable volatile int32_t m_refCount;
		};
	}
}

#endif // _FACEME_BASE_OBJ_UTILS_H_


