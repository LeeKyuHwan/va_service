/**
 * @file FaceMeLivenessDetectorDef.h
 * The definition for CyberLink FaceMe Liveness detection.
 *
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_LIVENESS_DETECTOR_DEF_H__
#define __FACEME_LIVENESS_DETECTOR_DEF_H__

#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
	/**
	* The config option of face liveness detection.
	*
	* @enum EFR_LIVENESS_DETECTION_OPTION
	*/
	enum EFR_LIVENESS_DETECTION_OPTION
	{
		FR_LIVENESS_DETECTION_OPTION_TRACKING_MODE = 0,	//!< Option of liveness tracking mode.
		FR_LIVENESS_DETECTION_OPTION_SINGLE_FACE_MOTION_TRACKING_SPEED_LEVEL,		//!< Option of liveness single face motion tracking speed level.
	};

	/**
	* The config option of face liveness detection.
	*
	* @see EFR_LIVENESS_DETECTION_OPTION
	*/
	typedef int32_t FR_LIVENESS_DETECTION_OPTION;

	/**
	* The mode for face liveness tracing.
	*
	* @enum EFR_LIVENESS_TRACKING_MODE
	*/
	enum EFR_LIVENESS_TRACKING_MODE
	{
		FR_LIVENESS_TRACKING_MODE_NORMAL = 0,			//!< Normal tracking
		FR_LIVENESS_TRACKING_MODE_MOTION,				//!< Motion tracking
        FR_LIVENESS_TRACKING_MODE_SINGLE_FACE_MOTION,	//!< Single Face Motion tracking
	};

	/**
	* The mode for face liveness tracing.
	*
	* @see FR_LIVENESS_TRACKING_MODE
	*/
	typedef int32_t FR_LIVENESS_TRACKING_MODE;

	/**
	* The mode for Liveness single face motion tracking speed level.
	*
	* @enum EFR_LIVENESS_SINGLE_FACE_MOTION_TRACKING_SPEED_LEVEL
	*/
	enum EFR_LIVENESS_SINGLE_FACE_MOTION_TRACKING_SPEED_LEVEL
	{
		FR_LIVENESS_SINGLE_FACE_MOTION_TRACKING_FAST = 0,	//!< Fast speed level
		FR_LIVENESS_SINGLE_FACE_MOTION_TRACKING_PRECISE,	//!< Precise speed level
	};

	/**
	* The mode for Liveness single face motion tracking speed level.
	*
	* @see FR_LIVENESS_SINGLE_FACE_MOTION_TRACKING_SPEED_LEVEL
	*/
	typedef int32_t FR_LIVENESS_SINGLE_FACE_MOTION_TRACKING_SPEED_LEVEL;

	/**
	* The mode for face tracking.
	*
	* @enum EFR_LIVENESS_DETECTION_MODE
	*/
	enum EFR_LIVENESS_DETECTION_MODE
	{
		FR_LIVENESS_DETECTION_MODE_DEPTH = 0,			//!< Liveness detection by depth information
		FR_LIVENESS_DETECTION_MODE_RGB_WITHBG		//!< Obsolete, do not use it
	};

	/**
	* The mode for face tracking.
	*
	* @see EFR_LIVENESS_DETECTION_MODE
	*/
	typedef int32_t FR_LIVENESS_DETECTION_MODE;

	/**
	* The type of liveness status.
	*
	* @enum EFR_FACE_LIVENESS_STATUS
	*/
	enum EFR_FACE_LIVENESS_STATUS
	{
		FR_FACE_IS_LIVE_UNKNOWN = 0,	//!< Unknown.
		FR_FACE_IS_LIVENESS = 1,		//!< Liveness.
		FR_FACE_IS_SPOOFING = 2,		//!< Spoofing.
		FR_FACE_IS_INVALID_ANGLE = 3,	//!< Invalid Angle.
		FR_FACE_IS_TOO_FAR = 4,			//!< Too Far.
		FR_FACE_IS_TOO_NEAR = 5			//!< Too Near.
	};

	/**
	* The mode for face anti-spoofing.
	*
	* @see EFR_FACE_LIVENESS_STATUS
	*/
	typedef int32_t FR_FACE_LIVENESS_STATUS;

	/**
	* Contains config of init face anti-spoofing.
	*
	* @struct FR_LivenessDetectorConfig
	*/
	struct FR_LivenessDetectorConfig
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.

		const char *appBundlePath;					//!< The application bundle folder path, represented by utf-8 encoded string.
		const char *appDataPath;					//!< The application folder path to be used for data saving, represented by utf-8 encoded string.
		const char *appCachePath;					//!< The application folder path to be used for cache saving, represented by utf-8 encoded string.

		FR_LIVENESS_DETECTION_MODE detectionMode;	//!< Face liveness detection mode
		const FR_Image* backgroundImage;			//!< The reference background.
		const char *depthCamId;						//!< The identifier of depth camera, represented by utf-8 encoded string.
		const FR_FaceFeatureScheme* featureScheme;	//!< The scheme of feature.
	};


	/**
	* Depth map format.
	*
	* @enum EFR_DEPTH_MAP_FORMAT
	*/
	enum EFR_DEPTH_MAP_FORMAT
	{
		FR_DEPTH_MAP_FORMAT_UNKNOWN = 0,//!< Unknown.
		FR_DEPTH_MAP_FORMAT_16UC1,		//!< 16-bit unsigned format.
	};

	/**
	* Depth map format.
	*
	* @see EFR_DEPTH_MAP_FORMAT
	*/
	typedef int32_t FR_DEPTH_MAP_FORMAT;

	/**
	* The depth map and the related information of an depth map.
	*
	* @struct FR_DepthMap
	*/
	struct FR_DepthMap
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		void *data;							//!< The depth map raw data.
		uint32_t width;						//!< The width of the depth map.
		uint32_t height;					//!< The height of the depth map.
		uint32_t stride;					//!< The line stride of the depth map.
		uint32_t channel;					//!< The channels of a pixel.
		uint32_t bytesPerPixel;				//!< The bytes of a pixel.
		FR_DEPTH_MAP_FORMAT depthMapFormat; //!< The depth map format.
	};

	/**
	* Contains config of liveness detection.
	*
	* @struct FR_LivenessDetectConfig
	*/
	struct FR_LivenessDetectConfig
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.

		float confidenceThreshold;			//!< The threshold of the confidence.
		const FR_FaceFeature* features;		//!< The feature vector of the face to be analyzed.
		const FR_FaceLandmark* landmarks;	//!< The feature points of the face to be analyzed.
		const FR_Pose* poses;				//!< The face pose for analyzed.
		bool discontinuity;					//!< The discontinuity of the input image frames.
	};

	/**
	* Contains information of face liveness.
	*
	* @struct FR_FaceLivenessResult
	*/
	struct FR_FaceLivenessResult
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		FR_FACE_LIVENESS_STATUS status;		//!< Face liveness status.
		float probability;					//!< The probability of the face to be live.
	};
}

#endif //__FACEME_LIVENESS_DETECTOR_DEF_H__
