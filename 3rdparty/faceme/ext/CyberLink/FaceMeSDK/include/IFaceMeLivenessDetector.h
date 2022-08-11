/**
 * @file IFaceMeLivenessDetector.h
 * The definition for CyberLink FaceMe Liveness detection.
 *
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEME_LIVENESS_DETECTOR_H__
#define __FACEME_IFACEME_LIVENESS_DETECTOR_H__


#include "IBaseObj.h"
#include "FaceMeLivenessDetectorDef.h"

namespace FaceMeSDK
{
	/**
	* CyberLink FaceMe Liveness Detector
	* @interface IFaceMeLivenessDetector
	*/
	class IFaceMeLivenessDetector : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeLivenessDetector() {}

		/**
		* Object identifiers of FaceMe Liveness Detector.
		*
		* @enum FaceMeLivenessDetectorOIDs
		*/
		enum FaceMeLivenessDetectorOIDs
		{
			OID_FaceMeLivenessDetector = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe Liveness Detector.
		};

		// General
		/**
		* Initialize FaceMe anti-spoofing engine.
		*
		* @param[in] config The configuration for anti-spoofing engine.
		* @param[in] licenseInfo The license information provided by CyberLink. The format of license information is a raw buffer.
		* @param[in] length The length of the license information.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_CONSTRAIN_HARDWARE</code>: License hardware is constrained.
		*/
		virtual FR_RETURN Initialize(const FR_LivenessDetectorConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

		/**
		* Finalize FaceMe Anti-Spoofing engine and release corresponding resource.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: failed.
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
		* Detect faces' liveness in a frame
		*
		* @param[in] detectConfig Providing configuration to improve the detection.
		* @param[in] image The image to detect face liveness.
		* @param[in] depthMap The depth map to detect face liveness.
		* @param[in] boundingBox The faces' bounding box array to detect anti spoofing.
		* @param[in] numOfFaces The number of faces in the frame.
		* @param[out] livenessResults The liveness information array result for faces.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_CONSTRAIN_HARDWARE</code>: License hardware is constrained.
		*/
		virtual FR_RETURN Detect(const FR_LivenessDetectConfig *detectConfig,
			const FR_Image *image,
			const FR_DepthMap *depthMap,
			const FR_Rectangle *boundingBox,
			uint32_t numOfFaces,
			FR_FaceLivenessResult* livenessResults) = 0;

		/**
		* Retrieve current setting of face liveness detection.\n
		*
		* @param[out] detectionOption The options of extraction: minimum and maximum face width, speed and result order.
		* @param[out] value Retrieve the current setting of the extractionOption.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN GetDetectionOption(FR_LIVENESS_DETECTION_OPTION detectionOption, uint32_t *value) = 0;

		/**
		* Set prefer setting of face detection.
		*
		* @param[in] detectionOption The options of extraction: minimum and maximum face width, speed and result order.
		* @param[in] value The assigned setting of the detectOption.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*/
		virtual FR_RETURN SetDetectionOption(FR_LIVENESS_DETECTION_OPTION detectionOption, uint32_t value) = 0;
	};
}

#endif //__FACEME_ANTI_SPOOF_H__
