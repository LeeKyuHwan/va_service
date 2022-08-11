/**
* @file FaceMeLicenseManagerDef.h
* The definition for CyberLink FaceMe License Manager.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_LICENSE_MANAGER_DEF_H__
#define __FACEME_LICENSE_MANAGER_DEF_H__

#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_DETECTION								"FaceRecognizer::AllowDetection" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_DETECTION_ULTRA_PRECISION				"FaceRecognizer::AllowDetection::UltraPrecision" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_DETECTION_HIGH_PRECISION				"FaceRecognizer::AllowDetection::HighPrecision"  //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_DETECTION_HIGH_SPEED					"FaceRecognizer::AllowDetection::HighSpeed" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION								"FaceRecognizer::AllowExtraction" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_HIGH_PRECISION_ASIAN	"FaceRecognizer::AllowExtraction::Model::HighPrecisionAsian" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_HIGH_PRECISION			"FaceRecognizer::AllowExtraction::Model::HighPrecision" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_H3			            "FaceRecognizer::AllowExtraction::Model::HighPrecision3" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_ULTRA_HIGH_PRECISION	"FaceRecognizer::AllowExtraction::Model::UltraHighPrecision" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_UH3	                "FaceRecognizer::AllowExtraction::Model::UltraHighPrecision3" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_VERY_HIGH_PRECISION	"FaceRecognizer::AllowExtraction::Model::VeryHighPrecision" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_FACE_ATTRIBUTES				"FaceRecognizer::AllowExtraction::FaceAttributes" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_FACE_POSE					"FaceRecognizer::AllowExtraction::FacePose" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_MOTIONDETECTOR_ALLOW										"MotionDetector::Allow" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_LIVENESSDETECTOR_ALLOW									"FaceLivenessDetector::Allow" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_3D_DEPTH_LIVENESSDETECTOR_ALLOW							"Face3DDepthLivenessDetector::Allow" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_USER_ACTION_DETECTOR_ALLOW								"UserActionDetector::Allow" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_2D_ANTI_SPOOFING_DETECTOR_ALLOW							"Face2DAntiSpoofingDetector::Allow" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_QUALITY_DETECTOR_ALLOW								    "PhotoQualityDetector::Allow" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_QUALITY_DETECTOR_ALLOW_CHECK_FACESIZE				    "PhotoQualityDetector::Allow::FaceSize" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_QUALITY_DETECTOR_ALLOW_CHECK_OCCLUSION				    "PhotoQualityDetector::Allow::Occlusion" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_QUALITY_DETECTOR_ALLOW_CHECK_LIGHTING				    "PhotoQualityDetector::Allow::Lighting" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_QUALITY_DETECTOR_ALLOW_CHECK_BLURINESS				    "PhotoQualityDetector::Allow::Bluriness" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_QUALITY_DETECTOR_ALLOW_CHECK_GRAYSCALE				    "PhotoQualityDetector::Allow::Grayscale" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_QUALITY_DETECTOR_ALLOW_CHECK_FACEANGLE				    "PhotoQualityDetector::Allow::FaceAngle" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_CAMERAMANAGER_ALLOW_CAMERA								"CameraManager::Allow" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_FACE_OCCLUSION		        "FaceRecognizer::AllowExtraction::FaceOcclusion" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_UH3M                   "FaceRecognizer::AllowExtraction::Model::UltraHighPrecision3Mask" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_VHM			        "FaceRecognizer::AllowExtraction::Model::VeryHighPrecisionMask" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_H5			            "FaceRecognizer::AllowExtraction::Model::HighPrecision5" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_UH5	                "FaceRecognizer::AllowExtraction::Model::UltraHighPrecision5" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_UH5M	                "FaceRecognizer::AllowExtraction::Model::UltraHighPrecision5Mask" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_VH5	                "FaceRecognizer::AllowExtraction::Model::VeryHighPrecision5" //!< value: (bool *)&allow, args: nullptr
#define FR_LICENSE_PROPERTY_ID_RECOGNIZER_ALLOW_EXTRACTION_MODEL_VH5M	                "FaceRecognizer::AllowExtraction::Model::VeryHighPrecision5Mask" //!< value: (bool *)&allow, args: nullptr
	/**
	* Contains License Manager Configuration.
	*
	* @struct FR_LicenseManagerConfig
	*/
	struct FR_LicenseManagerConfig
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		const char *appBundlePath;		//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;		//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;		//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.
	};
}

#endif //__FACEME_LICENSE_MANAGER_DEF_H__
