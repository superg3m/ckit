#pragma once

#include <math.h>

#include "./ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Vector2 {
	double x;
	double y;
} CKIT_Vector2;

typedef struct CKIT_Vector3 {
	double x;
	double y;
	double z;
} CKIT_Vector3;

typedef struct CKIT_Vector4 {
	double x;
	double y;
	double z;
	double w;
} CKIT_Vector4;

typedef struct Mat2x2 {
	CKIT_Vector2 data[2];
} Mat2x2;

typedef struct Mat3x3 {
	CKIT_Vector3 data[3];
} Mat3x3;

typedef struct Mat4x4 {
	CKIT_Vector4 data[4];
} Mat4x4;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	int int_abs(int a);
	double float_abs(double a);
	double lerp(double a, double b, double t);
	CKIT_Vector2 CKIT_Vector2_lerp(CKIT_Vector2 a, CKIT_Vector2 b, double t);
	CKIT_Vector3 CKIT_Vector3_lerp(CKIT_Vector3 a, CKIT_Vector3 b, double t);
	CKIT_Vector4 CKIT_Vector4_lerp(CKIT_Vector4 a, CKIT_Vector4 b, double t);

	void CKIT_Vector2_print(CKIT_Vector2 point);
	CKIT_Vector2 CKIT_Vector2_spline_point(CKIT_Vector2* spline_points, u32 spline_points_count, double t);

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
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	#include "../Collections/ckit_vector.h"
	#include "./ckit_logger.h"

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

	// Eventually I want to do some physics here Acceleration, Velocity, Position

#endif // CKIT_IMPL