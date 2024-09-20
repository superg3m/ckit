#pragma once

#include <math.h>

#include "../ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Vector2 {
	union {
		struct {
			double x;
			double y;
		};
		double v[2];
	};
} CKIT_Vector2;

typedef struct CKIT_Vector3 {
	union {
		struct {
			double x;
			double y;
			double z;
		};
		struct {
			double r;
			double g;
			double b;
		};
		double v[3];
	};
} CKIT_Vector3;

typedef struct CKIT_Vector4 {
	union {
		struct {
			double x;
			double y;
			double z;
			double w;
		};
		struct {
			double r;
			double g;
			double b;
			double a;
		};
		double v[4];
	};
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

typedef struct CKIT_Rectangle2D {
	CKIT_Vector2 position;
	u32 width;
	u32 height;
} CKIT_Rectangle2D;

typedef struct CKIT_Rectangle3D {
	CKIT_Vector3 position;
	u32 length;
	u32 width;
	u32 height;
} CKIT_Rectangle3D;

typedef struct CKIT_Circle2D {
	CKIT_Vector2 position;
	u32 radius;
} CKIT_Circle2D;

typedef struct CKIT_Circle3D {
	CKIT_Vector3 position;
	u32 radius;
} CKIT_Circle3D;
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

	CKIT_Rectangle2D ckit_rectangle2d_create(s32 x, s32 y, u32 width, u32 height);
	Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);
	CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);

	CKIT_Rectangle3D ckit_rectangle3d_create(s32 x, s32 y, s32 z, u32 length, u32 width, u32 height);

	CKIT_Circle2D ckit_circle2d_create(s32 x, s32 y, u32 radius);
	CKIT_Circle3D ckit_circle3d_create(s32 x, s32 y, s32 z, u32 radius);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************