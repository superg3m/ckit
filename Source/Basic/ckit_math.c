#pragma once

#include <math.h>

#include "../../Include/Basic/ckit_math.h"

#include "../../Include/Collections/ckit_vector.h"
#include "../../Include/Basic/ckit_logger.h"

int int_abs(int a) {
	return a < 0 ? (a * -1) : a;
}

double float_abs(double a) {
	return a < 0 ? (a * -1) : a;
}

double lerp(double a, double b, double t) {
	return b + ((a - b) * t);
}

CKIT_Vector2 CKIT_Vector2_lerp(CKIT_Vector2 a, CKIT_Vector2 b, double t) {
	CKIT_Vector2 vec_ret = {0};
	vec_ret.x = lerp(a.x, b.x, t); 
	vec_ret.y = lerp(a.y, b.y, t); 
	return vec_ret;
}

CKIT_Vector3 CKIT_Vector3_lerp(CKIT_Vector3 a, CKIT_Vector3 b, double t) {
	CKIT_Vector3 vec_ret = {0};
	vec_ret.x = lerp(a.x, b.x, t);
	vec_ret.y = lerp(a.y, b.y, t);
	vec_ret.z = lerp(a.z, b.z, t);

	return vec_ret;
}

CKIT_Vector4 CKIT_Vector4_lerp(CKIT_Vector4 a, CKIT_Vector4 b, double t) {
	CKIT_Vector4 vec_ret = {0};
	vec_ret.x = lerp(a.x, b.x, t);
	vec_ret.y = lerp(a.y, b.y, t);
	vec_ret.z = lerp(a.z, b.z, t);
	vec_ret.w = lerp(a.w, b.w, t);

	return vec_ret;
}


CKIT_Vector2 CKIT_Vector2_projection(CKIT_Vector2 a, CKIT_Vector2 b) {
	CKIT_Vector2 vec_ret = {0};
	(void)vec_ret;

	return vec_ret;
}

CKIT_Vector2 CKIT_Vector2_noramlize(CKIT_Vector2 a) {
	CKIT_Vector2 vec_ret = {0};
	(void)vec_ret;

	return vec_ret;
}

CKIT_Vector2 CKIT_Vector2_perpendicular(CKIT_Vector2 a) {
	CKIT_Vector2 vec_ret = {0};
	(void)vec_ret;

	return vec_ret;
}

void CKIT_Vector2_print(CKIT_Vector2 point) {
	LOG_PRINT("(%f, %f)\n", point.x, point.y);
}

CKIT_Vector2 CKIT_Vector2_spline_point(CKIT_Vector2* spline_points, u32 spline_points_count, double t) {
	if (spline_points_count == 1) {
		return spline_points[0];
	}

	CKIT_Vector2* points_vector = NULLPTR;
	for (u32 i = 0; i < spline_points_count - 1; i++) { // get lerp points
		CKIT_Vector2 lerp_point = CKIT_Vector2_lerp(spline_points[i + 1], spline_points[i], t);
		ckit_vector_push(points_vector, lerp_point);
	}

	CKIT_Vector2 ret = CKIT_Vector2_spline_point(points_vector, ckit_vector_count(points_vector), t); // feed back points
	ckit_vector_free(points_vector);
	return ret;
}

CKIT_Rectangle2D ckit_rectangle2d_create(s32 x, s32 y, u32 width, u32 height) {
	CKIT_Rectangle2D ret = {0};
	ret.position.x = x;
	ret.position.y = y;
	ret.width = width;
	ret.height = height;

	return ret;
}

CKIT_Rectangle3D ckit_rectangle3d_create(s32 x, s32 y, s32 z, u32 length, u32 width, u32 height) {
	CKIT_Rectangle3D ret = {0};
	ret.position.x = x;
	ret.position.y = y;
	ret.position.z = z;
	ret.length = length;
	ret.width = width;
	ret.height = height;

	return ret;
}

Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2) {
	if (rect1.position.x < rect2.position.x + rect2.width && rect1.position.x + rect1.width > rect2.position.x &&
		rect1.position.y < rect2.position.y + rect2.height && rect1.position.y + rect1.height > rect2.position.y) {
		return TRUE;
	}

	return FALSE;
}

CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2) {
	CKIT_Rectangle2D ret = {0};
	return ret;
}

CKIT_Circle2D ckit_circle2d_create(s32 x, s32 y, u32 radius) {
	CKIT_Circle2D ret = {0};
	ret.position.x = x;
	ret.position.y = y;
	ret.radius = radius;

	return ret;
}

CKIT_Circle3D ckit_circle3d_create(s32 x, s32 y, s32 z, u32 radius) {
	CKIT_Circle3D ret = {0};
	ret.position.x = x;
	ret.position.y = y;
	ret.position.z = z;
	ret.radius = radius;

	return ret;
}

// Eventually I want to do some physics here Acceleration, Velocity, Position