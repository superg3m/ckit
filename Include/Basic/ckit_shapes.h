// Date: September 19, 2024
// TODO(Jovanni): THIS ENTIRE FILE SHOULD BE IN CORE

#pragma once

#include "../../Core/Basic/ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Rectangle2D {
	s32 x;
	s32 y;
	u32 width;
	u32 height;
} CKIT_Rectangle2D;

typedef struct CKIT_Rectangle3D {
	s32 x;
	s32 y;
	s32 z;
	u32 length;
	u32 width;
	u32 height;
} CKIT_Rectangle3D;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Rectangle2D ckit_rectangle_create(s32 x, s32 y, u32 width, u32 height);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
	CKIT_Rectangle2D ckit_rectangle_create(s32 x, s32 y, u32 width, u32 height) {
		CKIT_Rectangle2D ret = {0};
		ret.x = x;
		ret.y = y;
		ret.width = width;
		ret.height = height;

		return ret;
	}

#endif // CKIT_IMPL