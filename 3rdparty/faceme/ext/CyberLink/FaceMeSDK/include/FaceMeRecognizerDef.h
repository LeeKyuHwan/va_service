/**
* @file FaceMeRecognizerDef.h
* The definition for CyberLink FaceMe Face Recognizer.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_RECOGNIZER_DEF_H__
#define __FACEME_RECOGNIZER_DEF_H__

#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
#define FR_RECOGNIZER_PROPERTY_ID_GET_HARDWARE_ENABLED "HardwareEnabled"		//!< The type of value argument should be (bool *). The value of args argument should be nullptr.

	/**
	* config preference.
	*
	* @enum EFR_PREFERENCE
	*/
	enum EFR_PREFERENCE
	{
		PREFER_NONE = 0,						           //!< None.
		PREFER_HARDWARE_DETECTION = (1 << 1),	           //!< Prefers GPU hardware acceleration on face detection.
		PREFER_FAST_DETECTION = (1 << 2),		           //!< (Deprecated) Prefers non-DNN algorithm for faster face detection speed. 
		PREFER_HARDWARE_EXTRACTION = (1 << 3),	           //!< Prefers GPU hardware acceleration on face extraction.
		PREFER_FAST_EXTRACTION = (1 << 4),		           //!< Reserved for future use.
		PREFER_INTEL_MOVIDIUS_VPU_DETECTION = (1 << 5),	   //!< Prefers Intel Movidius VPU acceleration on face detection.
		PREFER_INTEL_MOVIDIUS_VPU_EXTRACTION = (1 << 6),   //!< Prefers Intel Movidius VPU acceleration on face extraction.
		PREFER_INTEL_MOVIDIUS_VPU_HDDL = (1 << 7)		   //!< Prefers Intel Movidius VPU acceleration with HDDL plugin.
	};

	/**
	* config preference.
	*
	* @see FR_PREFERENCE
	*/
	typedef int32_t FR_PREFERENCE;

	/**
	* The config option of face extraction.
	*
	* @enum EFR_EXTRACTION_OPTION
	*/
	enum EFR_EXTRACTION_OPTION
	{
		FR_EXTRACTION_OPTION_MINFACEWIDTH = 0,			//!< Option of minimum face width.
		FR_EXTRACTION_OPTION_MAXFACEWIDTH,				//!< Option of maximum face width.
		FR_EXTRACTION_OPTION_DETECTION_SPEED_LEVEL,		//!< Option of speed level for face detection.
		FR_EXTRACTION_OPTION_DETECTION_OUTPUT_ORDER,	//!< Option of output order for face detection.
		FR_EXTRACTION_OPTION_DETECTION_MODE,			//!< Option of detection mode.
		FR_EXTRACTION_OPTION_TRACKING_MODE,				//!< Option of tracking mode.
		FR_EXTRACTION_OPTION_FAST_DETECTION_PERIOD		//!< Option of fast detection period in millisecond.
	};

	/**
	* The config option of face extraction.
	*
	* @see EFR_EXTRACTION_OPTION
	*/
	typedef int32_t FR_EXTRACTION_OPTION;

	/**
	* The mode of face recognizer.
	*
	* @enum EFR_RECOGNIZER_MODE
	*/
	enum EFR_RECOGNIZER_MODE
	{
		FR_RECOGNIZER_MODE_IMAGE = 0,	//!< face recognizer mode for Image.
		FR_RECOGNIZER_MODE_VIDEO = 1	//!< face recognizer mode for Video.
	};

	/**
	* The mode of face recognizer.
	*
	* @see EFR_RECOGNIZER_MODE
	*/
	typedef int32_t FR_RECOGNIZER_MODE;

	/**
	* The mode for face detection
	*
	* @enum EFR_DETECTION_MODE
	*/
	enum EFR_DETECTION_MODE
	{
		FR_DETECTION_MODE_NORMAL = 0,		//!< Normal detection
		FR_DETECTION_MODE_FAST				//!< Fast detection
	};

	/**
	* The mode for face detection.
	*
	* @see FR_DETECTION_MODE
	*/
	typedef int32_t FR_DETECTION_MODE;


	/**
	* The mode for face tracking
	*
	* @enum EFR_TRACKING_MODE
	*/
	enum EFR_TRACKING_MODE
	{
		FR_TRACKING_MODE_NORMAL = 0,		//!< Normal tracking
		FR_TRACKING_MODE_FAST,				//!< Fast tracking
		FR_TRACKING_MODE_MOTION				//!< Motion tracking
	};

	/**
	* The mode for face tracing.
	*
	* @see FR_TRACKING_MODE
	*/
	typedef int32_t FR_TRACKING_MODE;

	/**
	* Speed level for detection.
	*
	* @enum EFR_DETECTION_SPEED_LEVEL
	*/
	enum EFR_DETECTION_SPEED_LEVEL
	{
		FR_DETECTION_SPEED_LEVEL_DEFAULT = 0,	//!< Default.
		FR_DETECTION_SPEED_LEVEL_STANDARD,		//!< Standard.
		FR_DETECTION_SPEED_LEVEL_HIGH,			//!< High.
		FR_DETECTION_SPEED_LEVEL_ULTRA_HIGH		//!< Ultra-High.
	};

	/**
	* Speed level for detection.
	*
	* @see EFR_DETECTION_SPEED_LEVEL
	*/
	typedef int32_t FR_DETECTION_SPEED_LEVEL;

	/**
	* Output order for detection.
	*
	* @enum EFR_DETECTION_OUTPUT_ORDER
	*/
	enum EFR_DETECTION_OUTPUT_ORDER
	{
		FR_DETECTION_OUTPUT_ORDER_NO_ORDER = 0,		//!< No order.
		FR_DETECTION_OUTPUT_ORDER_CONFIDENCE,	    //!< Confidence. 
		FR_DETECTION_OUTPUT_ORDER_FACE_WIDTH		//!< Face width.
	};

	/**
	* Output order for detection.
	*
	* @see EFR_DETECTION_OUTPUT_ORDER
	*/
	typedef int32_t FR_DETECTION_OUTPUT_ORDER;

	/**
	* Speed level for detection model.
	*
	* @enum EFR_DETECTION_MODEL_SPEED_LEVEL
	*/
	enum EFR_DETECTION_MODEL_SPEED_LEVEL
	{
		FR_DETECTION_MODEL_SPEED_LEVEL_DEFAULT = 0,		//!< Default.
		FR_DETECTION_MODEL_SPEED_LEVEL_HIGH,			//!< High.
		FR_DETECTION_MODEL_SPEED_LEVEL_ULTRA_HIGH,		//!< Ultra-High.
	};

	/**
	* Speed level for detection model.
	*
	* @see EFR_DETECTION_MODEL_SPEED_LEVEL
	*/
	typedef int32_t FR_DETECTION_MODEL_SPEED_LEVEL;

	/**
	* Speed level for extraction model.
	*
	* @enum EFR_EXTRACTION_MODEL_SPEED_LEVEL
	*/
	enum EFR_EXTRACTION_MODEL_SPEED_LEVEL
	{
		FR_EXTRACTION_MODEL_SPEED_LEVEL_DEFAULT = 0,	//!< Default.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_STANDARD,		//!< Standard.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_HIGH,			//!< High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_ULTRA_HIGH,		//!< Ultra-High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_HIGH_ASIAN,		//!< High for Asian.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_VERY_HIGH,		//!< Very-High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_H3,		        //!< 3rd generation of High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_UH3, 	        //!< 3rd generation of Ultra-High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_UH3_M, 	        //!< 3rd generation of Ultra-High with mask.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_VH_M, 	        //!< Very-High with mask.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_H5,		        //!< 5th generation of High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_UH5, 	        //!< 5th generation of Ultra-High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_UH5_M, 	        //!< 5th generation of Ultra-High with mask.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_VH5, 	        //!< 5th generation of Very-High.
		FR_EXTRACTION_MODEL_SPEED_LEVEL_VH5_M 	        //!< 5th generation of Very-High with mask.
	};

	/**
	* Speed level for extraction model.
	*
	* @see EFR_EXTRACTION_MODEL_SPEED_LEVEL
	*/
	typedef int32_t FR_EXTRACTION_MODEL_SPEED_LEVEL;

	/**
	* Contains Recognizer Configuration.
	*
	* @struct FR_RecognizerConfig
	*/
	struct FR_RecognizerConfig
	{
		uint32_t sizeOfStructure;									//!< The size of the structure.
		const char *appBundlePath;									//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;									//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;									//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.
		FR_PREFERENCE preference;									//!< @see EFR_PREFERENCE .
		FR_DETECTION_MODEL_SPEED_LEVEL detectionModelSpeedLevel;	//!< @see FR_DETECTION_MODEL_SPEED_LEVEL .
		int32_t maxDetectionThreads;								//!< The maximum number of threads to be used for face detection.
		FR_EXTRACTION_MODEL_SPEED_LEVEL extractionModelSpeedLevel;	//!< @see EFR_EXTRACTION_MODEL_SPEED_LEVEL .
		int32_t maxExtractionThreads;								//!< The maximum number of threads to be used for face extraction.
		FR_RECOGNIZER_MODE mode;									//!< @see EFR_RECOGNIZER_MODE .
		uint8_t gpuId;												//!< The GPU device identifier.
		uint32_t maxFrameWidth;									//!< The maximum width of frame.
		uint32_t maxFrameHeight;								//!< The maximum height of frame.
		uint32_t minFaceWidth;										//!< The minimum pixel width of face.
		FR_DETECTION_OUTPUT_ORDER detectOutputOrder;				//!< @see EFR_DETECTION_OUTPUT_ORDER .
		uint32_t preferredDetectionBatchSize;						//!< The preferred detection batch size
		uint32_t preferredExtractionBatchSize;						//!< The preferred extraction batch size
	};

	/**
	* Contains Detection Configuration.
	*
	* @struct FR_DetectConfig
	*/
	struct FR_DetectConfig
	{
		uint32_t sizeOfStructure;				//!< The size of the structure.
		FR_FEATURE_OPTIONS detectOptions;		//!< The bitwise combination of the feature options. @see EFR_FEATURE_OPTIONS
	};

	/**
	* Contains Extraction Configuration.
	*
	* @struct FR_ExtractConfig
	*/
	struct FR_ExtractConfig
	{
		uint32_t sizeOfStructure;				//!< The size of the structure.
		FR_FEATURE_OPTIONS extractOptions;		//!< The bitwise combination of the feature options. @see EFR_FEATURE_OPTIONS
	};

}

#endif //__FACEME_RECOGNIZER_DEF_H__
