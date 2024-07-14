#include "./ckit_math.h"
#include "../Collection/Vector/ckit_vector.h"
#include "../Logger/ckit_logger.h"

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

// Date: July 14, 2024
// TODO(Jovanni): Rewrite this because this is trash
Vec2 vec2_spline_point(Vec2* spline_points, u32 spline_points_size, float t) { 
	Vec2 vec_ret;
	if (spline_points_size <= 1) {
		// Date: May 26, 2024
		// TODO(Jovanni): This is a bad fix idk what to do here tbh?
		ckit_assert_msg(spline_points_size > 1, "Not enough spline points\n");
	}

	Vec2** lerp_points_vector = ckit_vector_reserve(4, Vec2*);

	u32 counter = spline_points_size;

	// I can't push back lerp_points right away because it grows thats insane lmao
	Vec2* lerp_points = NULLPTR;
	for (int i = 0; i < counter - 1; i++) {
		Vec2 lerp_point = vec2_lerp(spline_points[i+1], spline_points[i], t);
		LOG_DEBUG("(%f, %f) - (%f)(%f, %f) = (%f, %f)\n", spline_points[i + 1].x, spline_points[i + 1].y, t, spline_points[i].x, spline_points[i].y, lerp_point.x, lerp_point.y);
		ckit_vector_push(lerp_points, lerp_point);
	}
	ckit_vector_push(lerp_points_vector, lerp_points);
	LOG_PRINT("size vectors: %d\n", ckit_vector_count(lerp_points_vector));
	LOG_PRINT("size points: %d\n", ckit_vector_count(lerp_points));

	counter--;

	while (counter != 1) {
		lerp_points = NULLPTR;
		Vec2* current_vector = ckit_vector_pop(lerp_points_vector); // when you pop you need to free here basically

		LOG_INFO("==============\n");
		for (int i = 0; i < counter - 1; i++) {
			Vec2 lerp_point = vec2_lerp(current_vector[i+1], current_vector[i], t);
			LOG_DEBUG("(%f, %f) - (%f)(%f, %f) = (%f, %f)\n", current_vector[i + 1].x, current_vector[i + 1].y, t, current_vector[i].x, current_vector[i].y, lerp_point.x, lerp_point.y);
			ckit_vector_push(lerp_points, lerp_point);
		}
		LOG_INFO("==============\n");

		ckit_vector_push(lerp_points_vector, lerp_points);
		ckit_vector_free(current_vector);
		counter--;
	}

	Vec2* last_point_vector = ckit_vector_pop(lerp_points_vector);
	vec_ret = last_point_vector[0];
	ckit_vector_free(last_point_vector);

	for (int i = 0; i < ckit_vector_count(lerp_points_vector); i++) {
		ckit_vector_free(lerp_points_vector[i]);
	}
	ckit_vector_free(lerp_points_vector);

	return vec_ret;
}