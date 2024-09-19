#include "./ckit_math.h"

Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);

/**
 * @brief If there is no collision to get then return Zero Initalized CKIT_Rectangle2D 
 * 
 * @param rect1 
 * @param rect2 
 * @return CKIT_Rectangle2D 
 */
CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);


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