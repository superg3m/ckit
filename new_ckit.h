#pragma once

#ifdef __cplusplus
    #define CKIT_API extern "C"
#else
    #define CKIT_API
#endif

#if defined(CKIT_IMPL)
    #define CKIT_IMPL_INIT
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

    // Date: September 25, 2024
    // TODO(Jovanni): CKIT_IMPL_THREADING
    // TODO(Jovanni): CKIT_IMPL_SIMD
    // TODO(Jovanni): CKIT_IMPL_CUDA
    // TODO(Jovanni): CKIT_IMPL_FILE_WATCHER
    // TODO(Jovanni): CKIT_IMPL_PARSER
#endif

#define CKIT_INCLUDE_INIT
#define CKIT_INCLUDE_LOGGER
#define CKIT_INCLUDE_ASSERT
#define CKIT_INCLUDE_MEMORY
#define CKIT_INCLUDE_ARENA
#define CKIT_INCLUDE_STRING
#define CKIT_INCLUDE_MATH
#define CKIT_INCLUDE_CHAR
#define CKIT_INCLUDE_COLLECTIONS
#define CKIT_INCLUDE_OS
#define CKIT_INCLUDE_UTILITIES


#if defined(CKIT_INCLUDE_INIT)
	#include "./ckg/ckg.h"

	void ckit_init();
	void ckit_cleanup(bool generate_memory_report);
#endif


#if defined(CKIT_INCLUDE_MEMORY)
	typedef struct CKIT_Allocator CKIT_Allocator;
	typedef void*(CKIT_Alloc_T)(CKIT_Allocator* allocator, size_t allocation_size);
	typedef void(CKIT_Free_T)(CKIT_Allocator* allocator, void* data);

	typedef struct CKIT_Allocator {
		CKIT_Alloc_T* allocate;
		CKIT_Free_T* free;
		void* ctx;
	} CKIT_Allocator;
#endif

#if defined(CKIT_IMPL_MEMORY)
	typedef struct CKIT_Allocator CKIT_Allocator;
	typedef void*(CKIT_Alloc_T)(CKIT_Allocator* allocator, size_t allocation_size);
	typedef void(CKIT_Free_T)(CKIT_Allocator* allocator, void* data);

	typedef struct CKIT_Allocator {
		CKIT_Alloc_T* allocate;
		CKIT_Free_T* free;
		void* ctx;
	} CKIT_Allocator;

	internal void* ckit_default_libc_malloc(CKG_Allocator* allocator, size_t allocation_size) {
        (void)allocator;
        return ckg_alloc(allocation_size);
    }

    internal void ckit_default_libc_free(CKG_Allocator* allocator, void* data) {
        (void)allocator;
        ckg_free(data);
    }

	internal CKIT_Allocator allocator = {ckit_default_libc_malloc, ckit_default_libc_free, 0};
#endif