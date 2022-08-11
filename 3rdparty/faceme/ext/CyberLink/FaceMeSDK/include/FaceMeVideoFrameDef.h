/**
* @file FaceMeVideoFrameDef.h
* The private definition of video frame for CyberLink Camera Recognizer.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FR_VIDEO_FRAME_DEF_H__
#define __FR_VIDEO_FRAME_DEF_H__
#include <stdint.h>
#include "FaceMeBaseDef.h"

namespace FaceMeSDK
{
	/**
	* extra pixel format.
	*
	* @enum EFR_PIXEL_FORMAT_EXTRA
	*/
	enum EFR_PIXEL_FORMAT_EXTRA
	{
		FR_PIXEL_FORMAT_CL = 1000 //!< CPU/GPU CL Internal Frame pixel format.
	};

	/**
	* The type of video frame.
	*
	* @enum EFR_VIDEO_FRAME_TYPE
	*/
	enum EFR_VIDEO_FRAME_TYPE
	{
		FR_VIDEO_FRAME_TYPE_UNKNOWN = 0,	//!< Unknown memory type for video frame.
		FR_VIDEO_FRAME_TYPE_SYSTEM = 1,		//!< System memory for video frame.
		FR_VIDEO_FRAME_TYPE_GPU = 2			//!< The GPU memory for video frame.
	};


	/**
	* The type of video frame.
	*
	* @see EFR_VIDEO_FRAME_TYPE
	*/
	typedef int32_t FR_VIDEO_FRAME_TYPE;

	/**
	* The format of video frame.
	*
	* @enum EFR_VIDEO_FRAME_FORMAT
	*/
	enum EFR_VIDEO_FRAME_FORMAT
	{
		FR_VIDEO_FRAME_FORMAT_UNKNOWN = 0,			//!< Unknown video format.
		FR_VIDEO_FRAME_FORMAT_BGR24 = 1,			//!< BGR 24-bit video format.
		FR_VIDEO_FRAME_FORMAT_PLANAR_BGR24 = 2,		//!< Planar BGR 24-bit video format.
		FR_VIDEO_FRAME_FORMAT_RGB24 = 3,			//!< RGB 24-bit video format.
		FR_VIDEO_FRAME_FORMAT_PLANAR_RGB24 = 4,		//!< Planar RGB 24-bit video format.
		FR_VIDEO_FRAME_FORMAT_NV12 = 5,				//!< NV12 12-bit video format.
		FR_VIDEO_FRAME_FORMAT_RGBA = 6,			//!< Planar RGBA 32-bit video format.
		FR_VIDEO_FRAME_FORMAT_YUY2 = 7,			//!< YUY2 video format.
		FR_VIDEO_FRAME_FORMAT_BGRA = 8			//!< Planar BGRA 32-bit video format.
	};

	/**
	* The format of video frame.
	*
	* @see EFR_VIDEO_FRAME_FORMAT
	*/
	typedef int32_t FR_VIDEO_FRAME_FORMAT;

	/**
	* Contains information of video frame
	*
	* @struct FR_VideoFrameAllocateInfo
	*/
	struct FR_VideoFrameAllocateInfo
	{
		uint32_t sizeOfStructure;	//!< The size of the structure.
		uint32_t width;				//!< The width of frame.
		uint32_t height;			//!< The height of frame.
		uint32_t srcWidth;			//!< The source width of frame.
		uint32_t srcHeight;			//!< The source height of frame.
	};
}

#endif //__FR_VIDEO_FRAME_DEF_H__
