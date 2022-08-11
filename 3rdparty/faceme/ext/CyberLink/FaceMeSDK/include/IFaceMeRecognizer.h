/**
 * @file IFaceMeRecognizer.h
 * The interface of CyberLink FaceMe Recognizer.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEMERECOGNIZER_H__
#define __FACEME_IFACEMERECOGNIZER_H__
#include "FaceMeRecognizerDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink Face Recognizer
	* @interface IFaceMeRecognizer
	*/
	class IFaceMeRecognizer : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeRecognizer() {}

		/**
		* Object identifiers of FaceMe Recognizer.
		*
		* @enum FaceMeRecognizerOIDs
		*/
		enum FaceMeRecognizerOIDs
		{
			OID_FaceMeRecognizer = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of FaceMe Recognizer.
		};

		// General
		/**
		* Initialize FaceMe Recognizer.
		*
		* @param[in] config The configuration for recognizer setup.
		* @param[in] licenseInfo The license information provided by CyberLink. The format of license information is a raw buffer.
		* @param[in] length Length of the license information.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: The license feature is not supported.
		*  * <code>FR_RETURN_LICENSE_CONSTRAIN_HARDWARE</code>: License hardware is constrained.
		*/
		virtual FR_RETURN Initialize(const FR_RecognizerConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

		/**
		* Finalize FaceMe SDK and release corresponding resources.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN Finalize() = 0;

		/**
		* Retrieve feature scheme and model threshold values.
		*
		* @param[in,out] featureScheme Retrieve the scheme information and the False Acceptance Rate of levels.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: The license feature is not supported.
		* @note License registration must have done before initialization.
		*/
		virtual FR_RETURN GetFeatureScheme(FR_FaceFeatureScheme *featureScheme) = 0;

		/**
		* Set property.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[in] value The specified type of reference variant value by property identifier.
		* @param[in] args The specified type of reference variant arguments by property identifier.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The property identifier is not found.
		*/
		virtual FR_RETURN SetProperty(const char *propertyId, void *value, void *args) = 0;

		/**
		* Get property.
		*
		* @param[in] propertyId An utf-8 encoded string of property identifier.
		* @param[out] value Retrieve the specified type of reference variant value by property identifier.
		* @param[out] args Retrieve the specified type of reference variant arguments by property identifier.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The property identifier is not found.
		*/
		virtual FR_RETURN GetProperty(const char *propertyId, void *value, void *args) = 0;

		// Face recognition
		/**
		* Retrieve current setting of face extraction.\n
		*
		* @param[in] extractionOption The options of extraction: minimum and maximum face width, speed and result order.
		* @param[in,out] value Retrieve the current setting of the extractionOption.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetExtractionOption(FR_EXTRACTION_OPTION extractionOption, uint32_t *value) = 0;

		/**
		* Set prefer setting of face extraction.
		*
		* @param[in] extractionOption The options of extraction: minimum and maximum face width, speed and result order.
		* @param[in] value The assigned setting of the extractionOption.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN SetExtractionOption(FR_EXTRACTION_OPTION extractionOption, uint32_t value) = 0;

		/**
		* Process face detection and extraction.
		*
		* @param[in] extractConfig For configuring the result. Recognition will be processed if enable one of name, age, emotion, or gender. Otherwise face detection only.
		* @param[in] images The array of images to be processed.
		* @param[in] numOfImages The number of images to be processed.
		* @param[out] faceCounts Retrieve the array of number of faces in input images. The size of array should be the same as array of images.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_NOT_FOUND</code>: No face is detected.
		*/
		virtual FR_RETURN ExtractFace(const FR_ExtractConfig *extractConfig, const FR_Image *images, uint32_t numOfImages, uint32_t *faceCounts) = 0;

		/**
		* Retrieve the information of a specified face.
		*
		* @param[in] imageIndex The index of a specified image in the list of images, should be less than the number of images, the input argument of @ref ExtractFace.
		* @param[in] faceIndex The index of face in the list of detected faces of a specified image, should be less than count, the output argument of @ref ExtractFace.
		* @param[in,out] faceInfo Face information data of the selected face. The data include the confidence level for the face detection result and a bounding box of the detected face.Be sure to enable FR_FEATURE_OPTION_BOUNDING_BOX in extract_config when calling @ref ExtractFace.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is larger than the count of faces or images in recognized image.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN GetFaceInfo(uint32_t imageIndex, uint32_t faceIndex, FR_FaceInfo *faceInfo) = 0;

		/**
		* Retrieve the landmark of a specified face.
		*
		* @param[in] imageIndex The index of a specified image in the list of images, should be less than the number of images, the input argument of @ref ExtractFace.
		* @param[in] faceIndex The index of face in the list of detected faces of a specified image, should be less than count, the output argument of @ref ExtractFace.
		* @param[in,out] faceLandmark Returned landmarks of eyes, nose and corners of mouth. Be sure to enable FR_FEATURE_OPTION_FEATURE_LANDMARK in extract_config when calling @ref ExtractFace.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is larger than count the of faces or images in recognized image.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN GetFaceLandmark(uint32_t imageIndex, uint32_t faceIndex, FR_FaceLandmark *faceLandmark) = 0;

		/**
		* Retrieve the attribute of a specified face.
		*
		* @param[in] imageIndex The index of a specified image in the list of images, should be less than the number of images, the input argument of @ref ExtractFace.
		* @param[in] faceIndex The index of face in the list of detected faces of a specified image, should be less than count, the output argument of @ref ExtractFace.
		* @param[in,out] faceAttribute Face attributes like Age, Gender, Emotion and Pose data of the selected face. Be sure to enable FaceMeSDK::FR_FEATURE_OPTION_EMOTION = (1LL << 4), FaceMeSDK::FR_FEATURE_OPTION_AGE = (1LL << 5), FaceMeSDK::FR_FEATURE_OPTION_GENDER = (1LL << 6), FaceMeSDK::FR_FEATURE_OPTION_POSE in extract_config when calling @ref ExtractFace.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is larger than the count of faces or images in recognized image.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: The license feature is not supported.
		*/
		virtual FR_RETURN GetFaceAttribute(uint32_t imageIndex, uint32_t faceIndex, FR_FaceAttribute *faceAttribute) = 0;

		/**
		* Retrieve feature data of a specified face.
		*
		* @param[in] imageIndex The index of a specified image in the list of images, should be less than the number of images, the input argument of @ref ExtractFace.
		* @param[in] faceIndex The index of face in the list of detected faces of a specified image, should be less than count, the output argument of @ref ExtractFace.
		* @param[in,out] faceFeature Face feature data of the selected face.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is larger than the count of faces or images in recognized image.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: The license feature is not supported.
		*/
		virtual FR_RETURN GetFaceFeature(uint32_t imageIndex, uint32_t faceIndex, FR_FaceFeature *faceFeature) = 0;

		/**
		* Compare the similarity between two faces.
		*
		* @param[in] faceFeatureA Represents the feature vector of the first face.
		* @param[in] faceFeatureB Represents the feature vector of the second face.
		* @param[out] confidence The similarity confidence ranged from 0.0 to 1.0 between two faces.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: The license feature is not supported.
		*/
		virtual FR_RETURN CompareFaceFeature(const FR_FaceFeature *faceFeatureA, const FR_FaceFeature *faceFeatureB, float *confidence) = 0;
	};
	
	/**
	* CyberLink Face Recognizer EX API.
	* @interface IFaceMeRecognizerEx
	*/
	class IFaceMeRecognizerEx : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeRecognizerEx() {}

		enum FaceMeRecognizerOIDs
		{
			OID_FaceMeRecognizerEx = (BaseObject::OID_RefCountedObject + 2)	//!< Object identifier of FaceMe Recognizer Ex.
		};

		/**
		* Process face detection and extraction.
		*
		* @param[in] detectConfig For configuring the result. Recognition will be processed if enable one of bounding box, landmarks or mask occlusion(depends on license capabilities).
		* @param[in] image An images to be processed.
		* @param[out] faceCount Retrieve the number of faces in input image.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_NOT_FOUND</code>: No face is detected.
		*/
		virtual FR_RETURN DetectFace(const FR_DetectConfig *detectConfig, const FR_Image *image, uint32_t *faceCount) = 0;

		/**
		* Process face detection and extraction.
		*
		* @param[in] extractConfig For configuring the result. Recognition will be processed if enable one of name, age, emotion, or gender. Otherwise face detection only.
		* @param[in] image An images to be processed.
		* @param[in] faceInfos The array of face information data for faces to be extracted. The data includes the confidence level for the face detection result, a bounding box of the detected face and mask occlusion status(depends on license capabilities).
		* @param[in] faceLandmarks The array of facial feature landmarks including eyes, nose, and mouth corners of the detected faces.
		* @param[in] faceCount The number of face to be processed.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_NOT_FOUND</code>: No face is detected.
		*/
		virtual FR_RETURN ExtractFace(const FR_ExtractConfig *extractConfig, const FR_Image *image, const FR_FaceInfo *faceInfos, const FR_FaceLandmark *faceLandmarks, uint32_t faceCount) = 0;
	};
}

#endif // __FACEME_IFACEMERECOGNIZER_H__