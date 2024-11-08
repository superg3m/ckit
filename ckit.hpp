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

#pragma once
#include <cmath> // for sqrtf and atan2f 
// Date: November 07, 2024
// TODO(Jovanni): replace with intrinsics later

namespace ckit {
    template <typename T>
    struct Memory {
        static void copy(void* source, void* dest, size_t source_size, size_t dest_size);
        static void zero(void* data, size_t data_size);
        static void compare(void* buffer1, void* buffer2, size_t buffer1_size, size_t buffer2_size);

        // static void insert_at_index(T* data, u32 number_of_elements, u32 data_capacity, u32 index);
        static T delete_at_index(T* data, u32 number_of_elements, u32 data_capacity, u32 index);

        static void insert_at_index(T* data, u32 number_of_elements, u32 data_capacity, u32 index) {
            assert((number_of_elements + 1) < data_capacity);
            assert(index < data_capacity - 1);
            assert(index >= 0);

            u8* byte_data = (u8*)data;

            size_t total_size = sizeof(T) * data_capacity;
            size_t source_offset = index * sizeof(T);
            size_t dest_offset = (index + 1) * sizeof(T);

            size_t payload_source_size = (number_of_elements * sizeof(T)) - source_offset;
            Memory::copy(byte_data + source_offset, byte_data + dest_offset, payload_source_size, total_size - source_offset);
        }

        static T delete_at_index(T* data, u32 number_of_elements, u32 data_capacity, u32 index) {
            assert((s32)number_of_elements - 1 >= 0);
            assert(index < data_capacity);
            assert(index >= 0);

            u8* byte_data = (u8*)data;

            size_t total_size = sizeof(T) * data_capacity;
            size_t source_offset = (index + 1) * sizeof(T);
            size_t dest_offset =  index * sizeof(T);

            size_t payload_source_size = (number_of_elements * sizeof(T)) - source_offset;
            Memory::copy(byte_data + source_offset, byte_data + dest_offset, payload_source_size, total_size - source_offset);
        }
    };

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

    template<typename T>
    struct DynamicArray {
        T* data;
        u64 capacity;
        u64 count;

        void push(T element);
        void insert(int index);
        T pop(T element);
        T remove(int index);

        void T operator[](u64 index);

        // IMPL

        void push(T element) {
            if (this->count + 1 > this->capacity) {
                this->capacity *= 2;
                T* new_data = new T[this->capacity];

                size_t array_size_in_byte = dynamic_array.count * sizeof(T);

                memory::copy(this->data, new_data, array_size_in_byte, array_size_in_byte)

                this->data = new_data;
                
            }

            this->data[this->count++] = element;
        }

        void insert(int index) {

        }

        T pop(T element) {
            assert(this->count > 0);
            return this->data[--this->count];
        }

        T remove(int index) {
            assert(this->count - 1 >= 0);
            assert(index < this->count);
            assert(index >= 0);

            u8* byte_data = (u8*)this->data;

            size_t total_size = element_size_in_bytes * data_capacity;
            size_t source_offset = (index + 1) * element_size_in_bytes;
            size_t dest_offset =  index * element_size_in_bytes;

            size_t payload_source_size = (number_of_elements * element_size_in_bytes) - source_offset;
            memory::copy(byte_data + source_offset, byte_data + dest_offset, payload_source_size, total_size - source_offset);

            return this->data[--this->count]
        }


        void T DynamicArray<T>::operator[](u64 index) {

        }
    };

    template<typename T>
    T* to_array(DynamicArray<T> dynamic_array);

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


    #if defined(CKIT_IMPL_COLLECTIONS)
        template<typename T>
        T* to_array(DynamicArray<T> dynamic_array) {
            T* ret = new T[dynamic_array.count];
            size_t array_size_in_byte = dynamic_array.count * sizeof(T);

            memory::copy(dynamic_array.data, ret, array_size_in_byte, array_size_in_byte);

            return ret;
        }
    #endif // CKIT_IMPL_COLLECTIONS
}


// Dynamic Arrays/Vector
// HashMap
// LinkedList
// Memory Ops
// Strings
// Math Ops/Structures
// Vec2, Vec3, Vec4
// Mat2, Mat3, Mat4