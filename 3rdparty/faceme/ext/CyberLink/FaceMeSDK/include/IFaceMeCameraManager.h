/**
 * @file IFaceMeCameraManager.h
 * The interface of CyberLink Camera Manager.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FR_IFACEMECAMERAMGR_H__
#define __FR_IFACEMECAMERAMGR_H__
#include "FaceMeCameraManagerDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink FaceMe Camera Information
	* @interface IFaceMeCameraInfo
	*/
	class IFaceMeCameraInfo : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeCameraInfo() {}

		/**
		* Object identifiers of Camera Information.
		*
		* @enum FaceMeCameraInfoOIDs
		*/
		enum FaceMeCameraInfoOIDs
		{
			OID_FaceMeCameraInfo = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of Cam Device Info.
		};

		/**
		* Get string information.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[out] stringValue An object of string of property identifier. Caller should release this object after use.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetString(const char *propertyId, BaseObject::ICharString **stringValue) = 0;

		/**
		* Get blob information.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[out] blobValue An object of data blob of property identifier. Caller should release this object after use.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetBlob(const char *propertyId, BaseObject::IBlob **blobValue) = 0;

		/**
		* Get object information.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[out] objectValue An object of property identifier. Caller should release this object after use.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetRefObject(const char *propertyId, BaseObject::IRefCountedObject **objectValue) = 0;

		/**
		* Get unsigned integer information.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetUInt32(const char *propertyId, uint32_t *value) = 0;

		/**
		* Get unsigned integer information.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetUInt64(const char *propertyId, uint64_t *value) = 0;

		/**
		* Get float information.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetFloat(const char *propertyId, float *value) = 0;
	};

	/**
	* CyberLink FaceMe Camera Enumerator
	* @interface IFaceMeCameraEnumerator
	*/
	class IFaceMeCameraEnumerator : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeCameraEnumerator() {}

		/**
		* Object identifiers of Camera Enumerator.
		*
		* @enum FaceMeCameraEnumeratorOIDs
		*/
		enum FaceMeCameraEnumeratorOIDs
		{
			OID_FaceMeCameraEnumerator = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of Camera Enumerator.
		};

		// General
		/**
		* Reset to head position.
		*
		**/
		virtual void Reset() = 0;

		/**
		* Move to next position.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NOT_FOUND</code>: The next position is not found.
		**/
		virtual FR_RETURN Next() = 0;

		/**
		* Get current.
		*
		* @param[out] cameraInfo An object of capture device information. Caller should release this object after use.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetCurrent(IFaceMeCameraInfo **cameraInfo) = 0;
	};

	/**
	* CyberLink FaceMe Video Stream Handler
	* @interface IFaceMeVideoStreamHandler
	*
	* @note User should implement handler by derived it.
	*/
	class IFaceMeVideoStreamHandler
	{
	public:
		virtual ~IFaceMeVideoStreamHandler() {}

		/**
		* On Video Frame is arrived.
		*
		* @param[in] frameInfo The information of arrived frame.
		* @param[in] frame The image data of current frame.
		* @param[in] depthMap The depth map data of current frame.
		*/
		virtual void OnVideoFrame(const FR_VideoFrameInfo *frameInfo, const FR_Image *frame, const FR_DepthMap *depthMap) = 0;

		/**
		* On End Of Stream is arrived.
		*
		*/
		virtual void OnEndOfStream() = 0;

		/**
		* On Error is occurred.
		*
		*/
		virtual void OnError(FR_RETURN result) = 0;
	};

	/**
	* CyberLink Camera Capturer
	* @interface IFaceMeCameraCapturer
	*/
	class IFaceMeCameraCapturer : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeCameraCapturer() {}

		/**
		* Object identifiers of Camera Capturer.
		*
		* @enum FaceMeCameraCapturerOIDs
		*/
		enum FaceMeCameraCapturerOIDs
		{
			OID_FaceMeCameraCapturer = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of Camera Capturer.
		};

		// General
		/**
		* Start Capture.
		*
		* @param[in] config The configuration for capture setup.
		* @param[in] handler Caller should keep life-cycle of the handler implemented by caller after started.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN Start(const FR_CaptureConfig *config, IFaceMeVideoStreamHandler *handler) = 0;

		/**
		* Stop Capture.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN Stop() = 0;

		/**
		* Set property.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[in] value The specified type of reference variant value by property identifier.
		* @param[in] args The specified type of reference variant arguments by property identifier.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The property identifier is not found.
		*/
		virtual FR_RETURN SetProperty(const char *propertyId, void *value, void *args) = 0;

		/**
		* Get property.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[out] value Retrieve the specified type of reference variant value by property identifier.
		* @param[out] args Retrieve the specified type of reference variant arguments by property identifier.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The property identifier is not found.
		*/
		virtual FR_RETURN GetProperty(const char *propertyId, void *value, void *args) = 0;
	};

	/**
	* CyberLink FaceMe Image Stream
	* @interface IFaceMeImageStream
	*/
	class IFaceMeImageStream : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeImageStream() {}

		/**
		* Object identifiers of Camera Capturer.
		*
		* @enum FaceMeCameraCapturerOIDs
		*/
		enum FaceMeCameraCapturerOIDs
		{
			OID_FaceMeImageStream = (IFaceMeCameraCapturer::FaceMeCameraCapturerOIDs::OID_FaceMeCameraCapturer + 1)	//!< Object identifier of Image Stream.
		};

		/**
		* Add Image buffer to the stream.
		*
		* @param[in] image The image data of new frame.
		* @param[out] pFrameIndex Retrieve a index of new frame.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN AddImage(const FR_Image *image, int64_t *pFrameIndex) = 0;

		/**
		* Add Image buffer to the stream.
		*
		* @param[in] image The image data of new frame.
		* @param[out] pFrameIndex Retrieve a index of new frame.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN AddImage(const char *imagePath, int64_t *pFrameIndex) = 0;
	};

	/**
	* CyberLink FaceMe Image Stream Handler
	* @interface IFaceMeImageStreamHandler
	*
	* @note User should implement handler by derived it.
	*/
	class IFaceMeImageStreamHandler
	{
	public:
		virtual ~IFaceMeImageStreamHandler() {}

		/**
		* On Image Error is occurred.
		*
		*/
		virtual void OnImageFrameError(const FR_VideoFrameInfo *frameInfo, FR_RETURN result) = 0;
	};

	/**
	* CyberLink FaceMe Camera Manager
	* @interface IFaceMeCameraManager
	*/
	class IFaceMeCameraManager : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeCameraManager() {}

		/**
		* Object identifiers of Camera Manager.
		*
		* @enum FaceMeCameraManagerOIDs
		*/
		enum FaceMeCameraManagerOIDs
		{
			OID_FaceMeCameraManager = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of Camera Manager.
		};

		// General
		/**
		* Initialize FaceMe Camera Manager.
		*
		* @param[in] config The configuration for Camera Manager.
		* @param[in] licenseInfo The license information provided by CyberLink. The format of license information is a raw buffer.
		* @param[in] length The length of the license information.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_CONSTRAIN_HARDWARE</code>: License hardware is constrained.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		*/
		virtual FR_RETURN Initialize(const FR_CameraManagerConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

		/**
		* Finalize FaceMe Camera Manager and release corresponding resource.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: failed.
		* @note All of the created IFaceMeCameraCapturer and IFaceMeCameraEnumerator interface should be released before calling Finalize() API or release.
		*/
		virtual FR_RETURN Finalize() = 0;

		/**
		* Create Camera Capturer by device identifier.
		*
		* @param[in] deviceId device Identifier.
		* @param[in] config The configuration for capturer setup.
		* @param[out] capturer Retrieve a new instance of IFaceMeCameraCapturer. Caller should release this object when not used.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: The license number of camera exceeded the usage limitation.
		* @note All of the created IFaceMeCameraCapturer interface should be release before calling Finalize() API or Release.
		*/
		virtual FR_RETURN CreateCameraCapturer(int32_t deviceId, const FR_CameraCapturerConfig *config, IFaceMeCameraCapturer **capturer) = 0;

		/**
		* Create Camera Capturer by device path.
		*
		* @param[in] devicePath utf-8 encoded string of device path or url of IPCam.
		* @param[in] config The configuration for capturer setup.
		* @param[out] capturer Retrieve a new instance of IFaceMeCamCapturer. Caller should release this object when not used.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: The license number of camera exceeded the usage limitation.
		* @note All of the created IFaceMeCameraCapturer interface should be release before calling Finalize() API or release.
		*/
		virtual FR_RETURN CreateCameraCapturer(const char *devicePath, const FR_CameraCapturerConfig *config, IFaceMeCameraCapturer **capturer) = 0;

		/**
		* Create camera enumerator.
		*
		* @param[in] config The configuration forcamera enumerator setup.
		* @param[out] enumerator Retrieve a new instance of IFaceMeCameraEnumerator. Caller should release this object when not used.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		* @note All of the created IFaceMeCameraEnumerator interface should be release before calling Finalize() API or release.
		*/
		virtual FR_RETURN CreateCameraEnumerator(const FR_CameraEnumeratorConfig *config, IFaceMeCameraEnumerator **enumerator) = 0;


		/**
		* Set property.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[in] value The specified type of reference variant value by property identifier.
		* @param[in] args The specified type of reference variant arguments by property identifier.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The property identifier is not found.
		*/
		virtual FR_RETURN SetProperty(const char *propertyId, void *value, void *args) = 0;

		/**
		* Get property.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[out] value Retrieve the specified type of reference variant value by property identifier.
		* @param[out] args Retrieve the specified type of reference variant arguments by property identifier.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The property identifier is not found.
		*/
		virtual FR_RETURN GetProperty(const char *propertyId, void *value, void *args) = 0;
	};
}

#endif // __FR_IFACEMECAMERAMGR_H__