#pragma once

#include "../ckit_types.h"
#include "../Basic/ckit_math.h"
//========================== Begin Types ==========================
typedef CKIT_Vector4 CKIT_Color;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	u32 ckit_color_to_u32(CKIT_Color color);
	CKIT_Color ckit_color_from_u32(u32 color);
	CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color);
	CKIT_Color ckit_color_u32_alpha_blend(u32 front_color_u32, u32 back_color_u32);

	/**
	 * @brief value from 0.0 to 1.0
	 * 
	 * @param color 
	 * @param value 
	 * @return CKIT_Color 
	 */
	CKIT_Color ckit_color_multiply(CKIT_Color color, float value);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define CKIT_COLOR_BLACK ((CKIT_Color){0, 0, 0, 255})
#define CKIT_COLOR_RED ((CKIT_Color){255, 0, 0, 255})
#define CKIT_COLOR_BLUE ((CKIT_Color){0, 0, 255, 255})
#define CKIT_COLOR_GREEN ((CKIT_Color){0, 255, 0, 255})
#define CKIT_COLOR_WHITE ((CKIT_Color){255, 255, 255, 255})
#define CKIT_COLOR_PINK ((CKIT_Color){255, 105, 180, 255})
#define CKIT_COLOR_LIME ((CKIT_Color){0, 255, 128, 255})
#define CKIT_COLOR_CYAN ((CKIT_Color){0, 255, 255, 255})
#define CKIT_COLOR_PURPLE ((CKIT_Color){128, 0, 128, 255})
#define CKIT_COLOR_YELLOW ((CKIT_Color){255, 255, 0, 255})
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++