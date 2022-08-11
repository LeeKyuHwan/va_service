/**
* @file FaceMeVideoFrameRendererDef.h
* The private definition of video frame renderer for CyberLink Camera Recognizer.
* CyberLink FaceMe (R) SDK
* Copyright (c) 2020 CyberLink Corp. All rights reserved.
* https://www.cyberlink.com
*/
#pragma once
#ifndef	__FR_VIDEO_FRAME_RENDERER_DEF_H__
#define __FR_VIDEO_FRAME_RENDERER_DEF_H__
#include <stdint.h>
#include "FaceMeBaseDef.h"
#include "FaceMeVideoFrameDef.h"

namespace FaceMeSDK
{
	/**
	* Contains configuration of video frame allocator
	*
	* @struct FR_VideoFrameAllocatorConfig
	*/
	struct FR_VideoFrameRendererConfig
	{
		uint32_t sizeOfStructure;	//!< The size of the structure.
		uint32_t maxFrameWidth;		//!< Maximum width of each frame.
		uint32_t maxFrameHeight;	//!< Maximum height of each frame.
		uint32_t preferredGpuId;	//!< Preferred gpu device identifier.
		FR_VIDEO_FRAME_TYPE type;	//!< The format of frame.
	};

	/**
	* Contains configuration for video frame render
	*
	* @struct FR_VideoFrameRenderConfig
	*/
	struct FR_VideoFrameRenderConfig
	{
		uint32_t sizeOfStructure;	//!< The size of the structure.
		FR_Rectangle srcRoi;		//!< The source region of interest.
		uint32_t width;				//!< The target width of frame.
		uint32_t height;			//!< The target height of frame.
		uint32_t stride;			//!< The target stride of frame.
		FR_VIDEO_FRAME_TYPE type;		//!< The target type of frame.
		FR_VIDEO_FRAME_FORMAT format;	//!< The target format of frame.
	};

	/**
	* Contains status of video frame renderer
	*
	* @struct FR_VideoFrameRendererStatus
	*/
	struct FR_VideoFrameRendererStatus
	{
		uint32_t sizeOfStructure;		//!< The size of the structure.
		uint32_t gpuId;					//!< selected gpu device identifier.
		FR_VIDEO_FRAME_TYPE type;	//!< The format of frame.
	};
}

#endif //__FR_VIDEO_FRAME_RENDERER_DEF_H__
