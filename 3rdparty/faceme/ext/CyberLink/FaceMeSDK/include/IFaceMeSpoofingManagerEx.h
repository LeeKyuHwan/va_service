/**
* @file IFaceMeSpoofingManagerEx.h
* The extend interface of CyberLink Spoofing Manager.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FACEME_ISPOOFINGMANAGER_EXTEND_H__
#define __FACEME_ISPOOFINGMANAGER_EXTEND_H__
#include "IBaseObj.h"
#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
#define FR_SPOOFING_MANAGER_ID_GET_DEVICE_PATH_LEN	"SpoofingManager::GetDevicePathLen"		//!< value: (int32_t *)devicePathLen, args: index
#define FR_SPOOFING_MANAGER_ID_GET_DEVICE_PATH		"SpoofingManager::GetDevicePath"		//!< value: (const char *)devicePath, args: index
#define FR_SPOOFING_MANAGER_ID_SET_CALL_BACK		"SpoofingManager::SetCallBack"			//!< value: (IFaceMeSpoofingManagerEx *)pSpoofingManagerEx, args: nullptr
#define FR_SPOOFING_MANAGER_ID_DISABLE_REALSENSE_DEPTH_MODULE "SpoofingManager::DisableRealSenseDepthModule"  //!< Setting of disable real sense depth module.  value: (bool *), args: nullptr.
#define FR_SPOOFING_MANAGER_ID_ENABLE_HARDWARE "SpoofingManager::EnableHardware"  //!< Setting of enable gpu acceleration.  value: (bool *), args: nullptr.

	class IFaceMeSpoofingManagerEx
	{
	public:
		virtual ~IFaceMeSpoofingManagerEx() {}

		/**
		* Callback function for get Spoofing GetEndOfStream.
		*
		* @return Returns an <code>void</code>.
		*/
		virtual void GetEndOfStream() = 0;

		/**
		* Callback function for get Spoofing GetError.
		*
		* @param[out] result The result of error reason.
		* @return Returns an <code>void</code>.
		*/
		virtual void GetError(FR_RETURN result) = 0;
	};
}

#endif // __FACEME_ISPOOFINGMANAGER_EX_H__