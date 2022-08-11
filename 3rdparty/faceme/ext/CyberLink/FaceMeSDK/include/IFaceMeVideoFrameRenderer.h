/**
 * @file IFaceMeVideoFrameRenderer.h
 * The private interface of CyberLink Video Frame Renderer.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FR_IFACEMEVIDEOFRAMERENDERER_H__
#define __FR_IFACEMEVIDEOFRAMERENDERER_H__
#include "FaceMeVideoFrameRendererDef.h"
#include "IFaceMeVideoFrame.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink FaceMe Video Frame Allocator
	* @interface IFaceMeFrameAllocator
	*/
	class IFaceMeVideoFrameRenderer : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeVideoFrameRenderer() {}

		/**
		* Object identifiers of FaceMe Manager.
		*
		* @enum FaceMeDataManagerOIDs
		*/
		enum FaceMeVideoFrameRendererOIDs
		{
			OID_FaceMeVideoFrameRenderer = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe Video Frame Renderer
		};

		// General
		/**
		* Initialize FaceMe Video Frame Renderer.
		*
		* @param[in] config The configuration for video frame renderer.
		* @param[in] licenseInfo The license information provided by CyberLink. The format of license information is a raw buffer.
		* @param[in] length Length of the license information.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_CONSTRAIN_HARDWARE</code>: License hardware is constrained.
		*/
		virtual FR_RETURN Initialize(const FR_VideoFrameRendererConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

		/**
		* Finalize FaceMe SDK and release corresponding resources.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN Finalize() = 0;

		/**
		* Render Frame and fill frame buffer.
		*
		* @param[in] config The configuration for video frame render.
		* @param[in] frame A pointer of IFaceMeVideoFrame instance represents the allocated frame.
		* @param[in, out] frameData The host or gpu memory of the frame.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN RenderBuffer(const FR_VideoFrameRenderConfig *config, IFaceMeVideoFrame *frame, void* frameData) = 0;

		/**
		* Get current status of video frame renderer.
		*
		* @param[out] status  Retrieve the status of video frame allocator.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN GetStatus(FR_VideoFrameRendererStatus *status) = 0;
	};
}

#endif // __FR_IFACEMEVIDEOFRAMERENDERER_H__