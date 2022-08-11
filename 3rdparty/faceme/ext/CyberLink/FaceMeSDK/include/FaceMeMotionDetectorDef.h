/**
* @file FaceMeMotionDetectorDef.h
* The definition for CyberLink FaceMe Motion Detector.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_MOTION_DETECTOR_DEF_H__
#define __FACEME_MOTION_DETECTOR_DEF_H__

#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
	/**
	* Contains Motion Detector Configuration.
	*
	* @struct FR_MotionDetectorConfig
	*/
	struct FR_MotionDetectorConfig
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		const char *appBundlePath;		//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;		//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;		//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.
	};

	/**
	* Contains Detect Configuration.
	*
	* @struct FR_MotionDetectConfig
	*/
	struct FR_MotionDetectConfig
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		bool discontinuity;				//!< The discontinuity of the input image frames.
		bool inverseScan;				//!< The scan direction of pixels.
		FR_Rectangle region;			//!< The region to be analyzed.
	};
}

#endif //__FACEME_MOTION_DETECTOR_DEF_H__
