/**
 * @file IFaceMeMotionDetector.h
 * The interface of CyberLink FaceMe Motion Detector.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEMEMOTIONDETECTOR_H__
#define __FACEME_IFACEMEMOTIONDETECTOR_H__
#include "FaceMeMotionDetectorDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink FaceMe Motion Detector
	* @interface IFaceMeMotionDetector
	*/
	class IFaceMeMotionDetector : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeMotionDetector() {}

		/**
		* Object identifiers of FaceMe Motion Detector.
		*
		* @enum FaceMeMotionDetectorOIDs
		*/
		enum FaceMeMotionDetectorOIDs
		{
			OID_FaceMeMotionDetector = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe Motion Detector.
		};

		// General
		/**
		* Initialize FaceMe Motion Detector.
		*
		* @param[in] config The configuration for motion detector setup.
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
		virtual FR_RETURN Initialize(const FR_MotionDetectorConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

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
		* Detect motion from image.
		*
		* @param[in] detectConfig Configuration for detection.
		* @param[in] image An image to be processed.
		* @param[out] isMoving Retrieve the result that if there is any motion be detected.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FALSE</code>: Detecting is in progress.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN Detect(const FR_MotionDetectConfig *detectConfig, const FR_Image *image, bool *isMoving) = 0;

		/**
		* Set sensitive level to compare with previous image if there is a motion.
		*
		* @param[in] sensitivity Set the sensitivity of the Motion Detector. The higher the value, the detector would be more sensitive to detect small movements. The value must between 1 and 100.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN SetSensitivity(uint32_t sensitivity) = 0;


		/**
		* Specify number of images for Motion Detector to check at a time.
		*
		* @param[in] threshold Specify number of images (which are continuous frames come from a single video camera) that Motion Detector should check at a time. The larger the number, the detector would more likely detect there's motion in the video. The minimum value is 1.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN SetNoMotionThreshold(uint32_t threshold) = 0;
	};
}

#endif // __FACEME_IFACEMEMOTIONDETECTOR_H__