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


#pragma once
#include <cmath> // for sqrtf and atan2f

namespace ckit {
    #define VECTOR_DIM 3

    float approach(const float goal, const float current_value, float dt);
    
    struct Vector {
        union {
            struct {
                float x, y, z;
            };
            float v[VECTOR_DIM];
        };

        // Constructors
        Vector(float value = 0.0f);
        Vector(float x_value, float y_value);
        Vector(float x_value, float y_value, float z_value);

        // Vector-Vector operators
        Vector& operator+=(const Vector& v);
        Vector& operator-=(const Vector& v);
        Vector& operator*=(const Vector& v);
        Vector& operator/=(const Vector& v);

        Vector operator+(const Vector& v) const;
        Vector operator-(const Vector& v) const;
        Vector operator*(const Vector& v) const;
        Vector operator/(const Vector& v) const;

        // Vector-Scalar operators
        Vector& operator+=(float scalar);
        Vector& operator-=(float scalar);
        Vector& operator*=(float scalar);
        Vector& operator/=(float scalar);

        Vector operator+(float scalar) const;
        Vector operator-(float scalar) const;
        Vector operator*(float scalar) const;
        Vector operator/(float scalar) const;

        // Scalar-Vector friend operators (to allow scalar * Vector)
        friend Vector operator+(float scalar, const Vector& v);
        friend Vector operator-(float scalar, const Vector& v);
        friend Vector operator*(float scalar, const Vector& v);
        friend Vector operator/(float scalar, const Vector& v);

        // Utility methods
        float getMagnitude() const;
        float getMagnitudeSquared() const;
        Vector normalize();
        float getAngleFromOrigin() const;
        float getAngleFromTarget(const Vector& v2) const;

        static Vector lerp(const Vector& v1, const Vector& v2, float t);
        static float dotProduct(const Vector& v1, const Vector& v2);
    };

    // ==============================================================
    //                       IMPLEMENTATION
    // ==============================================================

    #ifdef CKIT_IMPL_MATH
        float approach(const float goal, const float current_value, float dt) {
            float difference = goal - current_value;

            if (difference > dt) {
                return difference + dt;
            } else if (difference < -dt) {
                return difference - dt;
            }

            return goal;
        }

		Vector::Vector(float value) {
			this->x = value;
			this->y = value;
			this->z = value;
		}

		Vector::Vector(float x_value, float y_value) {
			this->x = x_value;
			this->y = y_value;
			this->z = 0;
		}

		Vector::Vector(float x_value, float y_value, float z_value) {
			this->x = x_value;
			this->y = y_value;
			this->z = z_value;
		}

		Vector& Vector::operator+=(const Vector& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector& Vector::operator-=(const Vector& v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}

		Vector& Vector::operator*=(const Vector& v) {
			x *= v.x; y *= v.y; z *= v.z;
			return *this;
		}

		Vector& Vector::operator/=(const Vector& v) {
			x /= v.x; y /= v.y; z /= v.z;
			return *this;
		}

		Vector Vector::operator+(const Vector& v) const {
			return Vector(x + v.x, y + v.y, z + v.z);
		}

		Vector Vector::operator-(const Vector& v) const {
			return Vector(x - v.x, y - v.y, z - v.z);
		}

		Vector Vector::operator*(const Vector& v) const {
			return Vector(x * v.x, y * v.y, z * v.z);
		}

		Vector Vector::operator/(const Vector& v) const {
			return Vector(x / v.x, y / v.y, z / v.z);
		}

		Vector& Vector::operator+=(float scalar) {
			x += scalar; y += scalar; z += scalar;
			return *this;
		}

		Vector& Vector::operator-=(float scalar) {
			x -= scalar; y -= scalar; z -= scalar;
			return *this;
		}

		Vector& Vector::operator*=(float scalar) {
			x *= scalar; y *= scalar; z *= scalar;
			return *this;
		}

		Vector& Vector::operator/=(float scalar) {
			x /= scalar; y /= scalar; z /= scalar;
			return *this;
		}

		Vector Vector::operator+(float scalar) const {
			return Vector(x + scalar, y + scalar, z + scalar);
		}

		Vector Vector::operator-(float scalar) const {
			return Vector(x - scalar, y - scalar, z - scalar);
		}

		Vector Vector::operator*(float scalar) const {
			return Vector(x * scalar, y * scalar, z * scalar);
		}

		Vector Vector::operator/(float scalar) const {
			return Vector(x / scalar, y / scalar, z / scalar);
		}

		// Scalar-Vector friend operators
		Vector operator+(float scalar, const Vector& v) {
			return v + scalar;
		}

		Vector operator-(float scalar, const Vector& v) {
			return Vector(scalar - v.x, scalar - v.y, scalar - v.z);
		}

		Vector operator*(float scalar, const Vector& v) {
			return v * scalar;
		}

		Vector operator/(float scalar, const Vector& v) {
			return Vector(scalar / v.x, scalar / v.y, scalar / v.z);
		}

		float Vector::getMagnitude() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		float Vector::getMagnitudeSquared() const {
			return x * x + y * y + z * z;
		}

		Vector Vector::normalize() {
			float magnitude = getMagnitude();
			if (magnitude > 0.0f) {
				*this /= magnitude;
			}
			return *this;
		}

		float Vector::getAngleFromOrigin() const {
			return std::atan2(y, x) * (180.0f / PI);
		}

		float Vector::getAngleFromTarget(const Vector& v2) const {
			Vector delta = *this - v2;
			return delta.getAngleFromOrigin();
		}

		Vector Vector::lerp(const Vector& v1, const Vector& v2, float t) {
			return ((1.0f - t) * v1) + (t * v2);
		}

		float Vector::dotProduct(const Vector& v1, const Vector& v2) {
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}
    #endif // CKIT_IMPL_MATH
}


// Dynamic Arrays/Vector
// HashMap
// LinkedList
// Memory Ops
// Strings
// Math Ops/Structures
// Vec2, Vec3, Vec4
// Mat2, Mat3, Mat4