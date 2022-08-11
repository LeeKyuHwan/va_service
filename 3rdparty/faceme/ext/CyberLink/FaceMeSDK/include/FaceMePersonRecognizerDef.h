/**
* @file FaceMePersonRecognizerDef.h
* The definition for CyberLink FaceMe Person Recognizer.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_PERSON_RECOGNIZER_DEF_H__
#define __FACEME_PERSON_RECOGNIZER_DEF_H__

#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
#define FR_PERSON_RECOGNIZER_PROPERTY_ID_GET_HARDWARE_ENABLED "HardwareEnabled"		//!< The type of value argument should be (bool *). The value of args argument should be nullptr.

	/**
	* config preference for FaceMe Person Recognizer.
	*
	* @enum EFR_PERSON_RECOGNIZER_PREFERENCE
	*/
	enum EFR_PERSON_RECOGNIZER_PREFERENCE
	{
		FR_PERSON_RECOGNIZER_PREFERENCE_NONE = 0,						       //!< None.
		FR_PERSON_RECOGNIZER_PREFERENCE_HARDWARE = (1 << 1),				   //!< Prefers  GPU hardware acceleration.
		FR_PERSON_RECOGNIZER_PREFERENCE_MOVIDIUS_VPU_DETECTION = (1 << 2),	   //!< Prefers Intel Movidius VPU acceleration.
	};

	/**
	* config preference for FaceMe Person Recognizer.
	*
	* @see EFR_PERSON_RECOGNIZER_PREFERENCE
	*/
	typedef int32_t FR_PERSON_RECOGNIZER_PREFERENCE;

	/**
	*model for person detection.
	*
	* @enum EFR_PERSON_DETECTION_MODEL
	*/
	enum EFR_PERSON_DETECTION_MODEL_SPEED_LEVEL
	{
		FR_PERSON_DETECTION_MODEL_SPEED_LEVEL_DEFAULT = 0,				//!< None.
		FR_PERSON_DETECTION_MODEL_SPEED_LEVEL_HIGH,   					//!< High.
		FR_PERSON_DETECTION_MODEL_SPEED_LEVEL_ULTRA_HIGH    			//!< Ultra-high.
	};

	/**
	* model for person detection.
	*
	* @see EFR_PERSON_DETECTION_MODEL
	*/
	typedef int32_t FR_PERSON_DETECTION_MODEL_SPEED_LEVEL;

	/**
	* Output order for detection.
	*
	* @enum EFR_PERSON_DETECT_OUTPUT_ORDER
	*/
	enum EFR_PERSON_DETECT_OUTPUT_ORDER
	{
		FR_PERSON_DETECT_OUTPUT_ORDER_NO_ORDER = 0,								//!< No order.
		FR_PERSON_DETECT_OUTPUT_ORDER_CONFIDENCE = 1,							//!< Confidence.
	};

	/**
	* Output order for detection.
	*
	* @see EFR_PERSON_DETECT_OUTPUT_ORDER
	*/
	typedef int32_t FR_PERSON_DETECT_OUTPUT_ORDER;

	/**
	*The type of person box.
	*
	* @enum EFR_PERSON_BOX_TYPE
	*/
	enum EFR_PERSON_BOX_TYPE
	{
		FR_PERSON_BOX_TYPE_FULL_BODY = 0,				//!< Detect Full body.
		FR_PERSON_BOX_TYPE_VISIBLE_BODY = 1,			//!< Detect Visible body.
		FR_PERSON_BOX_TYPE_HEAD = 2,					//!< Detect Head.
	};

	/**
	* Face feature options.
	*
	* @see EFR_FEATURE_OPTIONS
	*/
	typedef int32_t FR_PERSON_BOX_TYPE;

	/**
	* Contains Person Recognizer Configuration.
	*
	* @struct FR_PersonRecognizerConfig
	*/
	struct FR_PersonRecognizerConfig
	{
		uint32_t sizeOfStructure;									//!< The size of the structure.
		const char *appBundlePath;									//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;									//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;									//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.
		FR_PERSON_RECOGNIZER_PREFERENCE preference;					//!< @see FR_PERSON_RECOGNIZER_PREFERENCE.
		FR_PERSON_DETECTION_MODEL_SPEED_LEVEL detectionModel;					//!< @see FR_PERSON_DETECTION_MODEL .
		int32_t maxDetectionThreads;								//!< The maximum number of threads to be used for face detection.
		FR_PERSON_DETECT_OUTPUT_ORDER detectOutputOrder;			//!< @see FR_PERSON_DETECT_OUTPUT_ORDER .
		uint8_t gpuId;												//!< The GPU device identifier.
	};

	/**
	* Contains information of a person box.
	*
	* @struct FR_PersonInfo
	*/
	struct FR_PersonInfo
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		float confidence;					//!< The confidence of the person box.
		FR_PERSON_BOX_TYPE type;			//!< The options of extracted feature.
		FR_Rectangle boundingBox;			//!< The bounding box of the person.
	};

	/**
	* Contains result of person detection
	*
	* @struct FR_PersonDetectResult
	*/
	struct FR_PersonDetectResult
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.
		uint32_t imageWidth;						//!< The width of image frame.	
		uint32_t imageHeight;						//!< The height of image frame.	
		uint32_t personCount;						//!< The number of person in the image.	
		const FR_PersonInfo *personInfos;			//!< The array of person information in the image.
	};
}

#endif //__FACEME_RECOGNIZER_DEF_H__
