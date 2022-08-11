/**
* @file FaceMeCameraRecognizerDef.h
* The common definition for CyberLink Camera Recognizer.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FR_CAMERA_RECOGNIZER_DEF_H__
#define __FR_CAMERA_RECOGNIZER_DEF_H__
#include <stdint.h>
#include "FaceMeBaseDef.h"
#include "FaceMeRecognizerDef.h"
#include "FaceMeQualityDetectorDef.h"
#include "FaceMeLivenessDetectorDef.h"

namespace FaceMeSDK
{
#define FR_CAMERA_RECOGNIZER_PROPERTY_ID_GET_NUM_OF_GPU_DEVICE "NumOfGPUDevice"		//!< The type of value argument should be (uint32_t *). The value of args argument should be nullptr.
#define FR_CAMERA_RECOGNIZER_PROPERTY_ID_GET_NUM_OF_VPU_DEVICE "NumOfVPUDevice"		//!< The type of value argument should be (uint32_t *). The value of args argument should be char* for bundle path.


	/**
	* The type of FaceMe Camera Recognizer Handler.
	*
	* @enum EFR_CAMERA_RECOGNIZER_HANDLER_TYPE
	*/
	enum EFR_CAMERA_RECOGNIZER_HANDLER_TYPE
	{
		FR_CAMERA_RECOGNIZER_HANDLER_TYPE_DEFAULT = 0	//!< @see IFaceMeCameraRecognizerHandler .
	};
	/**
	* The type of FaceMe Camera Recognizer Handler.
	*
	* @see EFR_CAMERA_RECOGNIZER_HANDLER_TYPE
	*/
	typedef int32_t FR_CAMERA_RECOGNIZER_HANDLER_TYPE;

	/**
	* config preference for FaceMe Camera Recognizer.
	*
	* @enum EFR_CAMERA_RECOGNIZER_PREFERENCE
	*/
	enum EFR_CAMERA_RECOGNIZER_PREFERENCE
	{
		FR_CAMERA_RECOGNIZER_PREFERENCE_NONE = 0,						       //!< None.
		FR_CAMERA_RECOGNIZER_PREFERENCE_FAST_DETECTION = (1 << 1),				   //!< Prefers Fast detection.
		FR_CAMERA_RECOGNIZER_PREFERENCE_HARDWARE = (1 << 2),				   //!< Prefers  GPU hardware acceleration.
		FR_CAMERA_RECOGNIZER_PREFERENCE_MOVIDIUS_VPU_DETECTION = (1 << 3),	   //!< Prefers Intel Movidius VPU acceleration.
		FR_CAMERA_RECOGNIZER_PREFERENCE_MOVIDIUS_VPU_EXTRACTION = (1 << 4),	   //!< Prefers Intel Movidius VPU acceleration.
		FR_CAMERA_RECOGNIZER_PREFERENCE_FAST_TRACKING = (1 << 5),				   //!< Prefers Fast tracking.
		FR_CAMERA_RECOGNIZER_PREFERENCE_OCCLUSION = (1 << 6),				   //!< Prefers to enable occlusion detection.
		FR_CAMERA_RECOGNIZER_PREFERENCE_MOVIDIUS_VPU_HDDL = (1 << 7)		   //!< Prefers Intel Movidius VPU acceleration with HDDL plugin.
	};

	/**
	* config preference for FaceMe Camera Recognizer.
	*
	* @see FR_CAMERA_RECOGNIZER_PREFERENCE
	*/
	typedef int32_t FR_CAMERA_RECOGNIZER_PREFERENCE;

	/**
	* The type of Extracted image.
	*
	* @enum EFR_EXTRACT_IMAGE_TYPE
	*/
	enum EFR_EXTRACT_IMAGE_TYPE
	{
		FR_EXTRACT_IMAGE_TYPE_HOST_MEM = 0		//!< Extract image to the host memory.
	};

	/**
	* The type of Extracted image.
	*
	* @see EFR_EXTRACT_IMAGE_TYPE
	*/
	typedef int32_t FR_EXTRACT_IMAGE_TYPE;

	/**
	* The type of feature scheme
	*
	* @enum EFR_CAMERA_RECOGNIZER_FEATURE_SCHEME_TYPE
	*/
	enum EFR_CAMERA_RECOGNIZER_FEATURE_SCHEME_TYPE
	{
		FR_CAMERA_RECOGNIZER_FEATURE_SCHEME_TYPE_EXTRACTION = 0,				//!< feature scheme for extraction.
		FR_CAMERA_RECOGNIZER_FEATURE_SCHEME_TYPE_ADVANCED_EXTRACTION = 1		//!< feature scheme for advanced extraction.
	};

	/**
	* The type of Extracted image.
	*
	* @see EFR_CAMERA_RECOGNIZER_FEATURE_SCHEME_TYPE
	*/
	typedef int32_t FR_CAMERA_RECOGNIZER_FEATURE_SCHEME_TYPE;

	/**
	* Contains Recognizer Configuration for Cameras.
	*
	* @struct FR_CameraRecognizerConfig
	*/
	struct FR_CameraRecognizerConfig
	{
		uint32_t sizeOfStructure;											//!< The size of the structure.
		const char *appBundlePath;											//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;											//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;											//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.
		FR_CAMERA_RECOGNIZER_PREFERENCE preference;							//!< @see EFR_CAMERA_RECOGNIZER_PREFERENCE .
		FR_DETECTION_MODEL_SPEED_LEVEL detectionModelSpeedLevel;			//!< @see EFR_DETECTION_MODEL_SPEED_LEVEL .
		int32_t maxDetectionThreads;										//!< The maximum number of threads to be used for face detection.
		FR_EXTRACTION_MODEL_SPEED_LEVEL extractionModelSpeedLevel;			//!< @see EFR_EXTRACTION_MODEL_SPEED_LEVEL .
		int32_t maxExtractionThreads;										//!< The maximum number of threads to be used for face extraction.
		FR_EXTRACTION_MODEL_SPEED_LEVEL advancedExtractionModelSpeedLevel;	//!< @see EFR_EXTRACTION_MODEL_SPEED_LEVEL for advanced face extraction. Set default when no advanced face extraction.
		int32_t maxAdvancedExtractionThreads;								//!< The maximum number of threads to be used for advanced face extraction.
		int32_t maxAdvancedExtractionInstance;								//!< The maximum number of instance to be used for advenced face extraction.
		uint32_t gpuId;														//!< The GPU device identifier.
		uint32_t maxFrameWidth;												//!< The maximum width of frame.
		uint32_t maxFrameHeight;											//!< The maximum height of frame.
		uint32_t minFaceWidth;												//!< The minimum pixel width of face.
		FR_DETECTION_OUTPUT_ORDER detectOutputOrder;						//!< @see EFR_DETECTION_OUTPUT_ORDER .
		uint32_t maxNumOfCameras;											//!< Maximum number of cameras for GPU memory usage control. Set zero as default.
		uint32_t preferredDetectionBatchSize;								//!< The preferred detection batch size
		uint32_t preferredExtractionBatchSize;								//!< The preferred extraction batch size
	};

	/**
	* Contains result of face detection
	*
	* @struct FR_FaceDetectResult
	*/
	struct FR_FaceDetectResult
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.
		uint32_t imageWidth;						//!< The width of image frame.
		uint32_t imageHeight;						//!< The height of image frame.
		uint32_t faceCount;							//!< The number of faces in the image.
		const FR_FaceInfo *faceInfos;				//!< The array of face information in the image.
		const FR_FaceLandmark *faceLandmarks;		//!< The array of face landmarks in the image.
	};

	/**
	* Contains next actions after face detection
	*
	* @struct FR_FaceDetectNextActions
	*/
	struct FR_FaceDetectNextActions
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.

		FR_FEATURE_OPTIONS extractOptions;				//!< Setting feature options when need face extraction. The default value is FR_FEATURE_OPTION_NONE.
		bool *extractFilter;							//!< Setting to false in the array when a face do not need face extraction. The default value is true.

		FR_FEATURE_OPTIONS advancedExtractOptions;		//!< Setting feature options when need advanced face extraction. The default value is false.
		bool *advancedExtractFilter;					//!< Setting to false in the array when a face do not need advanced face extraction. The default value is true.

		bool needExtractImage;							//!< Setting to true when need image frame extraction. The default value is false.
		FR_Rectangle extractImageRect;					//!< Setting Region Of Interest for image frame extraction. The default rectangle is empty.
		float extractImageScalingFactor;				//!< Setting scaling factor for image frame extraction. The default value is 1.0f.
		FR_EXTRACT_IMAGE_TYPE extractImageType;			//!< Setting the type of extracted image. The default is extracted to the host memory.

		bool needExtractFaceImages;						//!< Setting to true when need face image extraction. The default value is false.
		FR_Rectangle *extractFaceImageRects;			//!< Setting the array of Region Of Interest for face images extraction. The default rectangles are empty.
		float *extractFaceImageScalingFactors;			//!< Setting array of scaling factors for face images extraction. The default values are 1.0f.
		FR_EXTRACT_IMAGE_TYPE *extractFaceImageTypes;	//!< Setting the type of extracted face image. The default is extracted to the host memory.
	};

	/**
	* Contains result of face extraction
	*
	* @struct FR_FaceExtractResult
	*/
	struct FR_FaceExtractResult
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.
		uint32_t imageWidth;						//!< The width of image frame.
		uint32_t imageHeight;						//!< The height of image frame.
		uint32_t faceCount;							//!< The number of faces in the image.
		bool isAdvancedExtraction;					//!< Determine the extraction result is advanced face extraction or not.
		const FR_FaceInfo *faceInfos;				//!< The array of face information in the image.
		const FR_FaceLandmark *faceLandmarks;		//!< The array of face landmarks in the image.
		const FR_FaceFeature *faceFeatures;			//!< The array of face features in the image.
		const FR_FaceAttribute *faceAttributes;		//!< The array of face attributes in the image.
	};

	/**
	* Contains next actions after face extraction
	*
	* @struct FR_FaceExtraceNextActions
	*/
	struct FR_FaceExtractNextActions
	{
		uint32_t sizeOfStructure;						//!< The size of the structure.

		FR_FEATURE_OPTIONS advancedExtractOptions;			//!< Setting to true when need advanced face extraction. The default value is FR_FEATURE_OPTION_NONE.
		bool *advancedExtractFilter;					//!< Setting to false in the array when a face do not need advanced face extraction. The default value is true.

		bool needExtractImage;							//!< Setting to true when need image frame extraction. The default value is false.
		FR_Rectangle extractImageRect;					//!< Setting Region Of Interest for image frame extraction. The default rectangle is empty.
		float extractImageScalingFactor;				//!< Setting scaling factor for image frame extraction. The default value is 1.0f.
		FR_EXTRACT_IMAGE_TYPE extractImageType;			//!< Setting the type of extracted image frame. The default is extracted to the host memory.

		bool needExtractFaceImages;						//!< Setting to true when need face image extraction. The default value is false.
		FR_Rectangle *extractFaceImageRects;			//!< Setting the array of Region Of Interest for face images extraction. The default rectangles are empty.
		float *extractFaceImageScalingFactors;			//!< Setting the array of scaling factors for face images extraction. The default values are 1.0f.
		FR_EXTRACT_IMAGE_TYPE *extractFaceImageTypes;	//!< Setting  the array of type for extracted face images. The default type is extracted to the host memory.

		bool needQualiyCheck;							//!< Setting to true when need face quality check. The default value is false.
	};

	/**
	* Contains result of face quality checking
	*
	* @struct FR_FaceQualityCheckResult
	*/
	struct FR_FaceQualityCheckResult
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.
		uint32_t imageWidth;						//!< The width of image frame.
		uint32_t imageHeight;						//!< The height of image frame.
		uint32_t faceCount;							//!< The number of faces in the image.
		const FR_FaceInfo *faceInfos;				//!< The array of face information in the image.
		const FR_FaceLandmark *faceLandmarks;		//!< The array of face landmarks in the image.
		const FR_FaceFeature *faceFeatures;			//!< The array of face features in the image.
		const FR_FaceAttribute *faceAttributes;		//!< The array of face attributes in the image.
		const FR_QualityDetectResult *results;		//!< The array of quality check result in the image.
	};

	/**
	* Contains next actions after face quality checking
	*
	* @struct FR_FaceQualityCheckNextActions
	*/
	struct FR_FaceQualityCheckNextActions
	{
		uint32_t sizeOfStructure;						//!< The size of the structure.
		FR_FEATURE_OPTIONS advancedExtractOptions;			//!< Setting to true when need advanced face extraction. The default value is FR_FEATURE_OPTION_NONE.
		bool *advancedExtractFilter;					//!< Setting to false in the array when a face do not need advanced face extraction. The default value is true.

		bool needExtractImage;							//!< Setting to true when need image frame extraction. The default value is false.
		FR_Rectangle extractImageRect;					//!< Setting Region Of Interest for image frame extraction. The default rectangle is empty.
		float extractImageScalingFactor;				//!< Setting scaling factor for image frame extraction. The default value is 1.0f.
		FR_EXTRACT_IMAGE_TYPE extractImageType;			//!< Setting the type of extracted image frame. The default is extracted to the host memory.

		bool needExtractFaceImages;						//!< Setting to true when need face image extraction. The default value is false.
		FR_Rectangle *extractFaceImageRects;			//!< Setting the array of Region Of Interest for face images extraction. The default rectangles are empty.
		float *extractFaceImageScalingFactors;			//!< Setting the array of scaling factors for face images extraction. The default values are 1.0f.
		FR_EXTRACT_IMAGE_TYPE *extractFaceImageTypes;	//!< Setting the array of type of extracted face images. The default type is extracted to the host memory.
	};

	/**
	* Contains result of face images
	*
	* @struct FR_FaceImageResult
	*/
	struct FR_FaceImageResult
	{
		uint32_t sizeOfStructure;					//!< The size of the structure.
		uint32_t faceCount;							//!< The number of faces in the image.
		const FR_Image *faceImages;					//!< The array of face image datas.
	};
}

#endif //__FR_CAMERA_RECOGNIZER_DEF_H__
