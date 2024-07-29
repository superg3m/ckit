#include "ckit_math.h"
#include "ckit_vector.h"
#include "ckit_logger.h"

int int_abs(int a) {
	return a < 0 ? (a * -1) : a;
}

double float_abs(double a) {
	return a < 0 ? (a * -1) : a;
}

float lerp(float a, float b, float t) {
	return b + ((a - b) * t);
}

Vec2 vec2_lerp(Vec2 a, Vec2 b, float t) {
	Vec2 vec_ret;
	vec_ret.x = lerp(a.x, b.x, t); 
	vec_ret.y = lerp(a.y, b.y, t); 
	return vec_ret;
}

Vec3 vec3_lerp(Vec3 a, Vec3 b, float t) {
	Vec3 vec_ret;
	vec_ret.x = lerp(a.x, b.x, t);
	vec_ret.y = lerp(a.y, b.y, t);
	vec_ret.z = lerp(a.z, b.z, t);

	return vec_ret;
}

Vec4 vec4_lerp(Vec4 a, Vec4 b, float t) {
	Vec4 vec_ret;
	vec_ret.x = lerp(a.x, b.x, t);
	vec_ret.y = lerp(a.y, b.y, t);
	vec_ret.z = lerp(a.z, b.z, t);
	vec_ret.w = lerp(a.w, b.w, t);

	return vec_ret;
}

void vec2_print(Vec2 point) {
	LOG_PRINT("(%f, %f)\n", point.x, point.y);
}

Vec2 vec2_spline_point(Vec2* spline_points, u32 spline_points_count, float t) {
	if (spline_points_count == 1) {
		return spline_points[0];
	}

	Vec2* points_vector = NULLPTR;
	for (int i = 0; i < spline_points_count - 1; i++) { // get lerp points
		Vec2 lerp_point = vec2_lerp(spline_points[i + 1], spline_points[i], t);
		ckit_vector_push(points_vector, lerp_point);
	}

	Vec2 ret = vec2_spline_point(points_vector, ckit_vector_count(points_vector), t); // feed back points
	ckit_vector_free(points_vector);
	return ret;
}