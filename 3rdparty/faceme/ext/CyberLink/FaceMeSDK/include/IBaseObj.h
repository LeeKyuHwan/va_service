/**
* @file IBaseObj.h
* The basic object interface for FaceMeSDK
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef _FACEME_IBASEOBJ_H_
#define _FACEME_IBASEOBJ_H_

#include <stdint.h>

#define FR_SAFE_RELASE(p) if ((p) != nullptr) {(p)->Release(); (p) = nullptr;}

namespace FaceMeSDK
{
	namespace BaseObject
	{

		typedef uint32_t OID; //!< Object identifier.
		typedef const OID& REFOID;  //!< Reference of the object identifier.

		/**
		* Basic object identifiers of RefCountedObject.
		* @enum RefCountedObjectOIDs
		*/
		enum RefCountedObjectOIDs
		{
			OID_RefCountedObject = 0	//!< Object identifier of RefCountedObject.
		};

		/**
		* Reference counted Object
		* @interface IRefCountedObject
		*/
		class IRefCountedObject
		{
		public:
			virtual ~IRefCountedObject() {}

			/**
			* Increments the reference count for an interface on an object. 
			* This method should be called for every new copy of a pointer to an interface on an object.
			*
			* @return The method returns the new reference count.
			*/
			virtual int32_t AddRef() const = 0;

			/**
			* Decrements the reference count for an interface on an object.
			*
			* @return The method returns the new reference count.
			* @remark When the reference count on an object is changed to zero, Release() will let the interface pointer free itself. 
			*/
			virtual int32_t Release() const = 0;

			/**
			* Retrieves pointers to the supported interfaces on an object.
			*
			* @param[in] iid Object identifier of interface to be queried.
			* @param[out] ppv Pointer to a buffer that receives a pointer to the object whose interface is queried.
			* @return This method returns true if the interface is supported.
			*/
			virtual bool QueryInterface(REFOID iid, void **ppv) = 0;
		};

		/**
		* Object Id of String Object.
		* @enum CharStringOIDs
		*/
		enum CharStringOIDs
		{
			OID_CharString = (OID_RefCountedObject + 1),		//!< Object Identifier of Character String
			OID_MutableCharString = (OID_RefCountedObject + 2),	//!< Object Identifier of Mutable Character String
		};

		/**
		* Character String Object
		* @interface ICharString
		*/
		class ICharString : public IRefCountedObject
		{
		public:
			virtual ~ICharString() {}

			/**
			* Get an utf-8 encoded string.
			*
			* @return A pointer of an utf-8 encoded string is returned.
			*/
			virtual const char *GetString() const = 0;

			/**
			* Get length of the utf-8 encoded string.
			*
			* @return Length of the string.
			*/
			virtual uint32_t GetLength() const = 0;
		};

		/**
		* Mutable Character String Object
		* @interface IMutableCharString
		*/
		class IMutableCharString : public ICharString
		{
		public:
			virtual ~IMutableCharString() {}

			/**
			* Set an utf-8 encoded string.
			*
			* @param[in] str A pointer of an utf-8 encoded string.
			*/
			virtual void SetString(const char *str) = 0;

			/**
			* Get an utf-8 encoded string buffer for access.
			*
			* @return A pointer of an utf-8 encoded string buffer is returned.
			*/
			virtual char *GetAccess() = 0;

			/**
			* Get length of the utf-8 encoded string.
			*
			* @return Length of the string.
			*/
			virtual uint32_t GetSize() = 0;
		};

		/**
		* Object identifiers of Blob.
		*
		* @enum BlobOIDs
		*/
		enum BlobOIDs
		{
			OID_Blob = (OID_RefCountedObject + 1),			//!< Object Identifier of Blob.
			OID_MutableBlob = (OID_RefCountedObject + 2),	//!< Object Identifier of Mutable Blob.
		};

		/**
		* Blob Object
		* @interface IBlob
		*/
		class IBlob : public IRefCountedObject
		{
		public:
			virtual ~IBlob() {}

			/**
			* Get a pointer of an data from Blob.
			*
			* @return A pointer of an data is returned.
			*/
			virtual const void *GetPtr() const = 0;

			/**
			* Get size of data by bytes.
			*
			* @return The size of the data is returned by bytes.
			*/
			virtual uint32_t GetSize() const = 0;
		};

		/**
		* Mutable Blob Object
		* @interface IMutableBlob
		*/
		class IMutableBlob : public IBlob
		{
		public:
			virtual ~IMutableBlob() {}

			/**
			* Allocate Mutable Buffer.
			*
			* @param[in] len Size of buffer to be allocated.
			*/
			virtual bool Alloc(uint32_t len) = 0;

			/**
			* Free Buffer.
			*/
			virtual void Free() = 0;

			/**
			* Get data buffer for access.
			*
			* @return A pointer of an data buffer is returned.
			*/
			virtual void *GetAccess() = 0;
		};
	}
}

#endif // _FACEME_IBASEOBJ_H_