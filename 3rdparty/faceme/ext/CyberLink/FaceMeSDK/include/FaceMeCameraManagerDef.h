/**
* @file FaceMeCameraManagerDef.h
* The common definition for CyberLink Camera Manager.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FR_CAMERAMGR_DEF_H__
#define __FR_CAMERAMGR_DEF_H__
#include <stdint.h>
#include "FaceMeBaseDef.h"
#include "FaceMeLivenessDetectorDef.h"

namespace FaceMeSDK
{
	/**
	* The type of camera. 
	*
	* @enum EFR_CAMERA_TYPE
	*/
	enum EFR_CAMERA_TYPE
	{
		FR_CAMERA_TYPE_LOCAL_CAMERA = 0,	//!< Webcam.
		FR_CAMERA_TYPE_IP_CAMERA = 1,		//!< IP Camera.
		FR_CAMERA_TYPE_STILL_IMAGE = 2,		//!< Still Image.
	};

	/**
	* The type of camera.
	*
	* @see EFR_CAMERA_TYPE
	*/
	typedef int32_t FR_CAMERA_TYPE;

	/**
	* The orientation of capture output.
	*
	* @enum EFR_CAPTURE_ORIENTATION
	*/
	enum EFR_CAPTURE_ORIENTATION
	{
		FR_CAPTURE_ORIENTATIONE_NONE = 0,			//!< None.
		FR_CAPTURE_ORIENTATION_FLIP_HORIZONTAL = 2	//!< Flip Horizontal
	};

	/**
	* The options for camera manager.
	*/
	typedef int32_t FR_CAMERA_MGR_OPTIONS;

	/**
	* The orientation of capture output.
	*
	* @see EFR_CAPTURE_ORIENTATION
	*/
	typedef int32_t FR_CAPTURE_ORIENTATION;



	/**
	* Camera Capture options.
	*
	* @enum EFR_CAPTURE_OPTIONS
	*/
	enum EFR_CAPTURE_OPTIONS
	{
		FR_CAPTURE_OPTION_NONE = 0,							//!< None.
		FR_CAPTURE_OPTION_FRAME_NO_DROP = (1LL << 2)		//!< Preferred not drop frame.
	};

	/**
	* The preferred options for capture.
	*/
	typedef int32_t FR_CAPTURE_OPTIONS;


	/**
	* Contains config for initialize Camera Manager.
	*
	* @struct FR_CameraManagerConfig
	*/
	struct FR_CameraManagerConfig
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.

		const char *appBundlePath;					//!< The application bundle folder path, represented by utf-8 encoded string.
		const char *appDataPath;					//!< The application folder path to be used for data saving, represented by utf-8 encoded string.
		const char *appCachePath;					//!< The application folder path to be used for cache saving, represented by utf-8 encoded string.
		FR_CAMERA_MGR_OPTIONS options;				//!< Options for camera manager. Reserved for future use.
	};

	/**
	* The related information of an video frame.
	*
	* @struct FR_VideoFrameInfo
	*/
	struct FR_VideoFrameInfo
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		int64_t frameIndex;				//!< The index of current video frame.
		int64_t timePosition;			//!< The position of time of current video frame.
		int64_t timeDuration;			//!< The duration of time of current video frame.
	};

	/**
	* Contains configuration of a Camera Capturer.
	*
	* @struct FR_CameraCapturerConfig
	*/
	struct FR_CameraCapturerConfig
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		FR_CAMERA_TYPE cameraType;			//!< The type of camera.
	};

	/**
	* Contains capture configuration of a Camera Capturer.
	*
	* @struct FR_CaptureConfig
	*/
	struct FR_CaptureConfig
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		uint32_t preferredWidth;			//!< Preferred width. If the value is zero, then default width is used.
		uint32_t preferredHeight;			//!< Preferred height. If the value is zero, then default height is used.
		float preferredFrameRate;			//!< Preferred frame rate. If the value is zero, then default frame rate is used.
		FR_CAPTURE_ORIENTATION orientation;	//!< Orientation. @see EFR_CAPTURE_ORIENTATION.
		FR_CAPTURE_OPTIONS preferredOptions;//!< Preferred options.
		uint32_t preferredGpuId;			//!< Preferred GPU device identifier. If the value is zero, then default GPU device is used.
	};

	/**
	* Contains configuration of a Camera Enumerator.
	*
	* @struct FR_CameraEnumeratorConfig
	*/
	struct FR_CameraEnumeratorConfig
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		FR_CAMERA_TYPE cameraType;			//!< The type of camera.
	};

