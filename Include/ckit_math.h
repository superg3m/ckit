#pragma once

#include <math.h>

#include "ckit_types.h"

typedef struct Vec2 {
	float x;
	float y;
} Vec2;

typedef struct Vec3 {
	float x;
	float y;
	float z;
} Vec3;

typedef struct Vec4 {
	float x;
	float y;
	float z;
	float w;
} Vec4;

typedef struct Mat2x2 {
	Vec2 data[2];
} Mat2x2;

typedef struct Mat3x3 {
	Vec3 data[3];
} Mat3x3;

typedef struct Mat4x4 {
	Vec4 data[4];
} Mat4x4;

int int_abs(int a);
double float_abs(double a);
float lerp(float a, float b, float t);
Vec2 vec2_lerp(Vec2 a, Vec2 b, float t);
Vec3 vec3_lerp(Vec3 a, Vec3 b, float t);
Vec4 vec4_lerp(Vec4 a, Vec4 b, float t);

Vec2 vec2_spline_point(Vec2* spline_points, u32 spline_points_count, float t);

Mat2x2 ckit_mat2x2_projection_mat();
Mat2x2 ckit_mat2x2_rotation_mat();
Mat2x2 ckit_mat2x2_transposition();
Mat2x2 ckit_mat2x2_mult();
Mat2x2 ckit_mat2x2_add();

Mat3x3 ckit_mat3x3_projection_mat();
Mat3x3 ckit_mat3x3_rotation_mat();
Mat3x3 ckit_mat3x3_transposition();
Mat3x3 ckit_mat3x3_mult();
Mat3x3 ckit_mat3x3_add();

Mat4x4 ckit_mat4x4_projection_mat();
Mat4x4 ckit_mat4x4_rotation_mat();
Mat4x4 ckit_mat4x4_transposition();
Mat4x4 ckit_mat4x4_mult();
Mat4x4 ckit_mat4x4_add();