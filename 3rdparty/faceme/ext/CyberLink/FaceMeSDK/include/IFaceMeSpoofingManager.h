/**
* @file IFaceMeSpoofingManager.h
* The interface of CyberLink Spoofing Manager.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_ISPOOFINGMANAGER_H__
#define __FACEME_ISPOOFINGMANAGER_H__
#include "IBaseObj.h"
#include "FaceMeSpoofingManagerDef.h"
#include <windows.h>

namespace FaceMeSDK 
{
	/**
	* CyberLink Spoofing Detect Result Reciever
	* @interface IFaceMeGetSpoofingResult
	*/
	class IFaceMeGetSpoofingResult
	{
	public:
		virtual ~IFaceMeGetSpoofingResult() {}

		/**
		* Callback function for get Spoofing Result.
		*
		* @param[out] result The result of Spoofing Detect.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN GetSpoofingResult(FMPAntiSpoofingResult &result) = 0;
	};

	class IFaceMeSpoofingManager : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeSpoofingManager() {}

		enum FaceMeSpoofingManagerOIDs
		{
			OID_FaceMeSpoofingManager = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of Capture Device Info.
		};

		// General
		/**
		* Initialize ISpoofingManager engine.
		*
		* @param[in] config The configuration for video devices engine.
		* @param[in] licenseInfo The license information provided by CyberLink. The format of license information is a raw buffer.
		* @param[in] length The length of the license information.
		* @param[in] pGetSpoofingResult The GetSpoofingResult function pointer for callback.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: The license feature is not supported.
		*  * <code>FR_RETURN_LICENSE_CONSTRAIN_HARDWARE</code>: License hardware is constrained.
		*/
		virtual FR_RETURN Initialize(const FR_SpoofingManagerConfig *config, const uint8_t* licenseInfo, uint32_t length, IFaceMeGetSpoofingResult* pGetSpoofingResult) = 0;

		/**
		* Initialize Webcam infomations.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_WEBCAM_INIT_FAIL</code>: Webcam initialize failed.
		*/
		virtual FR_RETURN InitWebcamInfos() = 0;

		/**
		* Set Camera for using in Spoofing Manager.
		*
		* @param[in] deviceName The deviceName for Camera.
		* @param[in] preferenceWidth The width preference of webcam.
		* @param[in] preferenceHeight The Height preference of webcam.
		* @param[in] bMirror The boolean value that whether the webcam frame has been mirrored.
		* @param[in] preferedFrameRate The frame rate preference of webcam.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_NOT_INIT</code>: Failed.
		*  * <code>FR_RETURN_WEBCAM_INIT_FAIL</code>: Webcam initialize failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_NOT_SUPPORT</code>: Can't find available camera.
		*  * <code>FR_RETURN_OUT_OF_MEMORY</code>: Camera pointer initialize failed.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: License activation is exceeded.
		*/
		virtual FR_RETURN SetWebcam(const char *deviceName, uint32_t preferenceWidth, uint32_t preferenceHeight, bool bMirror, float preferedFrameRate) = 0;
		
		/**
		* Run Spoofing Manager.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The file path argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of data structure of file path is not accepted or not filled in.
		*/
		virtual FR_RETURN Run() = 0;

		/**
		* Stop Spoofing Manager.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN Stop() = 0;

		/**
		* Pause Spoofing Manager.
		*
		* @param[in] timeoutMs The timeout for pausing spoofing manager.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN Pause(int32_t timeoutMs) = 0;

		/**
		* Reset Spoofing Manager.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The reset argument is invalid.
		*/
		virtual FR_RETURN Reset() = 0;

		/**
		* Finalize Spoofing Manager.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN Finalize() = 0;

		/**
		* Get video window handler
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN GetVideoWindow(HWND &hwnd) = 0;

		/**
		* Set video window size
		*
		* @param[in] width The width of video window.
		* @param[in] height The height of video window.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN SetVideoWindowSize(int64_t width, int64_t height) = 0;

		/**
		* Set whether Interaction Check is enable
		*
		* @param[in] isEnableInteractionCheck The boolean value whether Interaction Check is enable.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN SetEnableInteractionCheck(bool isEnableInteractionCheck) = 0;

		/**
		* Get all enabled camera number
		*
		* @param[in] cameraNumber The number of all available cameras.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN GetEnabledCameraNumber(uint32_t &cameraNumber) = 0;

		/**
		* Get all enabled camera names
		*
		* @param[in] cameraName The camera name of the camera that it's index of all available cameras is cameraIndex.
		* @param[in] cameraNameLength The length of the camera name of the camera that it's index of all available cameras is cameraIndex.
		* @param[in] cameraIndex The camera index of all available cameras.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN GetEnabledCameraName(char *cameraName, uint32_t &cameraNameLength, uint32_t cameraIndex) = 0;

		/**
		* Get current camera name
		*
		* @param[in] cameraName The camera name of current camera.
		* @param[in] cameraNameLength The length of the camera name of current camera.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN GetCurrentCameraName(char *cameraName, uint32_t &cameraNameLength) = 0;

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

#endif // _FACEME_ISPOOFINGMANAGER_H__