#define FR_CAMERA_INFO_PROP_UINT32_DEVICE_ID "FR::CameraInfo::DeviceID"  //!< The identifier of device, represented by 32-bit integer value.
#define FR_CAMERA_INFO_PROP_STRING_DEVICE_PATH "FR::CameraInfo::DevicePath"	//!< The path of device, represented by utf-8 encoded string.
#define FR_CAMERA_INFO_PROP_STRING_DEVICE_NAME "FR::CameraInfo::DeviceName"	//!< The display name of device, represented by utf-8 encoded string.
#define FR_CAMERA_INFO_PROP_STRING_DEVICE_3D_MODEL_ID "FR::CameraInfo::Device3DModelID" //!< The identifier of model for 3D device only, represented by utf-8 encoded string.

#define FR_CAMERA_SETTING_PROP_AUTO_FOCUS "FR::CameraSetting::AutoFocus"  //!< Setting of auto focus feature, represented by boolean value. This setting is valid only when the camera is supported. value: (bool *)&isAutofocus, args: nullptr.
#define FR_CAMERA_SETTING_PROP_FOCUS "FR::CameraSetting::Focus"  //!< Setting of focus value, represented by 32-bit integer value. This setting is valid only when the camera is supported. value: (int32_t *)&focusValue, args: nullptr.
#define FR_CAMERA_SETTING_PROP_AUTO_EXPOSURE "FR::CameraSetting::AutoExposure"  //!< Setting of auto exposure feature, represented by boolean value. This setting is valid only when the camera is supported.value: (bool *)&isAutoExposure, args: nullptr.
#define FR_CAMERA_SETTING_PROP_EXPOSURE "FR::CameraSetting::Exposure"  //!< Setting of exposure value, represented by 32-bit integer value. This setting is valid only when the camera is supported. value: (int32_t *)&exposureValue, args: nullptr.
#define FR_CAMERA_SETTING_PROP_AUTO_FOCUS "FR::CameraSetting::AutoFocus"  //!< Setting of auto focus feature, represented by boolean value. This setting is valid only when the camera is supported. value: (bool *)&isAutofocus, args: nullptr.

#define FR_CAMERA_SETTING_PROP_FRAME_WIDTH "FR::CameraSetting::FrameWidth" //!< Read only setting of frame width, represented by 32-bit unsigned integer value. This setting is valid only when the camera is supported and started. value: (uint32_t *)&frameWidth, args: nullptr.
#define FR_CAMERA_SETTING_PROP_FRAME_HEIGHT "FR::CameraSetting::FrameHeight" //!< Read only setting of frame height, represented by 32-bit unsigned integer value. This setting is valid only when the camera is supported and started. value: (uint32_t *)&frameHeight, args: nullptr.
#define FR_CAMERA_SETTING_PROP_FRAME_RATE "FR::CameraSetting::FrameRate" //!< Read only setting of frame rate, represented by float value. This setting is valid only when the camera is supported and started. value: (float *)&frameRate, args: nullptr.

#define FR_CAMERA_SETTING_PROP_FM_IMAGE_STREAM_HANDLER "FR::CameraSetting::FaceMeImageStreamHandler"  //!< Setting of CyberLink Image Stream Handler, represented by pointer of IFaceMeImageStreameHandler interface. This setting is valid only when the camera is supported. value: (IFaceMeImageStreamHandler *), args: nullptr.

}

#endif //__FR_CAMERAMGR_DEF_H__
