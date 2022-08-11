/**
 * @file IFaceMeCameraRecognizer.h
 * The interface of CyberLink Camera Recognizer.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FR_IFACEMECAMERARECOGNIZER_H__
#define __FR_IFACEMECAMERARECOGNIZER_H__
#include "FaceMeCameraRecognizerDef.h"
#include "IFaceMeCameraManager.h"
#include "IFaceMeVideoFrameRenderer.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink Camera Recognizer Handler
	* @interface IFaceMeCameraRecognizerHandler
	*
	* @note User should implement handler by derived it.
	*/
	class IFaceMeCameraRecognizerHandler
	{
	public:
		virtual ~IFaceMeCameraRecognizerHandler() {}

		/**
		* Notify when the Face Detect is compeleted.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result The result of face detection.
		* @param[in,out] nextActions To indicate the next actions after face detection.
		*/
		virtual void OnFaceDetectCompleted(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, const FR_FaceDetectResult *result, FR_FaceDetectNextActions *nextActions) = 0;

		/**
		* Notify when the Face Detect Error is occurred.
		*
		* @param[in] result an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual void OnFaceDetectError(FR_RETURN result) = 0;

		/**
		* Notify when the Face Extract is compeleted.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result The result of face extraction.
		* @param[in,out] nextActions To indicate the next actions after face extraction.
		*/
		virtual void OnFaceExtractCompleted(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, const FR_FaceExtractResult *result, FR_FaceExtractNextActions *nextActions) = 0;

		/**
		* On Face Extract Error is occurred.
		*
		* @param[in] result an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual void OnFaceExtractError(FR_RETURN result) = 0;

		/**
		* Notify when the Face Images extract is compeleted.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result The result of face image extraction.
		*/
		virtual void OnFaceImagesExtractCompleted(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, const FR_FaceImageResult *result) = 0;

		/**
		* On Face Image Extract Error is occurred.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual void OnFaceImagesExtractError(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, FR_RETURN result) = 0;

		/**
		* Notify when the Images extract is compeleted.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result The result of frame image extraction.
		*/
		virtual void OnImageExtractCompleted(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, const FR_Image *result) = 0;

		/**
		* On Image Extract Error is occurred.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual void OnImageExtractError(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, FR_RETURN result) = 0;

		/**
		* Notify when the face quality check is compeleted.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result The result of face image quality checking.
		* @param[in,out] nextActions To indicate the next actions after face image quality checking.
		*/
		virtual void OnFaceQualityCheckCompleted(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, const FR_FaceQualityCheckResult *result, FR_FaceQualityCheckNextActions *nextActions) = 0;

		/**
		* On face quality check Error is occurred.
		*
		* @param[in] cameraId camera Identifier.
		* @param[in] frameInfo The video frame information.
		* @param[in] result an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual void OnFaceQualityCheckError(uint64_t cameraId, const FR_VideoFrameInfo *frameInfo, FR_RETURN result) = 0;
	};

	/**
	* CyberLink Camera Recognizer
	* @interface IFaceMeCameraRecognizer
	*/
	class IFaceMeCameraRecognizer : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeCameraRecognizer() {}

		/**
		* Object identifiers of Camera Recognizer.
		*
		* @enum FaceMeCameraRecognizerOIDs
		*/
		enum FaceMeCameraRecognizerOIDs
		{
			OID_FaceMeCameraRecognizer = (BaseObject::OID_RefCountedObject + 1)	//!< Object identifier of Camera Recognizer.
		};

		// General
		/**
		* Initialize FaceMe Camera Recognizer.
		*
		* @param[in] config The configuration for Camera Recognizer.
		* @param[in] handlerType The type of handler interface.
		* @param[in] handler The caller implemented interface of specified handler. Caller should keep life-cycle of the handler implemented by caller.
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
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		*/
		virtual FR_RETURN Initialize(const FR_CameraRecognizerConfig *config,
			FR_CAMERA_RECOGNIZER_HANDLER_TYPE handlerType, void *handler,
			const uint8_t* licenseInfo, uint32_t length) = 0;

		/**
		* Finalize FaceMe Camera Recognizer and release corresponding resource.
		*
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: failed.
		* @note All of the created IFaceMeCameraCapturer and IFaceMeCameraEnumerator interface should be released before calling Finalize() API or release.
		*/
		virtual FR_RETURN Finalize() = 0;

		/**
		* Retrieve feature scheme and model threshold values.
		*
		* @param[in] type The specified type of feature scheme.
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
		virtual FR_RETURN GetFeatureScheme(FR_CAMERA_RECOGNIZER_FEATURE_SCHEME_TYPE type, FR_FaceFeatureScheme *featureScheme) = 0;

		/**
		* Create Camera Capturer by device identifier.
		*
		* @param[in] deviceId device Identifier.
		* @param[in] config The configuration for capturer setup.
		* @param[out] capturer Retrieve a new instance of IFaceMeCameraCapturer. Caller should release this object when not used.
		* @param[out] cameraId Retrieve a new identifier of camera capturer.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: The license number of camera exceeded the usage limitation.
		* @note All of the created IFaceMeCameraCapturer interface should be release before calling Finalize() API or Release.
		*/
		virtual FR_RETURN CreateCameraCapturer(int32_t deviceId, const FR_CameraCapturerConfig *config, IFaceMeCameraCapturer **capturer, uint64_t *cameraId) = 0;

		/**
		* Create Camera Capturer by device path.
		*
		* @param[in] devicePath utf-8 encoded string of device path or url of IPCam.
		* @param[in] config The configuration for capturer setup.
		* @param[out] capturer Retrieve a new instance of IFaceMeCamCapturer. Caller should release this object when not used.
		* @param[out] cameraId Retrieve a new identifier of camera capturer.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: The license number of camera exceeded the usage limitation.
		* @note All of the created IFaceMeCameraCapturer interface should be release before calling Finalize() API or release.
		*/
		virtual FR_RETURN CreateCameraCapturer(const char *devicePath, const FR_CameraCapturerConfig *config, IFaceMeCameraCapturer **capturer, uint64_t *cameraId) = 0;

		/**
		* Create camera enumerator.
		*
		* @param[in] config The configuration forcamera enumerator setup.
		* @param[out] enumerator Retrieve a new instance of IFaceMeCameraEnumerator. Caller should release this object when not used.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		* @note All of the created IFaceMeCameraEnumerator interface should be release before calling Finalize() API or release.
		*/
		virtual FR_RETURN CreateCameraEnumerator(const FR_CameraEnumeratorConfig *config, IFaceMeCameraEnumerator **enumerator) = 0;

		/**
		* Create Video Frame Renderer.
		*
		* @param[out] renderer Retrieve a new instance of IFaceMeVideoFrameRenderer. Caller should release this object when not used.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The input argument is invalid.
		*  * <code>FR_RETURN_INCONSISTENT_VERSION</code>: The version of input data structure is not accepted or not filled in.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: License is not support.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: The license number of camera exceeded the usage limitation.
		* @note All of the created IFaceMeCameraCapturer interface should be release before calling Finalize() API or release.
		*/
		virtual FR_RETURN CreateVideoFrameRenderer(IFaceMeVideoFrameRenderer **renderer) = 0;

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
}

#endif // __FR_IFACEMECAMERARECOGNIZER_H__