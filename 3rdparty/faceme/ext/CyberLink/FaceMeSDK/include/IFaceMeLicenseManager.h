/**
 * @file IFaceMeLicenseManager.h
 * The interface of CyberLink FaceMe License Manager.
 * CyberLink FaceMe (R) SDK
 * Copyright (c) 2020 CyberLink Corp. All rights reserved.
 * https://www.cyberlink.com
 */
#pragma once
#ifndef	__FACEME_IFACEMELICENSEMANAGER_H__
#define __FACEME_IFACEMELICENSEMANAGER_H__
#include "FaceMeLicenseManagerDef.h"
#include "IBaseObj.h"

namespace FaceMeSDK
{
	/**
	* CyberLink Face License Manager
	* @interface IFaceMeLicenseManager
	*/
	class IFaceMeLicenseManager : public BaseObject::IRefCountedObject
	{
	public:
		virtual ~IFaceMeLicenseManager() {}

		/**
		* Object identifiers of FaceMe License Manager.
		*
		* @enum FaceMeLicenseManagerOIDs
		*/
		enum FaceMeLicenseManagerOIDs
		{
			OID_FaceMeLicenseManager = (BaseObject::OID_RefCountedObject + 1) //!< Object identifier of FaceMe License Manager.
		};

		/**
		* The callback function for asynchronous operation.
		*
		* @param[in] hr The result code after completed.
		* @param[in] args The custom argument passed to the callback function.
		*/
		typedef void(*AsyncResultCallback)(FR_RETURN hr, void *args);

		/**
		* Initialize FaceMe License Manager.
		*
		* @param[in] config The configuration for license manager setup.
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
		virtual FR_RETURN Initialize(const FR_LicenseManagerConfig *config, const uint8_t* licenseInfo, uint32_t length) = 0;

		/**
		* Finalize FaceMe License Manager and release corresponding resources.
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
		* Finalize FaceMe License Manager and release corresponding resources.
		*
		* @param[in] callback The callback function for asynchronous operation. If the callback is nullptr, the function execute as synchronous operation.
		* @param[in] args The custom argument passed to the callback function.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_ASYNC</code>: Asynchronous operation.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_UNKNOWN_HOST</code>: Unknown license server error.
		*  * <code>FR_RETURN_NETWORK_ERROR</code>: Network error while communicating with the license server.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: Number of activated devices exceeded.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: Use configurations that are not in licensing list.
		*  * <code>FR_RETURN_LICENSE_SERVER_ERROR</code>: License server responds error. Please contact CyberLink to resolve the issue.
		*  * <code>FR_RETURN_LICENSE_INVALID_PACKAGE_NAME</code>: License is restricted with specific package name for Android, or bundleId for iOS.
		*  * <code>FR_RETURN_LICENSE_INCORRECT_KEY</code>: License key is incorrect.
		*  * <code>FR_RETURN_LICENSE_INCORRECT_PLATFORM</code>: License key is incorrect.
		*/
		virtual FR_RETURN RegisterLicense(AsyncResultCallback callback, void *args) = 0;
	};

	/**
	* CyberLink Face License Manager Extension
	* @interface IFaceMeLicenseManagerEx
	*/
	class IFaceMeLicenseManagerEx : public IFaceMeLicenseManager
	{
	public:
		virtual ~IFaceMeLicenseManagerEx() {}

		/**
		* Object identifiers of FaceMe License Manager Extension.
		*
		* @enum FaceMeLicenseManagerOIDsEx
		*/
		enum FaceMeLicenseManagerOIDsEx
		{
			OID_FaceMeLicenseManagerEx = (BaseObject::OID_RefCountedObject + 2) //!< Object identifier of FaceMe License Manager Extension.
		};

		/**
		* Renew License Key.
		*
		* @param[in] callback The callback function for asynchronous operation. If the callback is nullptr, the function execute as synchronous operation.
		* @param[in] args The custom argument passed to the callback function.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_ASYNC</code>: Asynchronous operation.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_NO_INIT</code>: Not initialized yet.
		*  * <code>FR_RETURN_FAIL</code>: Failed.
		*  * <code>FR_RETURN_UNKNOWN_HOST</code>: Unknown license server error.
		*  * <code>FR_RETURN_NETWORK_ERROR</code>: Network error while communicating with the license server.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: The license is invalid.
		*  * <code>FR_RETURN_LICENSE_EXPIRED</code>: The license is expired.
		*  * <code>FR_RETURN_LICENSE_ACTIVATION_EXCEEDED</code>: Number of activated devices exceeded.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: Use configurations that are not in licensing list.
		*  * <code>FR_RETURN_LICENSE_SERVER_ERROR</code>: License server responds error. Please contact CyberLink to resolve the issue.
		*/
		virtual FR_RETURN RenewLicense(AsyncResultCallback callback, void *args) = 0;

		/**
		* Deactivate License Key.
		*
		* @param[in] callback The callback function for asynchronous operation. If the callback is nullptr, the function execute as synchronous operation.
		* @param[in] args The custom argument passed to the callback function.
		* @return Returns an <code>FR_RETURN</code>. Possible values include, but
		* are not limited to, the following list.
		*  * <code>FR_RETURN_ASYNC</code>: Asynchronous operation.
		*  * <code>FR_RETURN_OK</code>: Succeeded.
		*  * <code>FR_RETURN_LICENSE_FAIL</code>: Incorrect License Key or Device not found.
		*  * <code>FR_RETURN_LICENSE_NOT_SUPPORT</code>: Cannot deactivate a subscription license.
		*/
		virtual FR_RETURN DeactivateLicense(AsyncResultCallback callback, void *args) = 0;
	};
}

#endif // __FACEME_IFACEMELICENSEMANAGER_H__