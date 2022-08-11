/**
* @file FaceMeSpoofingManagerDef.h
* The common definition for CyberLink Spoofing Manager.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FR_SPOOFINGMANAGER_DEF_H__
#define __FR_SPOOFINGMANAGER_DEF_H__
#include <stdint.h>
#include <FaceMeBaseDef.h>
#include <string>
#include <vector>

namespace FaceMeSDK
{
#define FR_SPOOFING_MANAGER_ID_SET_PRECISION_MODE	"SpoofingManager::SetPrecisionMode"		//!< value: (int32_t *)&securityLevel, args: nullptr
#define FR_SPOOFING_MANAGER_ID_SET_SECOND_STAGE_ALWAYS_ON	"SpoofingManager::SetSecondStageAlwaysOn"		//!< value: (bool *)&isSetSecondStageAlwaysOn, args: nullptr

	/**
	* FaceMe instance type
	*
	* @enum EFR_INSTANCE_TYPE_SPOOFING_MANAGER
	*/
	enum EFR_INSTANCE_TYPE_SPOOFING_MANAGER
	{
		FR_INSTANCE_SPOOFING_MANAGER = 13		//!< Instance type of Spoofing Manager. 
	};

	/**
	* status return code.
	*
	* @enum EFR_RETURN_SPOOFING_MANAGER
	*/
	enum EFR_RETURN_SPOOFING_MANAGER
	{
		FR_RETURN_WEBCAM_INIT_FAIL = -61,		//!< Webcam initialize failed.
	};

	/**
	* Spoofing result.
	*
	* @enum EFR_SPOOFING_RESULT
	*/
	enum EFR_SPOOFING_RESULT
	{
		FR_SPOOFING_RESULT_UNKNOWN = 0,         //!< Spoofing result is unknown, indeterminate.
		FR_SPOOFING_RESULT_SPOOFING = 1,        //!< Spoofing result is spoofing.
		FR_SPOOFING_RESULT_LIVENESS = 2,        //!< Spoofing result is liveness.
	};

	/**
	* Spoofing result.
	*
	* @enum FR_SPOOFING_RESULT
	*/
	typedef int32_t FR_SPOOFING_RESULT;

	/**
	* Precision mode.
	*
	* @enum EFR_SPOOFING_MANAGER_PRECISION_MODE
	*/
	enum EFR_SPOOFING_MANAGER_PRECISION_MODE
	{
		FR_SPOOFING_MANAGER_PRECISION_MODE_HIGH_PRECISION = 0,		//!< Precision mode is high precision.
		FR_SPOOFING_MANAGER_PRECISION_MODE_STANDARD = 1,			//!< Precision mode is standard.
		FR_SPOOFING_MANAGER_PRECISION_MODE_FAST = 2,				//!< Precision mode is fast.
	};

	/**
	* Precision mode.
	*
	* @enum FR_SPOOFING_MANAGER_PRECISION_MODE
	*/
	typedef int32_t FR_SPOOFING_MANAGER_PRECISION_MODE;

	/**
	* Second stage mode.
	*
	* @enum EFR_SPOOFING_MANAGER_SECOND_STAGE_MODE
	*/
	enum EFR_SPOOFING_MANAGER_SECOND_STAGE_MODE
	{
		FR_SPOOFING_MANAGER_SECOND_STAGE_MODE_AUTO = 0,				//!< Second stage mode is auto.
		FR_SPOOFING_MANAGER_SECOND_STAGE_MODE_OFF = 1,				//!< Second stage mode is off.
		FR_SPOOFING_MANAGER_SECOND_STAGE_MODE_ALWAYS_ON = 2,		//!< Second stage mode is always on.
	};

	/**
	* Second stage mode.
	*
	* @enum FR_SPOOFING_MANAGER_SECOND_STAGE_MODE
	*/
	typedef int32_t FR_SPOOFING_MANAGER_SECOND_STAGE_MODE;

	/**
	* Anti-spoofing result return data.
	*
	* @struct FMPAntiSpoofingResult
	*/
	struct FMPAntiSpoofingResult
	{
		FR_Image frImage;
		FaceMeSDK::FR_SPOOFING_RESULT spoofingResult;
		double confidence;
	};

	/**
	* Contains config of init face anti-spoofing.
	*
	* @struct FR_LivenessDetectorConfig
	*/
	struct FR_SpoofingManagerConfig
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.

		const char *appBundlePath;					//!< The application bundle folder path, represented by utf-8 encoded string.
		const char *appDataPath;					//!< The application folder path to be used for data saving, represented by utf-8 encoded string.
		const char *appCachePath;					//!< The application folder path to be used for cache saving, represented by utf-8 encoded string.
	};
}

#endif //	__FR_SPOOFINGMANAGER_DEF_H__
