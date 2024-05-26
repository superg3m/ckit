#include "../core/ckit_types.h"
#include "../core/ckit_vector.h"

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

