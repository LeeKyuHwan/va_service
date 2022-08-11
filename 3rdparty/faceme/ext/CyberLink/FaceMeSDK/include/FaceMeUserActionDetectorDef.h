/**
 * @file FaceMeUserActionDetectorDef.h
 * The definition for CyberLink FaceMe User Action Detector.
 *
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_USER_ACTION_DETECTOR_DEF_H__
#define __FACEME_USER_ACTION_DETECTOR_DEF_H__

#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
	/**
	* The mode for user action detection.
	*
	* @enum EFR_USER_ACTION
	*/
	enum EFR_USER_ACTION
	{
		UA_DETECT_NONE = 0,			//!< None.
		UA_DETECT_BLINK_EYES,		//!< Blink eye detection.
		UA_DETECT_TURN_HEAD,		//!< Turn head detection.
		UA_DETECT_SMILE,			//!< Smile detection.
		UA_DETECT_OPEN_MOUTH,		//!< Open mouth detection.
		UA_DETECT_NOD_HEAD,			//!< Nod head detection.
		UA_DETECT_LIFT_EYE_BROWS,	//!< Lift eye brows detection.
		UA_DETECT_CENTER,			//!< Detect if the face is in the center of the image.
	};

	/**
	* The mode for user action detection.
	*
	* @see EFR_USER_ACTION
	*/
	typedef int32_t FR_USER_ACTION;

#define FR_USER_ACTION_PROPERTY_ID_GET_HARDWARE_ENABLED "HardwareEnabled"		//!< The type of value argument should be (bool *). The value of args argument should be nullptr.
#define FR_USER_ACTION_PROPERTY_ID_GET_SMILE_COUNT "SmileCount"					//!< The type of value argument should be (int *). The value of args argument should be nullptr.

	/**
	* config preference.
	*
	* @enum EFR_UA_PREFERENCE
	*/
	enum EFR_UA_PREFERENCE
	{
		FR_UA_PREFER_NONE = 0,						//!< None.
		FR_UA_PREFER_HARDWARE_DETECTION = (1 << 1),	//!< Prefers GPU hardware acceleration on face detection.
		FR_UA_PREFER_FAST_DETECTION = (1 << 2),		//!< (Deprecated) Prefers non-DNN algorithm for faster face detection speed.
		FR_UA_PREFER_INTEL_MOVIDIUS_VPU_DETECTION = (1 << 5),   //!< Prefers Intel Movidius VPU acceleration on detection.
	};

	/**
	* config preference.
	*
	* @see EFR_UA_PREFERENCE
	*/
	typedef int32_t FR_UA_PREFERENCE;

	/**
	* Contains User Action Detector Configuration.
	*
	* @struct FR_UserActionDetectorConfig
	*/
	struct FR_UserActionDetectorConfig
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.
		const char *appBundlePath;					//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;					//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;					//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.
		FR_UA_PREFERENCE preference;				//!< @see EFR_UA_PREFERENCE.
		const FR_FaceFeatureScheme* featureScheme;	//!< The scheme of feature.
	};

	/**
	* Contains Detect Configuration.
	*
	* @struct FR_UserActionDetectConfig
	*/
	struct FR_UserActionDetectConfig
	{
		uint32_t sizeOfStructure;				//!< The size of the structure.
		float confidenceThreshold;				//!< The threshold of the confidence.
		FR_Rectangle region;					//!< The region to be analyzed.
		const FR_FaceInfo *faceInfo;			//!< The face information for analyzed.
		const FR_FaceAttribute *faceAttribute;	//!< The face attribute for analyzed.
		const FR_FaceFeature *faceFeature;		//!< The feature vector of the face to be analyzed.
		const FR_Image *image;					//!< The image to detect action.
	};
}

#endif // __FACEME_USER_ACTION_DETECTOR_DEF_H__
