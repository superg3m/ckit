#include "../include/core/ckit_math.h"

int int_abs(int a) {
	return a < 0 ? (a * -1) : a;
}

double float_abs(double a) {
	return a < 0 ? (a * -1) : a;
}

float lerp(float a, float b, float t) {
	return a + ((a - b) * t);
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
		assert_in_function(spline_points_size > 1, "Not enough spline points");
	}
	Vec2** lerp_points_vector = vector_create(Vec2*);

	vector_push(lerp_points_vector, vector_create(Vec2));

	// create new vector for each set of lerp pointers 
	// then pop that will be the spline point you want



	for (int i = 0; i < spline_points_size - 1; i++) {
		Vec2 lerp_point = vec2_lerp(spline_points[i], spline_points[i+1], t);
		vector_push(lerp_points, lerp_point);
	}

	for (int i = 0; i < vector_size(lerp_points); i++) {
		Vec2 lerp_point = vec2_lerp(spline_points[i], spline_points[i+1], t);
		vector_push(lerp_points, lerp_point);
	}



}