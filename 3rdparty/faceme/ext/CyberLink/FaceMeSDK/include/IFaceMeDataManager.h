/**
 * @file IFaceMeDataManager.h
 * The interface of CyberLink FaceMe Data Manager.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEMEDATAMANAGER_H__
#define __FACEME_IFACEMEDATAMANAGER_H__
#include "FaceMeDataManagerDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink Face Manager
	* @interface IFaceMeDataManager
	*/
	class IFaceMeDataManager : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeDataManager() {}

		/**
		* Object identifiers of FaceMe Manager.
		*
		* @enum FaceMeDataManagerOIDs
		*/
		enum FaceMeDataManagerOIDs
		{
			OID_FaceMeDataManager = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe Manager
		};

		// General
		/**
		* Initialize FaceMe Data Manager.
		*
		* @param[in] config The configuration for data manager setup.
		* @param[in] featurescheme Set the feature information and the False Acceptance Rate of levels.
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
		*  * <code>FR_RETURN_LICENSE_CONSTRAIN_HARDWARE</code>: License hardware is constrained.
		*/
		virtual FR_RETURN Initialize(const FR_DataManagerConfig *config, const FR_FaceFeatureScheme *featurescheme, const uint8_t* licenseInfo, uint32_t length) = 0;

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
		* Set precision level of recognition result.
		*
		* @param[in] level Represent the False Acceptance Rate level.
		* @param[out] confidenceThreshold Represents the confidence threshold of a specified precision level.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN GetPrecisionThreshold(FR_PRECISION_LEVEL level, float *confidenceThreshold) = 0;

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
		* @param[in] propertyId An utf-8 encoded string name of property identifier.
		* @param[out] value Retrieve the specified type of reference variant value by property identifier.
		* @param[out] args Retrieve the specified type of reference variant arguments by property identifier.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_INVALID_ARGUMENT</code>: The property identifier is not found.
		*/
		virtual FR_RETURN GetProperty(const char *propertyId, void *value, void *args) = 0;

		/**
		* Add a new face collection.
		*
		* @param[in] name An utf-8 encoded string name belongs to the adding face collection.
		* @param[out] collectionId Retrieve a new unique identifier represents the added face collection.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*/
		virtual FR_RETURN CreateFaceCollection(const char *name, int64_t *collectionId) = 0;

		/**
		* Delete a specified face collection.
		*
		* @param[in] collectionId An unique identifier represents a specified face collection.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN DeleteFaceCollection(int64_t collectionId) = 0;

		/**
		* Query face collections.
		*
		* @param[in] start Starting offset of face collection.
		* @param[out] collectionIds Retrieve a list of face collection identifiers.
		* @param[in, out] count Set the number of face collection identifiers wanted to retrieve. Represents the number of retrieved face collection identifiers after the function is called.
		* @param[out] next Next offset of face collection. Returns -1 when EOF is reached.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN QueryFaceCollection(int64_t start, int64_t *collectionIds, int32_t *count, int64_t *next) = 0;

		/**
		* Query face collections by name.
		*
		* @param[in] name An utf-8 encoded string name of face collection.
		* @param[in] start starting offset of face collection.
		* @param[out] collectionIds Retrieve a list of face collection identifiers.
		* @param[in, out] count Set the number of face collection identifiers wanted to retrieve. Represents the number of retrieved face collection identifiers after the function is called.
		* @param[out] next Next offset of face collection. Returns -1 when EOF is reached.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN QueryFaceCollectionByName(const char *name, int64_t start, int64_t *collectionIds, int32_t *count, int64_t *next) = 0;

		/**
		* Set custom data block to the face collection.
		*
		* @param[in] collectionId An unique identifier represents a specified face collection.
		* @param[in] customData The data block of user data.
		* @param[in] dataSize The length of data block of user data.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN SetFaceCollectionCustomData(int64_t collectionId, const uint8_t *customData, uint32_t dataSize) = 0;

		/**
		* Get user data block from the face collection.
		*
		* @param[in] collectionId An unique identifier represents a specified face collection.
		* @param[out] blob An object of data blob of user data. Caller should release this object after use.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN GetFaceCollectionCustomData(int64_t collectionId, BaseObject::IBlob **blob) = 0;

		/**
		* Get collection name from the face collection.
		*
		* @param[in] collectionId An unique identifier represents a specified face collection.
		* @param[out] name An utf-8 encoded string name object. Caller should release this object after use.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN GetFaceCollectionName(int64_t collectionId, BaseObject::ICharString **name) = 0;

		/**
		* Set the name of the face collection.
		*
		* @param[in] collectionId An unique identifier represents a specified face collection.
		* @param[in] name An utf-8 encoded string name of the collection.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN SetFaceCollectionName(int64_t collectionId, const char *name) = 0;

		/**
		* Search similar faces from a face collection.
		*
		* @param[in] confidenceThreshold A confidence threshold between 0.0 and 1.0.
		* @param[in] collectionId An unique identifier represents a specified face collection. Search from all face collections when the value is -1.
		* @param[in] faceFeature The feature vector of the face to be searched.
		* @param[out] candidateFaces Retrieve the list of similar faces.
		* @param[in, out] maxNumOfCandidates Set the maximum number of similar faces wanted to retrieved. Represents the number of retrieved face candidates after the function is called.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*  * <code>FR_RETURN_NOT_FOUND</code>: No face found.
		*/
		virtual FR_RETURN SearchSimilarFace(float confidenceThreshold, int64_t collectionId, const FR_FaceFeature *faceFeature,
			FR_SimilarFaceResult *candidateFaces, int32_t *maxNumOfCandidates) = 0;

		/**
		* Add a new face to the face collection.
		*
		* @param[in] collectionId An unique identifier represents a specified face collection.
		* @param[in] faceFeature The face feature of the new face.
		* @param[out] faceId Retrieve a new unique identifier represents the added face.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN AddFace(int64_t collectionId, const FR_FaceFeature *faceFeature, int64_t *faceId) = 0;

		/**
		* Delete a specified face.
		*
		* @param[in] faceId An unique identifier represents a specified face.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN DeleteFace(int64_t faceId) = 0;

		/**
		* Query faces from a face collection.
		*
		* @param[in] collectionId An unique identifier represents a specified collection.
		* @param[in] start starting offset of face.
		* @param[out] faceIds Retrieve a list of face identifiers.
		* @param[in, out] count Set the number of face identifiers wanted to retrieve. Represents the number of retrieved face identifiers after the function is called.
		* @param[out] next Next offset of face. Returns -1 when EOF is reached.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN QueryFace(int64_t collectionId, int64_t start, int64_t *faceIds, int32_t *count, int64_t *next) = 0;

		/**
		* Get the face collection from  a face.
		*
		* @param[in] faceId An unique identifier represents a specified face.
		* @param[out] collectionId  Retrieve a face collection identifier associate with the speciftied face.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN GetFaceCollectionFromFace(int64_t faceId, int64_t *collectionId) = 0;

		/**
		* Set custom data block to the face.
		*
		* @param[in] faceId An unique identifier represents a specified face.
		* @param[in] customData The data block of custom data.
		* @param[in] dataSize The length of data block of user data.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN SetFaceCustomData(int64_t faceId, const uint8_t *customData, uint32_t dataSize) = 0;

		/**
		* Get custom data block from the face.
		*
		* @param[in] faceId An unique identifier represents a specified face.
		* @param[out] blob An object of data blob of custom data. Caller should release this object after use.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN GetFaceCustomData(int64_t faceId, BaseObject::IBlob **blob) = 0;

		/**
		* Get a feature vector from a face.
		*
		* @param[in] faceId An unique identifier represents a specified face.
		* @param[out] faceFeature Retrieve the feature vector of the specified face.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_INDEX_OUT_OF_RANGE</code>: The index is out of range.
		*/
		virtual FR_RETURN GetFaceFeature(int64_t faceId, FR_FaceFeature *faceFeature) = 0;
	};
}

#endif // __FACEME_IFACEMEDATAMANAGER_H__