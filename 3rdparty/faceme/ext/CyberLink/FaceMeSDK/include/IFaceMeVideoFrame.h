/**
 * @file IFaceMeVideoFrame.h
 * The private interface of CyberLink Video Frame.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FR_IFACEMEVIDEOFRAME_H__
#define __FR_IFACEMEVIDEOFRAME_H__
#include "FaceMeVideoFrameDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink FaceMe Video Frame
	* @interface IFaceMeVideoFrame
	*/
	class IFaceMeVideoFrame : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeVideoFrame() {}

		/**
		* Object identifiers of FaceMe Video Frame.
		*
		* @enum FaceMeVideoFrameOIDs
		*/
		enum FaceMeVideoFrameOIDs
		{
			OID_FaceMeVideoFrame = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe Video Frame
		};

		/**
		* Get information of video frame.
		*
		* @param[out] info  Retrieve the information of video frame.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN GetInfo(FR_VideoFrameAllocateInfo *info) = 0;

		/**
		* Get native pointer associate to the current video frame.
		*
		* @param[out] nativeFrame  Retrieve the native pointer associate to the current video frame.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetNativeFrame(void **nativeFrame) = 0;
	};
}

#endif // __FR_IFACEMEVIDEOFRAME_H__