/**
 * @file IFaceMeUserActionDetector.h
 * The interface of CyberLink FaceMe User Action Detector.
 *
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEMEUSERACTIONDETECTOR_H__
#define __FACEME_IFACEMEUSERACTIONDETECTOR_H__
#include "FaceMeUserActionDetectorDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink FaceMe User Action Detector
	* @interface IFaceMeUserActionDetector
	*/
	class IFaceMeUserActionDetector : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeUserActionDetector() {}

		/**
		* Object identifiers of FaceMe User Action Detector.
		*
		* @enum FaceMeUserActionDetectorOIDs
		*/
		enum FaceMeUserActionDetectorOIDs
		{
			OID_FaceMeUserActionDetector = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe User Action Detector.
		};

		// General
		/**
		* Initialize FaceMe User Action Detector.
		*
		* @param[in] config The configuration for user action detector setup.
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
		virtual FR_RETURN Initialize(const FR_UserActionDetectorConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

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
		 * Set User Action that detector will detect to.
		 *
		 * @param[in] userAction The user action that want to detect.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The user action is not found.
		 */
		virtual FR_RETURN SetUserAction(const FR_USER_ACTION userAction) = 0;

		/**
		* Detect user action from specific face info, attributes and/or face features.
		*
		* @param[in] detectConfig Configuration for detection.
		* @param[out] isMatched Retrieve the result that if there is a possible action be matched.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FALSE</code>: Detecting is in progress.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN Detect(const FR_UserActionDetectConfig *detectConfig, bool *isMatched) = 0;

	};
}

#endif // __FACEME_IFACEMEUSERACTIONDETECTOR_H__
