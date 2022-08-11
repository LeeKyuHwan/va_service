/**
 * @file IFaceMePersonRecognizer.h
 * The interface of CyberLink FaceMe Person Recognizer.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEMEPERSONRECOGNIZER_H__
#define __FACEME_IFACEMEPERSONRECOGNIZER_H__
#include "FaceMePersonRecognizerDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink Person Recognizer
	* @interface IFaceMePersonRecognizer
	*/
	class IFaceMePersonRecognizer : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMePersonRecognizer() {}

		/**
		* Object identifiers of FaceMe Person Recognizer.
		*
		* @enum FaceMePersonRecognizerOIDs
		*/
		enum FaceMePersonRecognizerOIDs
		{
			OID_FaceMePersonRecognizer = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of FaceMe Recognizer.
		};

		// General
		/**
		* Initialize FaceMe Person Recognizer.
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
		virtual FR_RETURN Initialize(const FR_PersonRecognizerConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

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

		// Person recognition

		/**
		* Process person detection.
		*
		* @param[in] detectConfig For configuring the result.
		* @param[in] image An images to be processed.
		* @param[out] personCount Retrieve the number of persons in input image.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_NOT_FOUND</code>: No person is detected.
		*/
		virtual FR_RETURN DetectPerson(const FR_Image *image, uint32_t *personCount) = 0;

		/**
		* Retrieve the information of a specified Person.
		*
		* @param[in] imageIndex The index of a specified image in the list of images, should be less than the number of images, the input argument of @ref DetectPerson.
		* @param[in] personIndex The index of person in the list of detected persons of a specified image, should be less than count, the output argument of @ref DetectPerson.
		* @param[in,out] personInfo Person information data of the selected person. The data include the confidence level for the person detection result and a bounding box of the detected person.Be sure to enable FR_FEATURE_OPTION_BOUNDING_BOX in extract_config when calling @ref DetectPerson.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is larger than the count of persons or images in recognized image.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*/
		virtual FR_RETURN GetPersonInfo(uint32_t personIndex, FR_PersonInfo *personInfo) = 0;
	};
}

#endif // __FACEME_IFACEMEPERSONRECOGNIZER_H__