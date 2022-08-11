/**
* @file FaceMeBaseDef.h
* The common definition for CyberLink FaceMe SDK.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_BASE_DEF_H__
#define __FACEME_BASE_DEF_H__
#include <stdint.h>

namespace FaceMeSDK
{
	/**
	* status return code.
	*
	* @enum EFR_RETURN
	*/
	enum EFR_RETURN
	{
		FR_RETURN_ASYNC = 2,						//!< Async operation.
		FR_RETURN_FALSE = 1,						//!< False.
		FR_RETURN_OK = 0,							//!< Succeeded.
		FR_RETURN_FAIL = -1,						//!< Failed.
		FR_RETURN_OUT_OF_MEMORY = -2,				//!< Out of memory error.
		FR_RETURN_NOT_IMPLEMENT = -3,				//!< Not implement error.
		FR_RETURN_INVALID_ARGUMENT = -4,			//!< Invalid argument error.
		FR_RETURN_INDEX_OUT_OF_RANGE = -5,			//!< Index out of range error.
		FR_RETURN_UNEXPECTED = -6,					//!< Unexpected error.
		FR_RETURN_INCONSISTENT_VERSION = -7,		//!< Inconsistent data structure version error.
		FR_RETURN_NOT_INIT = -8,					//!< Not initialize error.
		FR_RETURN_CONFIG_ERROR = -9,				//!< Configuration error.
		FR_RETURN_NOT_FOUND = -10,					//!< Not found error.
		FR_RETURN_NOT_SUPPORT = -11,				//!< Not support error.

		FR_RETURN_FEATURE_SIZE_MISMATCH = -30,		//!< Feature size mismatch.
		FR_RETURN_DATABASE_ERROR = -31,				//!< Database access error.
		FR_RETURN_FEATURE_INCOMPLETE = -32,			//!< Feature extract incomplete for add
		FR_RETURN_DATABASE_COLLECTION_EXCEEDED = -33, //!< Collection count in database is exceeded.

		FR_RETURN_UNKNOWN_HOST = -40,				//!< Unknown license server.
		FR_RETURN_NETWORK_ERROR = -41,				//!< Network issue while communicate with license server.
		FR_RETURN_LICENSE_FAIL = -42,				//!< License is failed.
		FR_RETURN_LICENSE_EXPIRED = -43,			//!< License is expired.
		FR_RETURN_LICENSE_ACTIVATION_EXCEEDED = -44,//!< License activation is exceeded.
		FR_RETURN_LICENSE_NOT_SUPPORT = -45,		//!< License feature is not support.
		FR_RETURN_LICENSE_CONSTRAIN_HARDWARE = -46,	//!< License hardware is constrained.
		FR_RETURN_LICENSE_SERVER_ERROR = -47,		//!< License server error.
		FR_RETURN_LICENSE_INVALID_PACKAGE_NAME = -48, //!< License is restricted with specific package name for Android, or bundleId for iOS.
		FR_RETURN_LICENSE_INCORRECT_KEY = -49,		//!< License key is incorrect.

		FR_RETURN_RUNTIME_FILE_NOT_COMPLETE = -50,  //!< FaceMe runtime files are not complete.
		FR_RETURN_INFERENCE_ENGINE_NOT_SUPPORT = -51, //!< Inference engine is not support.

		FR_RETURN_LICENSE_INCORRECT_PLATFORM = -60,	//!< License is not authorized on current platform.

		FR_RETURN_RECOGNIZE_IMAGE_PROCESSING_EXCEEDED = -70, //!< Recognize fps is exceeded.
	};

	/**
	* status return code.
	*
	* @see EFR_RETURN
	*/
	typedef int32_t FR_RETURN;

	/**
	* FaceMe instance type
	*
	* @enum EFR_INSTANCE_TYPE
	*/
	enum EFR_INSTANCE_TYPE
	{
		FR_INSTANCE_LICENSE_MANAGER = 1,			//!< Instance type of License Manager. 
		FR_INSTANCE_MOTION_DETECTOR = 4,			//!< Instance type of Motion Detector.
		FR_INSTANCE_QUALITY_DETECTOR = 9,			//!< Instance type of Quality Detector.
		FR_INSTANCE_CAMERA_MANAGER = 14,			//!< Instance type of Camera Manager.
		FR_INSTANCE_PERSON_CAMERA_RECOGNIZER = 17,	//!< Instance type of Person Camera Recognizer. 
		FR_INSTANCE_PERSON_RECOGNIZER = 19, //!< Instance type of Person Recognizer. 

		FR_INSTANCE_FACE_RECOGNIZER = 27,			//!< Instance type of Face Recognizer. 
		FR_INSTANCE_LOCAL_FACE_DATA_MANAGER = 22,	//!< Instance type of Local Face Manager. 
		FR_INSTANCE_ODBC_DATA_MANAGER = 23,			//!< Instance type of ODBC Face Manager. 
		FR_INSTANCE_LIVENESS_DETECTOR = 24,			//!< Instance type of Liveness Detector.
		FR_INSTANCE_USER_ACTION_DETECTOR = 25,		//!< Instance type of user action detector.
		FR_INSTANCE_CAMERA_RECOGNIZER = 28,			//!< Instance type of Camera Recognizer.
	};

	typedef int32_t FR_INSTANCE_TYPE;

	/**
	* pixel format.
	*
	* @enum EFR_PIXEL_FORMAT
	*/
	enum EFR_PIXEL_FORMAT
	{
		FR_PIXEL_FORMAT_UNKNOWN = 0,//!< Unknown.
		FR_PIXEL_FORMAT_BGRA,		//!< BGRA 32-bit pixel format.
		FR_PIXEL_FORMAT_RGBA,		//!< RGBA 32-bit pixel format.
		FR_PIXEL_FORMAT_BGR,		//!< BGR 24-bit pixel format.
		FR_PIXEL_FORMAT_RGB			//!< RGB 24-bit pxel format.
	};

	/**
	* pixel format.
	*
	* @see EFR_PIXEL_FORMAT
	*/
	typedef int32_t FR_PIXEL_FORMAT;

	/**
	* The type of emotion.
	*
	* @enum EFR_EMOTION
	*/
	enum EFR_EMOTION
	{
		FR_EMOTION_UNKNOWN = 0,	//!< Unknown.
		FR_EMOTION_HAPPY,		//!< happy.
		FR_EMOTION_SURPRISED,	//!< Surprised.
		FR_EMOTION_SAD,			//!< Sad.
		FR_EMOTION_ANGRY,		//!< Angry.
		FR_EMOTION_NEUTRAL,		//!< Neutral.
	};

	/**
	* The type of emotion.
	*
	* @see EFR_EMOTION
	*/
	typedef int32_t FR_EMOTION;

	/**
	* The type of gender.
	*
	* @enum EFR_GENDER
	*/
	enum EFR_GENDER
	{
		FR_GENDER_UNKNOWN = 0,	//!< Unknown.
		FR_GENDER_MALE,			//!< Male.
		FR_GENDER_FEMALE		//!< Female.
	};

	/**
	* The type of gender.
	*
	* @see EFR_GENDER
	*/
	typedef int32_t FR_GENDER;

	/**
	* The type of feature Landmark.
	*
	* @enum EFR_LANDMARK
	*/
	enum EFR_LANDMARK
	{
		FR_LANDMARK_EYE_LEFT = 0,		//!< Feature point of left eye.
		FR_LANDMARK_EYE_RIGHT,			//!< Feature point of right eye.
		FR_LANDMARK_EYE_NOSE,			//!< Feature point of nose.
		FR_LANDMARK_EYE_MOUTH_LEFT,		//!< Feature point of left mouth.
		FR_LANDMARK_EYE_MOUTH_RIGHT		//!< Feature point of right mouth.
	};

	/**
	* The type of feature Landmark
	*
	* @see EFR_LANDMARK
	*/
	typedef int32_t FR_LANDMARK;

	/**
	* The level of precision.
	*
	* @enum EFR_PRECISION_LEVEL
	*/
	enum EFR_PRECISION_LEVEL
	{
		FR_PRECISION_LEVEL_1E6 = 0,	//!< 1e-6
		FR_PRECISION_LEVEL_1E5,		//!< 1e-5
		FR_PRECISION_LEVEL_1E4,		//!< 1e-4
		FR_PRECISION_LEVEL_1E3,		//!< 1e-3
		FR_PRECISION_LEVEL_1E2		//!< 1e-2
	};

	/**
	* The level of precision.
	*
	* @see EFR_PRECISION_LEVEL
	*/
	typedef int32_t FR_PRECISION_LEVEL;


	/**
	* Face feature options.
	*
	* @enum EFR_FEATURE_OPTIONS
	*/
	enum EFR_FEATURE_OPTIONS
	{
		FR_FEATURE_OPTION_NONE = 0,									//!< None.
		FR_FEATURE_OPTION_BOUNDING_BOX = (1LL << 1),				//!< Extract Bounding box.
		FR_FEATURE_OPTION_FEATURE_LANDMARK = (1LL << 2),			//!< Extract Feature Landmark.
		FR_FEATURE_OPTION_FEATURE = (1LL << 3),						//!< Extract Feature.
		FR_FEATURE_OPTION_EMOTION = (1LL << 4),						//!< Extract Emotion.
		FR_FEATURE_OPTION_AGE = (1LL << 5),							//!< Extract Age.
		FR_FEATURE_OPTION_GENDER = (1LL << 6),						//!< Extract Gender.
		FR_FEATURE_OPTION_POSE = (1LL << 7),						//!< Extract Pose.
		FR_FEATURE_OPTION_OCCLUSION = (1LL << 0),					//!< Extract Occlusion.
		FR_FEATURE_OPTION_MASKED_FEATURE = (1LL << 8),				//!< Extract Masked Feature.
		FR_FEATURE_OPTION_FULL_FEATURE = ((1LL << 3) | (1LL << 8)),	//!< Extract Full Feature.
		FR_FEATURE_OPTION_ALL = INT32_MAX,							//!< Extract All.
	};

	/**
	* Face feature options.
	*
	* @see EFR_FEATURE_OPTIONS
	*/
	typedef int32_t FR_FEATURE_OPTIONS;

	/**
	* The type of face feature.
	*
	* @enum EFR_FEATURE_TYPE
	*/
	enum EFR_FEATURE_TYPE
	{
		FR_FEATURE_TYPE_NONE = 0,						//!< None.
		FR_FEATURE_TYPE_STANDARD_PRECISION = 1,			//!< Standard precision.
		FR_FEATURE_TYPE_HIGH_PRECISION = 2,				//!< High precision.
		FR_FEATURE_TYPE_ULTRA_HIGH_PRECISION = 3,		//!< Ultra-High precision.
		FR_FEATURE_TYPE_VERY_HIGH_PRECISION = 4,		//!< Very-High presicion.
		FR_FEATURE_TYPE_UH3 = 5,	                    //!< 3rd generation of Ultra-High precision.
		FR_FEATURE_TYPE_H3 = 6,							//!< 3rd generation of High precision.
		FR_FEATURE_TYPE_UH5 = 7,	                    //!< 5th generation of Ultra-High precision.
		FR_FEATURE_TYPE_VH5 = 8,	                    //!< 5th generation of Very-High presicion.
		FR_FEATURE_TYPE_H5 = 9,							//!< 5th generation of High precision.
	};

	/**
	* The type of face feature.
	*
	* @see EFR_FEATURE_TYPE
	*/
	typedef int32_t FR_FEATURE_TYPE;

	/**
	* The sub type of face feature.
	*
	* @enum EFR_FEATURE_SUB_TYPE
	*/
	enum EFR_FEATURE_SUB_TYPE
	{
		FR_FEATURE_SUB_TYPE_NONE = 0,	//!< None.
		FR_FEATURE_SUB_TYPE_ASIAN = 1,	//!< Asian.
		FR_FEATURE_SUB_TYPE_MASK = 2	//!< Mask.
	};

	/**
	* The sub type of face feature.
	*
	* @see EFR_FEATURE_SUB_TYPE
	*/
	typedef int32_t FR_FEATURE_SUB_TYPE;

	/**
	* The type of metadata of face feature.
	*
	* @enum EFR_FEATURE_METADATA_TYPE
	*/
	enum EFR_FEATURE_METADATA_TYPE
	{
		FR_FEATURE_METADATA_TYPE_NONE = 0,	//!< None.
		FR_FEATURE_METADATA_TYPE_MASK = 1	//!< Mask.
	};

	/**
	* The type of metadata of face feature.
	*
	* @see EFR_FEATURE_METADATA_TYPE
	*/
	typedef int32_t FR_FEATURE_METADATA_TYPE;

	/**
	* Face occlusion status.
	*
	* @enum EFR_OCCLUSION_STATUS
	*/
	enum EFR_OCCLUSION_STATUS
	{
		FR_OCCLUSION_STATUS_NONE = 0,						//!< None.
		FR_OCCLUSION_STATUS_LEFT_EYE = (1LL << 0),			//!< Left Eye Occluded.
		FR_OCCLUSION_STATUS_RIGHT_EYE = (1LL << 1),			//!< Right Eye Occluded.
		FR_OCCLUSION_STATUS_NOSE = (1LL << 2),				//!< Nose Occluded.
		FR_OCCLUSION_STATUS_MOUTH = (1LL << 3),				//!< Mouth Occluded.
		FR_OCCLUSION_STATUS_MASK = (1LL << 4)				//!< Mask detected.
	};

	/**
	* Face occlusion status.
	*
	* @see EFR_OCCLUSION_STATUS
	*/
	typedef int32_t FR_OCCLUSION_STATUS;

	/**
	* Contains point information in an XY coordinate.
	*
	* @struct FR_Point
	*/
	struct FR_Point
	{
		int16_t x;	//!< X.
		int16_t y;	//!< Y.
	};

	/**
	* Contains two points of a rectangle in the XY coordinate.
	*
	* @struct FR_Rectangle
	*/
	struct FR_Rectangle
	{
		FR_Point topLeft;		//!< The point of left-top corner.
		FR_Point bottomRight;	//!< The point of right-bottom corner.
	};


	/**
	* Contains feature points of a face.
	*
	* @struct FR_FaceLandmark
	*/
	struct FR_FaceLandmark
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		uint32_t featurePointSize;		//!< The size of the feature points.
		FR_Point featurePoints[100];	//!< The feature points. The returned array includes 5 feature points, in the order of Left Eye, Right Eye, Nose, Left point of the mouth corner, Right point of the mouth corner. See @ref FaceMeSDK::EFR_LANDMARK as the index for each landmark
	};

	/**
	* The bitmap and the related information of an image.
	*
	* @struct FR_Image
	*/
	struct FR_Image
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		void *data;						//!< The image raw data.
		uint32_t width;					//!< The width of the image.
		uint32_t height;				//!< The height of the image.
		uint32_t stride;				//!< The line stride of the image.
		uint32_t channel;				//!< The channels of a pixel.
		FR_PIXEL_FORMAT pixelFormat;	//!< The pixel format.
	};

	/**
	* Contains pose of a face.
	*
	* @struct FR_Pose
	*/
	struct FR_Pose
	{
		float yaw;		//!< Yaw. The angle (in degrees) for the face turning to left or right. Minus values represent angles that the face turn to his right hand side (left if see from the camera). Positive values represent angles that the face turn to his right hand side (right if see from the camera).
		float pitch;	//!< Pitch. The angle (in degrees) for the face raising up or down. Positive value means the angle that face raises up.
		float roll;		//!< Roll. The angle (in degrees) for the face roll to the left or right. Positive values means the angles that the face rolls to his right.
	};

	/**
	* Contains feature data of a face.
	*
	* @struct FR_FeatureData
	*/
	struct FR_FeatureData
	{
		uint32_t size;		//!< The size of the feature data.
		float data[2048];	//!< The feature data.
	};

	/**
	* Contains threshold values for a certain face feature extractor based on different False Acceptance Rates(FARs).
	*
	* @struct FR_FaceFeatureScheme
	*/
	struct FR_FaceFeatureScheme
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		float threshold_1_1e6;				//!< The confidence threshold for FAR of 1/1e6.
		float threshold_1_1e5;				//!< The confidence threshold for FAR of 1/1e5.
		float threshold_1_1e4;				//!< The confidence threshold for FAR of 1/1e4.
		float threshold_1_1e3;				//!< The confidence threshold for FAR of 1/1e3.
		float threshold_1_1e2;				//!< The confidence threshold for FAR of 1/1e2.
		FR_FEATURE_TYPE featureType;		//!< The type of the face feature.
		FR_FEATURE_SUB_TYPE featureSubType;	//!< The sub type of the face feature.
		FR_FEATURE_METADATA_TYPE metadataType;  //!< The type of metadata of the face feature.
		uint32_t metadataSize;					//!< The size of the metadata.
		uint8_t metadata[256];					//!< The custom metadata.
	};

	/**
	* Contains information of a face.
	*
	* @struct FR_FaceInfo
	*/
	struct FR_FaceInfo
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		float confidence;					//!< The confidence of the face.
		FR_FEATURE_OPTIONS options;			//!< The options of extracted feature.
		FR_Rectangle boundingBox;			//!< The bounding box of the face.
		FR_OCCLUSION_STATUS occlusion;		//!< The status of face occlusion. 
	};

	/**
	* The type of vector that stores confidence.
	*
	*/
	typedef struct FR_Confidence
	{
		uint32_t size = 0;
		float confidence[100];
	} FR_Confidence;

	/**
	* Contains attributes of a face.
	*
	* @struct FR_FaceAttribute
	*/
	struct FR_FaceAttribute
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		FR_FEATURE_OPTIONS options;			//!< The options of extracted feature.
		float age;							//!< The age of the face.
		FR_EMOTION emotion;					//!< The emotion of the face.
		FR_GENDER gender;					//!< The gender of the face.
		FR_Pose pose;					    //!< The pose of the face.
		FR_Confidence emotionConfidence; //!< The emotion confidence of the face.
		FR_Confidence genderConfidence;  //!< The gender confidence of the face.
	};

	/**
	* Contains feature data of a face.
	*
	* @struct FR_FaceFeature
	*/
	struct FR_FaceFeature
	{
		uint32_t sizeOfStructure;			//!< The size of the structure.
		FR_FEATURE_TYPE featureType;		//!< The type of the face feature.
		FR_FEATURE_SUB_TYPE featureSubType;	//!< The sub type of the face feature.
		FR_FeatureData featureData;			//!< The feature data of the face feature.
	};

	/**
	* Callback method For instance creation by instance type.
	*
	* @param[in] type The type of instance to create.
	* @param[out] instance Retrieve a new instance by type.
	* @return Returns an <code>FR_RETURN</code>. Possible values include, but
	* are not limited to, the following list.
	*  * <code>FR_RETURN_OK</code>: succeeded.
	*  * <code>ENGINE_RETURN_FAIL</code>: failed.
	*/
	typedef FR_RETURN(*FnFaceMeCreateInstance)(FR_INSTANCE_TYPE type, void **instance);


#define FR_OPT(option) ((int32_t)(option))
#define FR_SUCCEEDED(hr) (((FaceMeSDK::FR_RETURN)(hr)) >= 0)
#define FR_FAILED(hr) (((FaceMeSDK::FR_RETURN)(hr)) < 0)
}

#endif //__FACEME_BASE_DEF_H__
