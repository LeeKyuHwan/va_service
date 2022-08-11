/**
 * @file IFaceMeQualityDetector.h
 * The interface of CyberLink FaceMe Quality Detector.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEMEQUALITYETECTOR_H__
#define __FACEME_IFACEMEQUALITYETECTOR_H__
#include "FaceMeQualityDetectorDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink FaceMe Quality Detector
	* @interface IFaceMeQualityDetector
	*/
	class IFaceMeQualityDetector : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeQualityDetector() {}

		/**
		* Object identifiers of FaceMe Quality Detector.
		*
		* @enum FaceMeQualityDetectorOIDs
		*/
		enum FaceMeQualityDetectorOIDs
		{
			OID_FaceMeQualityDetector = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe Quality Detector.
		};

		// General
		/**
		* Initialize FaceMe Quality Detector.
		*
		* @param[in] config The configuration for quality detector setup.
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
		*/
		virtual FR_RETURN Initialize(const FR_QualityDetectorConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

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

		/**
		* Check image quality.
		*
		* @param[in] detectConfig Configuration for detection.
		* @param[in] image An image to be processed.
		* @param[out] result Retrieve the result that if there is any quality issue be detected.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FALSE</code>: Detecting is in progress.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN Detect(const FR_QualityDetectConfig *detectConfig, const FR_Image *image, FR_QualityDetectResult *result) = 0;

	};
}

#endif // __FACEME_IFACEMEQUALITYETECTOR_H__