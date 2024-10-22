#pragma once 

#if defined(CKIT_EXTERN)
    #define CKIT_API extern
#else
    #define CKIT_API
#endif

#if defined(CKIT_IMPL_CPP)
    #define CKIT_IMPL_TYPES
    #define CKIT_IMPL_LOGGER
    #define CKIT_IMPL_ASSERT
    #define CKIT_IMPL_MEMORY
    #define CKIT_IMPL_ARENA
    #define CKIT_IMPL_STRING
	#define CKIT_IMPL_MATH
    #define CKIT_IMPL_CHAR
    #define CKIT_IMPL_COLLECTIONS
    #define CKIT_IMPL_OS
	#define CKIT_IMPL_UTILITIES
#endif


#define CKIT_INCLUDE_TYPES
#if defined(CKIT_INCLUDE_TYPES)
    #include <stdint.h>
    #include <stdio.h>
    #include <stdarg.h>
    #include <stdlib.h>
	#include <iostream>
	#include <math.h>

    typedef int8_t  s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef u8 Boolean;

    #define FALSE 0
    #define TRUE 1
    #define NULLPTR 0
    #define PI 3.14159265359

    #define stringify(entry) #entry
    #define glue(a, b) a##b

    #define KiloBytes(value) ((u64)(value) * 1024L)
    #define MegaBytes(value) ((u64)KiloBytes(value) * 1024L)
    #define GigaBytes(value) ((u64)MegaBytes(value) * 1024L)

    #define MIN(a, b) (((a) < (b)) ? (a) : (b))
    #define MAX(a, b) (((a) > (b)) ? (a) : (b))
    #define CLAMP(value, min_value, max_value) (MIN(MAX(value, min_value), max_value))
	#define SQUARED(x) ((x) * (x))

    #define local_persist static
    #define internal static

    // Date: July 12, 2024
    // TODO(Jovanni): Test this to make sure its actually works but it makes sense to me
    #define OFFSET_OF(type, member) sizeof((u64)(&(((type*)0)->member)))

    #define FIRST_DIGIT(number) ((int)number % 10);

    CKIT_API void U32_EndianSwap(u32* number_to_endian_swap);
    CKIT_API void U64_EndianSwap(u64* number_to_endian_swap);
    #define GET_BIT(number, bit_to_check) ((number & (1 << bit_to_check)) >> bit_to_check)
    #define SET_BIT(number, bit_to_set) number |= (1 << bit_to_set);
    #define UNSET_BIT(number, bit_to_unset) number &= (~(1 << bit_to_unset));

    #define ArrayCount(array) (sizeof(array) / sizeof(array[0]))

    #define PLATFORM_MAX_PATH 256

    #if defined(_WIN32)
        #include <windows.h>
        #define PLATFORM_WINDOWS
        #define OS_DELIMITER '\\'
        #define CRASH __debugbreak()
    #elif defined(__APPLE__)
        #define PLATFORM_APPLE
        #define OS_DELIMITER '/'
        #define CRASH __builtin_trap()
    #elif defined(__linux__) || defined(__unix__) || defined(__POSIX__)
        #define PLATFORM_LINUX
        #define OS_DELIMITER '/'
        #define CRASH __builtin_trap()
    #else
        #error "Unknown Platform???"
    #endif

    CKIT_API void STACK_TRACE_DUMP();
#endif

/*
template<bool Condition, typename TrueType>
struct conditional_impl;

template<typename TrueType>
struct conditional_impl<true, TrueType> {
    using type = TrueType;
};

template<bool Condition, typename TrueType>
using conditional_t = typename conditional_impl<Condition, TrueType>::type;
*/

namespace ckit {
    struct Vector2D {
		union {
			struct {
				float x;
				float y;
			};

			float v[2];
		};

		Vector2D(float value = 0.0f);
		Vector2D(float x_value, float y_value);

		Vector2D& operator+=(const Vector2D& v);
		Vector2D& operator-=(const Vector2D& v);
		Vector2D& operator*=(const Vector2D& v);
		Vector2D& operator/=(const Vector2D& v);

		Vector2D operator+(const Vector2D& v) const;
		Vector2D operator-(const Vector2D& v) const;
		Vector2D operator*(const Vector2D& v) const;
		Vector2D operator/(const Vector2D& v) const;

