#pragma once

#include "../../Core/Basic/ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Color {
	u8 r;
	u8 g;
	u8 b;
	u8 a;
} CKIT_Color;

#define CKIT_COLOR_BLACK (CKIT_Color){0, 0, 0, 255}
#define CKIT_COLOR_RED (CKIT_Color){255, 0, 0, 255}
#define CKIT_COLOR_BLUE (CKIT_Color){0, 0, 255, 255}
#define CKIT_COLOR_GREEN (CKIT_Color){0, 255, 0, 255}
#define CKIT_COLOR_WHITE (CKIT_Color){255, 255, 255, 255}
#define CKIT_COLOR_PINK (CKIT_Color){255, 105, 180, 255}
#define CKIT_COLOR_LIME (CKIT_Color){0, 255, 128, 255}
#define CKIT_COLOR_CYAN (CKIT_Color){0, 255, 255, 255}
#define CKIT_COLOR_PURPLE (CKIT_Color){128, 0, 128, 255}
#define CKIT_COLOR_YELLOW (CKIT_Color){255, 255, 0, 255}

typedef struct Rectangle2D {
	s32 x;
	s32 y;
	u32 width;
	u32 height;
} Rectangle2D;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	Rectangle2D ckit_rectangle_create(s32 x, s32 y, u32 width, u32 height);
	Boolean ckit_rectangle_check_aabb_collision(Rectangle2D rect1, Rectangle2D rect2);

	/**
	 * @brief If there is no collision to get then return Zero Initalized Rectangle2D 
	 * 
	 * @param rect1 
	 * @param rect2 
	 * @return Rectangle2D 
	 */
	Rectangle2D ckit_rectangle_get_aabb_collision(Rectangle2D rect1, Rectangle2D rect2);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	Rectangle2D ckit_rectangle_create(s32 x, s32 y, u32 width, u32 height) {
		Rectangle2D ret = {0};
		ret.x = x;
		ret.y = y;
		ret.width = width;
		ret.height = height;

		return ret;
	}

	Boolean ckit_rectangle_check_aabb_collision(Rectangle2D rect1, Rectangle2D rect2) {	
		if (rect1.x < rect2.x + rect2.width && rect1.x + rect1.width > rect2.x &&
			rect1.y < rect2.y + rect2.height && rect1.y + rect1.height > rect2.y) {
			return TRUE;
		}

		return FALSE;
	}


	Rectangle2D ckit_rectangle_get_aabb_collision(Rectangle2D rect1, Rectangle2D rect2) {
		Rectangle2D ret = {0};

		return ret;
	}
#endif // CKIT_IMPL