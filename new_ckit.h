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
	typedef CKG_Alloc_T CKIT_Alloc_T;
	typedef CKG_Free_T CKIT_Free_T;
    typedef CKG_Allocator CKIT_Allocator;

    void* ckit_allocator_wrapper(CKIT_Allocator* allocator, size_t allocation_size);
    void ckit_free_wrapper(CKIT_Allocator* allocator, void* data);
#endif

void ckit_init() {
    // this needs to make the 
    ckg_bind_custom_allocator(ckit_allocator_wrapper, ckit_free_wrapper, NULLPTR);
    // ckit_tracker_init();
    // memory_init();
    // platform_console_init();
    // ckit_str_register_arena();
}

#if defined(CKIT_IMPL_MEMORY)
	typedef CKG_Alloc_T CKIT_Alloc_T;
	typedef CKG_Free_T CKIT_Free_T;
    typedef CKG_Allocator CKIT_Allocator;

    CKIT_API void ckit_bind_custom_allocator(CKIT_Alloc_T* a, CKIT_Alloc_T* f, void* ctx) {
        ckit_assert_msg(a, "Alloc function is NULLPTR\n");
        ckit_assert_msg(f, "Free function is NULLPTR\n");

        allocator.allocator = a;
        allocator.free = f;
        if (ctx) {
            allocator.ctx = ctx;
        }

        ckg_bind_custom_allocator(ckit_allocator_wrapper, ckit_free_wrapper, ctx);
    }

    void* ckit_allocator_wrapper(CKIT_Allocator* allocator, size_t allocation_size) {
        (void)allocator;

   
        u8* ret = allocator.allocate(sizeof(CKIT_MemoryHeader) + allocation_size);
        // memory_tagging
        // register memory

        ret += sizeof(CKIT_MemoryHeader);

        return ret;
    }

    void ckit_free_wrapper(CKIT_Allocator* allocator, void* data) {
        // memory_tagging 

        u8* base = (u8*)data - sizeof(CKIT_MemoryHeader);
        // memory_tagging
        // unregister memory

        allocator.free(data);
    }

	internal CKIT_Allocator allocator = {ckg_default_libc_malloc, ckg_default_libc_free, NULLPTR};
#endif

