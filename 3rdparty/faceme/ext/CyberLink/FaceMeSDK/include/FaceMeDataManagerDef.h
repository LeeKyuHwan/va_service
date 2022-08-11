/**
* @file FaceMeDataManagerDef.h
* The definition for CyberLink FaceMe Face Manager.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_DATA_MANAGER_DEF_H__
#define __FACEME_DATA_MANAGER_DEF_H__

#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
#define FR_DATAMANAGER_PROPERTY_ID_SET_UPDATE_INTERVAL "FaceDataManager::SetUpdateInterval"	//!< value: (int32_t *)&msLength, args: nullptr 
#define FR_DATAMANAGER_PROPERTY_ID_UPDATE "FaceDataManager::Update"	//!< value: nullptr, args: nullptr 
#define FR_DATAMANAGER_PROPERTY_ID_SET_MASKED_FEATURE_ENROLLMENT "FaceDataManager::SetMaskedFeatureEnrollment"//!< value: (bool *)&enableMaskedFeatureEnrollment, args: nullptr

	/**
	* Contains Data Manager Configuration.
	*
	* @struct FR_DataManagerConfig
	*/
	struct FR_DataManagerConfig
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		const char *appBundlePath;		//!< The application bundle folder path, represented by an utf-8 encoded string.
		const char *appDataPath;		//!< The application folder path to be used for data saving, represented by an utf-8 encoded string.
		const char *appCachePath;		//!< The application folder path to be used for cache saving, represented by an utf-8 encoded string.

		//!< The application config to open data base. It depends on to the database type. 
		// For example: "Driver={SQL Server};Server=192.168.x.x;Database=test;Uid=faceme;Pwd=12345678;" for FaceMeODBCDataManager to connect to MS SQL.
		//               To use FaceMeODBCDataManager in different SQL server, you may refer to https://docs.microsoft.com/en-us/sql/integration-services/import-export-data/connect-to-an-odbc-data-source-sql-server-import-and-export-wizard
		const char *connectConfig;
	};

	/**
	* Contains Similar Face result.
	*
	* @struct FR_SimilarFaceResult
	*/
	struct FR_SimilarFaceResult
	{
		uint32_t sizeOfStructure;	//!< The size of the structure.
		int64_t collectionId;		//!< The unique identifier of the collection.
		int64_t faceId;				//!< The unique identifier of the face.
		float confidence;			//!< The confidence between 0.0 and 1.0 of the face.
	};
}

#endif //__FACEME_DATA_MANAGER_DEF_H__