		float getMagnitude() const;
		void normalize();
	};

	struct Vector3D {
		union {
			struct {
				float x;
				float y;
				float z;
			};
			
			float v[3];
		};

		Vector3D(float value = 0.0f);
		Vector3D(float x_value, float y_value, float z_value);

		Vector3D& operator+=(const Vector3D& v);
		Vector3D& operator-=(const Vector3D& v);
		Vector3D& operator*=(const Vector3D& v);
		Vector3D& operator/=(const Vector3D& v);

		Vector3D operator+(const Vector3D& v) const;
		Vector3D operator-(const Vector3D& v) const;
		Vector3D operator*(const Vector3D& v) const;
		Vector3D operator/(const Vector3D& v) const;

		float getMagnitude() const;
		void normalize();
	};

	struct Vector4D {
		union {
			struct {
				float x;
				float y;
				float z;
				float w;
			};
			
			float v[4];
		};

		Vector4D(float value = 0.0f);
		Vector4D(float x_value, float y_value, float z_value, float w_value);

		Vector4D& operator+=(const Vector4D& v);
		Vector4D& operator-=(const Vector4D& v);
		Vector4D& operator*=(const Vector4D& v);
		Vector4D& operator/=(const Vector4D& v);

		Vector4D operator+(const Vector4D& v) const;
		Vector4D operator-(const Vector4D& v) const;
		Vector4D operator*(const Vector4D& v) const;
		Vector4D operator/(const Vector4D& v) const;

		float getMagnitude() const;
		void normalize();
	};

	//
	// ===================================================== MATRIX =====================================================
	//
}

//
// ===================================================== CKIT_IMPL =====================================================
//

namespace ckit {
	#if defined(CKIT_IMPL_MATH)
		Vector2D::Vector2D(float value) {
			this->x = value;
			this->y = value;
		}

		Vector2D::Vector2D(float x, float y) {
			this->x = x;
			this->y = y;
		}

		Vector2D& Vector2D::operator+=(const Vector2D& v) {
			this->x += v.x;
			this->y += v.y;

			return *this;
		}

		Vector2D& Vector2D::operator-=(const Vector2D& v) {
			this->x -= v.x;
			this->y -= v.y;

			return *this;
		}

		Vector2D& Vector2D::operator*=(const Vector2D& v) {
			this->x *= v.x;
			this->y *= v.y;

			return *this;
		}

		Vector2D& Vector2D::operator/=(const Vector2D& v) {
			this->x /= v.x;
			this->y /= v.y;

			return *this;
		}

		Vector2D Vector2D::operator+(const Vector2D& v) const {
			float new_x = this->x + v.x;
			float new_y = this->y + v.y;

			return Vector2D(new_x, new_y);
		}

		Vector2D Vector2D::operator-(const Vector2D& v) const {
			float new_x = this->x - v.x;
			float new_y = this->y - v.y;

			return Vector2D(new_x, new_y);
		}

		Vector2D Vector2D::operator*(const Vector2D& v) const {
			float new_x = this->x * v.x;
			float new_y = this->y * v.y;

			return Vector2D(new_x, new_y);
		}

		Vector2D Vector2D::operator/(const Vector2D& v) const {
			float new_x = this->x / v.x;
			float new_y = this->y / v.y;

			return Vector2D(new_x, new_y);
		}

		float Vector2D::getMagnitude() const {
			return sqrtf(SQUARED(this->x) + SQUARED(this->y));
		}

		void Vector2D::normalize() {
			float magnitude = this->getMagnitude();
			if (magnitude != 0.0f) {
				this->x /= magnitude;
				this->y /= magnitude;
			}
		}

		// -------------------- Vector3D Implementation --------------------

		Vector3D::Vector3D(float value) {
			this->x = value;
			this->y = value;
			this->z = value;
		}

