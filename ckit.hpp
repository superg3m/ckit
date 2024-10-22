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
    struct Vector2 {
		union {
			struct {
				float x;
				float y;
			};

			float v[2];
		};

		Vector2(float value = 0.0f);
		Vector2(float x_value, float y_value);

		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(const Vector2& v);
		Vector2& operator/=(const Vector2& v);

		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2 operator*(const Vector2& v) const;
		Vector2 operator/(const Vector2& v) const;

		float getMagnitude() const;
		float getMagnitudeSquared() const;
		float getAngleFromOrigin() const;
		float getAngleFromTarget(const Vector2& v2) const;
		void normalize();
	};

	struct Vector3 {
		union {
			struct {
				float x;
				float y;
				float z;
			};
			
			float v[3];
		};

		Vector3(float value = 0.0f);
		Vector3(float x_value, float y_value, float z_value);

		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(const Vector3& v);
		Vector3& operator/=(const Vector3& v);

		Vector3 operator+(const Vector3& v) const;
		Vector3 operator-(const Vector3& v) const;
		Vector3 operator*(const Vector3& v) const;
		Vector3 operator/(const Vector3& v) const;

		float getMagnitude() const;
		float getMagnitudeSquared() const;
		void normalize();
	};

	struct Vector4 {
		union {
			struct {
				float x;
				float y;
				float z;
				float w;
			};
			
			float v[4];
		};

		Vector4(float value = 0.0f);
		Vector4(float x_value, float y_value, float z_value, float w_value);

		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(const Vector4& v);
		Vector4& operator/=(const Vector4& v);

		Vector4 operator+(const Vector4& v) const;
		Vector4 operator-(const Vector4& v) const;
		Vector4 operator*(const Vector4& v) const;
		Vector4 operator/(const Vector4& v) const;

		float getMagnitude() const;
		float getMagnitudeSquared() const;
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
		Vector2::Vector2(float value) {
			this->x = value;
			this->y = value;
		}

		Vector2::Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		Vector2& Vector2::operator+=(const Vector2& v) {
			this->x += v.x;
			this->y += v.y;

			return *this;
		}

		Vector2& Vector2::operator-=(const Vector2& v) {
			this->x -= v.x;
			this->y -= v.y;

			return *this;
		}

		Vector2& Vector2::operator*=(const Vector2& v) {
			this->x *= v.x;
			this->y *= v.y;

			return *this;
		}

		Vector2& Vector2::operator/=(const Vector2& v) {
			this->x /= v.x;
			this->y /= v.y;

			return *this;
		}

		Vector2 Vector2::operator+(const Vector2& v) const {
			float new_x = this->x + v.x;
			float new_y = this->y + v.y;

			return Vector2(new_x, new_y);
		}

		Vector2 Vector2::operator-(const Vector2& v) const {
			float new_x = this->x - v.x;
			float new_y = this->y - v.y;

			return Vector2(new_x, new_y);
		}

		Vector2 Vector2::operator*(const Vector2& v) const {
			float new_x = this->x * v.x;
			float new_y = this->y * v.y;

			return Vector2(new_x, new_y);
		}

		Vector2 Vector2::operator/(const Vector2& v) const {
			float new_x = this->x / v.x;
			float new_y = this->y / v.y;

			return Vector2(new_x, new_y);
		}

		float Vector2::getMagnitude() const {
			return sqrtf(SQUARE(this->x) + SQUARE(this->y));
		}

		float Vector2::getMagnitudeSquared() const {
			return (SQUARE(this->x) + SQUARE(this->y));
		}

		void Vector2::normalize() {
			float magnitude = this->getMagnitude();
			if (magnitude != 0.0f) {
				this->x /= magnitude;
				this->y /= magnitude;
			}
		}

		float Vector2::getAngleFromOrigin() const {
			return atan2f(this->y, this->x) * (180 / PI);
		}

		// in degrees
		float Vector2::getAngleFromTarget(const Vector2& v2) const {
			Vector2 v3 = (*this) - v2;
			return atan2f(this->y, this->x) * (180 / PI);
		}

		// -------------------- Vector3 Implementation --------------------

		Vector3::Vector3(float value) {
			this->x = value;
			this->y = value;
			this->z = value;
		}

		Vector3::Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3& Vector3::operator+=(const Vector3& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;

			return *this;
		}

		Vector3& Vector3::operator-=(const Vector3& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;

			return *this;
		}

		Vector3& Vector3::operator*=(const Vector3& v) {
			this->x *= v.x;
			this->y *= v.y;
			this->z *= v.z;

			return *this;
		}

		Vector3& Vector3::operator/=(const Vector3& v) {
			this->x /= v.x;
			this->y /= v.y;
			this->z /= v.z;

			return *this;
		}

		Vector3 Vector3::operator+(const Vector3& v) const {
			float new_x = this->x + v.x;
			float new_y = this->y + v.y;
			float new_z = this->z + v.z;

			return Vector3(new_x, new_y, new_z);
		}

		Vector3 Vector3::operator-(const Vector3& v) const {
			float new_x = this->x - v.x;
			float new_y = this->y - v.y;
			float new_z = this->z - v.z;

			return Vector3(new_x, new_y, new_z);
		}

		Vector3 Vector3::operator*(const Vector3& v) const {
			float new_x = this->x * v.x;
			float new_y = this->y * v.y;
			float new_z = this->z * v.z;

			return Vector3(new_x, new_y, new_z);
		}

		Vector3 Vector3::operator/(const Vector3& v) const {
			float new_x = this->x / v.x;
			float new_y = this->y / v.y;
			float new_z = this->z / v.z;

			return Vector3(new_x, new_y, new_z);
		}

		float Vector3::getMagnitude() const {
			return sqrtf(SQUARE(this->x) + SQUARE(this->y) + SQUARE(this->z));
		}

		float Vector3::getMagnitudeSquared() const {
			return (SQUARE(this->x) + SQUARE(this->y) + SQUARE(this->z));
		}

		void Vector3::normalize() {
			float magnitude = this->getMagnitude();
			if (magnitude != 0.0f) {
				this->x /= magnitude;
				this->y /= magnitude;
				this->z /= magnitude;
			}
		}

		// -------------------- Vector4 Implementation --------------------

		Vector4::Vector4(float value) {
			this->x = value;
			this->y = value;
			this->z = value;
			this->w = value;
		}

		Vector4::Vector4(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vector4& Vector4::operator+=(const Vector4& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			this->w += v.w;

			return *this;
		}

		Vector4& Vector4::operator-=(const Vector4& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			this->w-= v.w;

			return *this;
		}

		Vector4& Vector4::operator*=(const Vector4& v) {
			this->x *= v.x;
			this->y *= v.y;
			this->z *= v.z;
			this->w *= v.w;

			return *this;
		}

		Vector4& Vector4::operator/=(const Vector4& v) {
			this->x /= v.x;
			this->y /= v.y;
			this->z /= v.z;
			this->w /= v.w;

			return *this;
		}

		Vector4 Vector4::operator+(const Vector4& v) const {
			float new_x = this->x + v.x;
			float new_y = this->y + v.y;
			float new_z = this->z + v.z;
			float new_w = this->w + v.w;

			return Vector4(new_x, new_y, new_z, new_w);
		}

		Vector4 Vector4::operator-(const Vector4& v) const {
			float new_x = this->x - v.x;
			float new_y = this->y - v.y;
			float new_z = this->z - v.z;
			float new_w = this->w - v.w;

			return Vector4(new_x, new_y, new_z, new_w);
		}

		Vector4 Vector4::operator*(const Vector4& v) const {
			float new_x = this->x * v.x;
			float new_y = this->y * v.y;
			float new_z = this->z * v.z;
			float new_w = this->w * v.w;

			return Vector4(new_x, new_y, new_z, new_w);
		}

		Vector4 Vector4::operator/(const Vector4& v) const {
			float new_x = this->x / v.x;
			float new_y = this->y / v.y;
			float new_z = this->z / v.z;
			float new_w = this->w / v.w;

			return Vector4(new_x, new_y, new_z, new_w);
		}

		float Vector4::getMagnitude() const {
			return sqrtf(SQUARE(this->x) + SQUARE(this->y) + SQUARE(this->z) + SQUARE(this->w));
		}

		float Vector4::getMagnitudeSquared() const {
			return (SQUARE(this->x) + SQUARE(this->y) + SQUARE(this->z) + SQUARE(this->w));
		}

		void Vector4::normalize() {
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