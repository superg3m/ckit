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
    template <u8 T_DIMENSIONS>
    struct Vector {
        union {
			struct {
				// conditional_t<(T_DIMENSIONS >= 1), float> x;
				// conditional_t<(T_DIMENSIONS >= 2), float> y;
				// conditional_t<(T_DIMENSIONS >= 3), float> z;
				// conditional_t<(T_DIMENSIONS >= 4), float> w;
				float x;
				float y;
				float z;
				float w;
			};

			float v[T_DIMENSIONS];
		};

        Vector(float value = 0.0f) {
            for (u8 i = 0; i < T_DIMENSIONS; i++) {
                v[i] = value;
            }
        }

        Vector& operator+=(const Vector& v2);
        Vector& operator-=(const Vector& v2);
        Vector& operator*=(const Vector& v2);
        Vector& operator/=(const Vector& v2);

		Vector operator+(const Vector& v2);
		Vector operator-(const Vector& v2);
		Vector operator*(const Vector& v2);
		Vector operator/(const Vector& v2);

		float getMagnitude() const;
		void normalize();
    };

	using Vector2D = Vector<2>;
	using Vector3D = Vector<3>;
	using Vector4D = Vector<4>;

	//
	// ===================================================== MATRIX =====================================================
	//

	//
	// ===================================================== CKIT_IMPL =====================================================
	//

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS>& Vector<T_DIMENSIONS>::operator+=(const Vector<T_DIMENSIONS>& v2) {
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			this->v[i] += v2.v[i];
		}
		return *this;
	}

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS>& Vector<T_DIMENSIONS>::operator-=(const Vector<T_DIMENSIONS>& v2) {
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			this->v[i] -= v2.v[i];
		}
		return *this;
	}

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS>& Vector<T_DIMENSIONS>::operator*=(const Vector<T_DIMENSIONS>& v2) {
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			this->v[i] *= v2.v[i];
		}
		return *this;
	}

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS>& Vector<T_DIMENSIONS>::operator/=(const Vector<T_DIMENSIONS>& v2) {
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			this->v[i] /= v2.v[i];
		}
		return *this;
	}

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS> Vector<T_DIMENSIONS>::operator+(const Vector<T_DIMENSIONS>& v2) {
		Vector<T_DIMENSIONS> result;
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			result.v[i] = this->v[i] + v2.v[i];
		}
		return result;
	}

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS> Vector<T_DIMENSIONS>::operator-(const Vector<T_DIMENSIONS>& v2) {
		Vector<T_DIMENSIONS> result;
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			result.v[i] = this->v[i] - v2.v[i];
		}
		return result;
	}

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS> Vector<T_DIMENSIONS>::operator*(const Vector<T_DIMENSIONS>& v2) {
		Vector<T_DIMENSIONS> result;
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			result.v[i] = this->v[i] * v2.v[i];
		}
		return result;
	}

	template <u8 T_DIMENSIONS>
	Vector<T_DIMENSIONS> Vector<T_DIMENSIONS>::operator/(const Vector<T_DIMENSIONS>& v2) {
		Vector<T_DIMENSIONS> result;
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			result.v[i] = this->v[i] / v2.v[i];
		}
		return result;
	}

	template <u8 T_DIMENSIONS>
	float Vector<T_DIMENSIONS>::getMagnitude() const {
		float sum = 0;
		for (u8 i = 0; i < T_DIMENSIONS; ++i) {
			sum += v[i] * v[i];
		}

		return sqrt(sum);
	}

	template <u8 T_DIMENSIONS>
	void Vector<T_DIMENSIONS>::normalize() {
		float magnitude = getMagnitude();
		if (magnitude != 0) {
			for (u8 i = 0; i < T_DIMENSIONS; ++i) {
				v[i] /= magnitude;
			}
		}
	}
}


// Dynamic Arrays/Vector
// HashMap
// LinkedList
// Memory Ops
// Strings
// Math Ops/Structures
// Vec2, Vec3, Vec4
// Mat2, Mat3, Mat4