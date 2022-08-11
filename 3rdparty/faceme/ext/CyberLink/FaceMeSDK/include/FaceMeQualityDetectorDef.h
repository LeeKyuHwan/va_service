/**
 * @file FaceMeQualityDetectorDef.h
 * The definition for CyberLink FaceMe Quality Detector.
 *
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_QUALITY_DETECTOR_DEF_H__
#define __FACEME_QUALITY_DETECTOR_DEF_H__

#include <stdint.h>
#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
#define FR_QUALITY_DETECTOR_PROPERTY_ID_GET_BLUR_THRESHOLD	"FaceQualityDetector::GetBlurThreshold"	//!< value: (float *)blur threshold, args: (FR_BLUR_QUALITY_MODE *)

	/**
	* Photo quality issue options.
	*
	* @enum EFR_QUALITY_ISSUE_OPTIONS
	*/
    enum EFR_QUALITY_ISSUE_OPTIONS
	{
		FR_QUALITY_ISSUE_OPTION_NONE = 0,                  //!< None.
		FR_QUALITY_ISSUE_OPTION_UNDERSIZED = 1,            //!< Check if the face is too small.
		FR_QUALITY_ISSUE_OPTION_OCCLUDED = (1LL << 1),     //!< Check if the face is occluded.
		FR_QUALITY_ISSUE_OPTION_BLUR = (1LL << 2),         //!< Check if the photo is blurred.
		FR_QUALITY_ISSUE_OPTION_EXPOSURE = (1LL << 3),     //!< Check lighting.
		FR_QUALITY_ISSUE_OPTION_EYE_CLOSED = (1LL << 4),   //!< Check if eyes are closed.
		FR_QUALITY_ISSUE_OPTION_WRONG_ANGLE = (1LL << 5),  //!< Check face angle.
		FR_QUALITY_ISSUE_OPTION_GRAYSCALE = (1LL << 6),    //!< Check if the image is grayscale.
		FR_QUALITY_ISSUE_OPTION_ALL = INT32_MAX            //!< Check all.
	};

	/**
	* The detect type of photo quality check.
	*
	* @see EFR_QUALITY_ISSUE_OPTIONS
	*/
	typedef int32_t FR_QUALITY_ISSUE_OPTIONS;

	/**
	* The mode of blur detection for different sources.
	*
	* @enum EFR_BLUR_QUALITY_MODE
	*/
	enum EFR_BLUR_QUALITY_MODE
	{
		FR_BLUR_CAMERA_MODE = 0,    //!< Image.
		FR_BLUR_WEBCAM_MODE         //!< Video.
	};

	/**
	* The mode of blur detection for different sources.
	*
	* @see EFR_BLUR_QUALITY_MODE
	*/
	typedef int32_t FR_BLUR_QUALITY_MODE;

	/**
	* The mode of photo quality check.
	*
	* @enum EFR_QUALITY_CHECK_MODE
	*/
	enum EFR_QUALITY_CHECK_MODE
	{
		FR_QUALITY_CHECK_MODE_ONE_FAILURE,  //!< Return when one detect type fails.
		FR_QUALITY_CHECK_MODE_ALL_FAILURE   //!< Return when all detect types are checked.
	};

	/**
	* The mode of photo quality check.
	*
	* @see EFR_QUALITY_CHECK_MODE
	*/
	typedef int32_t FR_QUALITY_CHECK_MODE;

	/**
	* The fail case of facial occlusion.
	*
	* @enum EFR_OCCLUSION_FAIL_REASON
	*/
	enum EFR_OCCLUSION_FAIL_REASON
	{
		FR_OCCLUSION_NONE = 0,              //!< None.
		FR_OCCLUSION_UNKNOWN = (1LL << 1),  //!< The face is not suitable for occlusion detection.
		FR_OCCLUSION_LEFTEYE = (1LL << 2),     //!< The face is occluded by left eye.
		FR_OCCLUSION_RIGHTEYE = (1LL << 3),     //!< The face is occluded by right eye.
		FR_OCCLUSION_NOSE = (1LL << 4),     //!< The face is occluded by nose.
		FR_OCCLUSION_MOUTH = (1LL << 5),     //!< The face is occluded by mouth.
	};

	/**
	* The fail case of facial occlusion.
	*
	* @see EFR_OCCLUSION_FAIL_REASON
	*/
	typedef int32_t FR_OCCLUSION_FAIL_REASON;

	/**
	* config preference.
	*
	* @enum EFR_QC_PREFERENCE
	*/
	enum EFR_QC_PREFERENCE
	{
		FR_QC_PREFER_NONE = 0,						            //!< None.
		FR_QC_PREFER_HARDWARE_DETECTION = (1 << 1),	            //!< Prefers GPU hardware acceleration on quality detection.
		FR_QC_PREFER_FAST_DETECTION = (1 << 2),		            //!< (Deprecated) Prefers non-DNN algorithm for faster quality detection speed.
		FR_QC_PREFER_INTEL_MOVIDIUS_VPU_DETECTION = (1 << 5),   //!< Prefers Intel Movidius VPU acceleration on quality detection.
	};

	/**
	* config preference.
	*
	* @see EFR_QC_PREFERENCE
	*/
	typedef int32_t FR_QC_PREFERENCE;

	/**
	* Contains Quality Detector Configuration.
	*
	* @struct FR_QualityDetectorConfig
	*/
	struct FR_QualityDetectorConfig
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		const char *appBundlePath;		//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;		//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;		//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.
		FR_QC_PREFERENCE preference;	//!< @see EFR_QC_PREFERENCE.
	};

	/**
	* Contains Detect Configuration.
	*
	* @struct FR_QualityDetectConfig
	*/
	struct FR_QualityDetectConfig
	{
		uint32_t sizeOfStructure;				//!< The size of the structure.
		FR_QUALITY_ISSUE_OPTIONS detectType;    //!< The detect type.
		FR_QUALITY_CHECK_MODE checkMode;        //!< The mode of photo quality check.
		FR_BLUR_QUALITY_MODE blurDetectMode;	//!< Different detect config for blur detection
		int32_t overExposureThreshold;          //!< The threshold of over exposure.
		int32_t underExposureThreshold;         //!< The threshold of under exposure.

		uint32_t faceCount;                     //!< Count of faces. When faceCount > 0, faceInfos, faceLandmarks and poses cannot be nullptr.
		FR_FaceInfo* faceInfos;                 //!< The face information for analyzed.
		FR_FaceLandmark* faceLandmarks;         //!< The feature landmarks for analzed.
		FR_Pose* poses;                         //!< The face poses for analyzed.
	};

	/**
	* Contains results of photo quality check.
	*
	* @struct FR_QualityDetectResult
	*/
	struct FR_QualityDetectResult
	{
		uint32_t sizeOfStructure;				        //!< The size of the structure.
		FR_QUALITY_ISSUE_OPTIONS issue;					//!< The detect result.
		float blur;                                     //!< The value of blurriness.
		float exposure;                                 //!< The value of exposure.
		uint32_t faceSize;                              //!< The face size. (pixel)
		FR_Pose wrongAngle;                             //!< The wrong face angle.
		FR_OCCLUSION_FAIL_REASON occlusionReason;       //!< @see EFR_OCCLUSION_FAIL_REASON.
	};
}

#endif // __FACEME_QUALITY_DETECTOR_DEF_H__