		Vector3D::Vector3D(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3D& Vector3D::operator+=(const Vector3D& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;

			return *this;
		}

		Vector3D& Vector3D::operator-=(const Vector3D& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;

			return *this;
		}

		Vector3D& Vector3D::operator*=(const Vector3D& v) {
			this->x *= v.x;
			this->y *= v.y;
			this->z *= v.z;

			return *this;
		}

		Vector3D& Vector3D::operator/=(const Vector3D& v) {
			this->x /= v.x;
			this->y /= v.y;
			this->z /= v.z;

			return *this;
		}

		Vector3D Vector3D::operator+(const Vector3D& v) const {
			float new_x = this->x + v.x;
			float new_y = this->y + v.y;
			float new_z = this->z + v.z;

			return Vector3D(new_x, new_y, new_z);
		}

		Vector3D Vector3D::operator-(const Vector3D& v) const {
			float new_x = this->x - v.x;
			float new_y = this->y - v.y;
			float new_z = this->z - v.z;

			return Vector3D(new_x, new_y, new_z);
		}

		Vector3D Vector3D::operator*(const Vector3D& v) const {
			float new_x = this->x * v.x;
			float new_y = this->y * v.y;
			float new_z = this->z * v.z;

			return Vector3D(new_x, new_y, new_z);
		}

		Vector3D Vector3D::operator/(const Vector3D& v) const {
			float new_x = this->x / v.x;
			float new_y = this->y / v.y;
			float new_z = this->z / v.z;

			return Vector3D(new_x, new_y, new_z);
		}

		float Vector3D::getMagnitude() const {
			return sqrtf(SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z));
		}

		void Vector3D::normalize() {
			float magnitude = this->getMagnitude();
			if (magnitude != 0.0f) {
				this->x /= magnitude;
				this->y /= magnitude;
				this->z /= magnitude;
			}
		}

		// -------------------- Vector4D Implementation --------------------

		Vector4D::Vector4D(float value) {
			this->x = value;
			this->y = value;
			this->z = value;
			this->w = value;
		}

		Vector4D::Vector4D(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vector4D& Vector4D::operator+=(const Vector4D& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			this->w += v.w;

			return *this;
		}

		Vector4D& Vector4D::operator-=(const Vector4D& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			this->w-= v.w;

			return *this;
		}

		Vector4D& Vector4D::operator*=(const Vector4D& v) {
			this->x *= v.x;
			this->y *= v.y;
			this->z *= v.z;
			this->w *= v.w;

			return *this;
		}

		Vector4D& Vector4D::operator/=(const Vector4D& v) {
			this->x /= v.x;
			this->y /= v.y;
			this->z /= v.z;
			this->w /= v.w;

			return *this;
		}

		Vector4D Vector4D::operator+(const Vector4D& v) const {
			float new_x = this->x + v.x;
			float new_y = this->y + v.y;
			float new_z = this->z + v.z;
			float new_w = this->w + v.w;

			return Vector4D(new_x, new_y, new_z, new_w);
		}

		Vector4D Vector4D::operator-(const Vector4D& v) const {
			float new_x = this->x - v.x;
			float new_y = this->y - v.y;
			float new_z = this->z - v.z;
			float new_w = this->w - v.w;

			return Vector4D(new_x, new_y, new_z, new_w);
		}

		Vector4D Vector4D::operator*(const Vector4D& v) const {
			float new_x = this->x * v.x;
			float new_y = this->y * v.y;
			float new_z = this->z * v.z;
			float new_w = this->w * v.w;

			return Vector4D(new_x, new_y, new_z, new_w);
		}

		Vector4D Vector4D::operator/(const Vector4D& v) const {
			float new_x = this->x / v.x;
			float new_y = this->y / v.y;
			float new_z = this->z / v.z;
			float new_w = this->w / v.w;

			return Vector4D(new_x, new_y, new_z, new_w);
		}

		float Vector4D::getMagnitude() const {
			return sqrtf(SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z) + SQUARED(this->w));
		}

		void Vector4D::normalize() {
			float magnitude = this->getMagnitude();
			if (magnitude != 0.0f) {
				this->x /= magnitude;
				this->y /= magnitude;
				this->z /= magnitude;
				this->w /= magnitude;
			}
		}
	#endif
}


// Dynamic Arrays/Vector
// HashMap
// LinkedList
// Memory Ops
// Strings
// Math Ops/Structures
// Vec2, Vec3, Vec4
// Mat2, Mat3, Mat4