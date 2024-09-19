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
	Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);

	/**
	 * @brief If there is no collision to get then return Zero Initalized CKIT_Rectangle2D 
	 * 
	 * @param rect1 
	 * @param rect2 
	 * @return CKIT_Rectangle2D 
	 */
	CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);
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

	Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2) {	
		if (rect1.x < rect2.x + rect2.width && rect1.x + rect1.width > rect2.x &&
			rect1.y < rect2.y + rect2.height && rect1.y + rect1.height > rect2.y) {
			return TRUE;
		}

		return FALSE;
	}


	CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2) {
		CKIT_Rectangle2D ret = {0};

		return ret;
	}
#endif // CKIT_IMPL