#include "../../include/core/ckit_math.h"

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

Vec2 vec2_spline_point(Vec2* spline_points, u32 spline_points_size, float t) {
	Vec2 vec_ret;
	if (spline_points_size <= 1) {
		// Date: May 26, 2024
		// TODO(Jovanni): This is a bad fix idk what to do here tbh?
		assert_in_function(spline_points_size > 1, "Not enough spline points\n");
	}

	Vec2** lerp_points_vector = vector_create_vector_container(Vec2*);	

	u32 counter = spline_points_size;

	Vec2* lerp_points = vector_create(Vec2);
	vector_push(lerp_points_vector, lerp_points);
	for (int i = 0; i < counter - 1; i++) {
		Vec2 lerp_point = vec2_lerp(spline_points[i+1], spline_points[i], t);
		LOG_DEBUG("(%f, %f) - (%f)(%f, %f) = (%f, %f)\n", spline_points[i + 1].x, spline_points[i + 1].y, t, spline_points[i].x, spline_points[i].y, lerp_point.x, lerp_point.y);
		vector_push(lerp_points, lerp_point);
	}
	LOG_PRINT("size vectors: %d\n", vector_size(lerp_points_vector));
	LOG_PRINT("size points: %d\n", vector_size(lerp_points));

	counter--;


	while (counter != 1) {
		lerp_points = vector_create(Vec2);
		LOG_PRINT("size vectors: %d\n", vector_size(lerp_points_vector));
		Vec2* current_vector = lerp_points_vector[0];
		LOG_PRINT("size vectors: %d\n", vector_size(lerp_points_vector));
		LOG_WARN("(%f, %f)\n", current_vector[0].x, current_vector[0].y);
		LOG_PRINT("size points: %d\n", vector_size(current_vector));
		for (int i = 0; i < vector_size(current_vector) - 1; i++) {
			LOG_WARN("(%f, %f) - (%f)(%f, %f)\n", current_vector[i + 1].x, current_vector[i + 1].y, t, current_vector[i].x, current_vector[i].y);
		}
		return vec_ret;
		vector_push(lerp_points_vector, lerp_points);
		for (int i = 0; i < counter - 1; i++) {
			Vec2 lerp_point = vec2_lerp(current_vector[i+1], current_vector[i], t);
			LOG_DEBUG("(%f, %f) - (%f)(%f, %f) = (%f, %f)\n", current_vector[i + 1].x, current_vector[i + 1].y, t, current_vector[i].x, current_vector[i].y, lerp_point.x, lerp_point.y);
			vector_push(lerp_points, lerp_point);
		}
		counter--;
	}

	vec_ret = vector_pop(lerp_points_vector, Vec2*)[0];

	return vec_ret;
}