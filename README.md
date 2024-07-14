# Ckits's Core Thesis
- The point of the ckit is to provide a nice library to build c projects with.
- Ckit is built with the granular api ckit granular [CKG](https://github.com/superg3m/ckit)
- This library will be completly cross-platform
- Collection of tools, data structures, and algorithms that I find useful

# How to build
1. ./bootstrap.ps1
2. ./build.ps1 or ./build.ps1 -debug
2. ./debug.ps1
2. ./run.ps1

Thoughts:
	Vectors right now in their conception have a really big problem ckit's dyanmic arrays (vectors) rely on a header component before the actual 
	data. This is a problem because taking a trivial example like a 2d dynamic array, if you push an array into a 2d array there is a high probability that the pointer to the array will get invalidated because when the dynamic array grows it changes address meaning I would somehow have to track references and update those. Updating refrences is a fools errand so what choices do we have?
	OPTIONS
		#1. Mandate that the code be written in a specific way that avoid this problem.
			- very error prone (I litterally made this error myself when writing spline points)
		#2 

# Goals
### key
- '[x]': Finished
- '[/]': Started working on it
- '[ ]': Haven't started working on it

## CORE
- [x] ckit_types.c

### Assert (Cross-Platform) (Fully inherits CKG impl)
- [x] ckit_assert.h
	- [x] ckit_assert_msg(expression, msg, ...)
	- [x] ckit_assert(expression)

### Logger (Cross-Platform) (Fully inherits CKG impl)
- [x] ckit_logger.h
	- [x] ckit_LOG_FATAL(msg, ...)
	- [x] ckit_LOG_ERROR(msg, ...)
	- [x] ckit_LOG_WARN(msg, ...)
	- [x] ckit_LOG_DEBUG(msg, ...)
	- [x] ckit_LOG_SUCCESS(msg, ...)
	- [x] ckit_LOG_PRINT(msg, ...)

### Collection
- [ ] ckit_hashmap.h
- [ ] ckit_linked_list.h
- [ ] ckit_stack.h
- [ ] ckit_queue.h
- [ ] ckit_binary_tree.h

- [x] ckit_vector.h
	- [x] ckg_vector_length(vector)
	- [x] ckg_vector_capacity(vector)
	- [x] ckg_vector_push(vector, element)
	- [x] ckg_vector_free(vector)

### String
- [x] ckit_string.h
	- [x] ckit_str_create(str)
	- [x] ckit_str_append(str, string_buffer_capacity, to_append)
	- [x] ckit_str_append_char(str, string_buffer_capacity, char to_append)
	- [x] ckit_str_insert(str, string_buffer_capacity, to_insert, index)
	- [x] ckit_str_insert_char(str, string_buffer_capacity, char to_insert, index)
	- [x] ckit_str_copy(str, string_buffer_capacity, to_copy)
	- [x] ckit_str_random(dest, length)
	- [x] ckit_str_equal(s1, s2)
	- [x] ckit_cstr_length(c_string or ckit_string)
	- [x] ckit_str_length(ckit_string only)
	- [x] ckit_str_clear(str)
	- [x] ckit_substring(str, start_range, end_range)
	- [x] ckit_str_contains(str, contains)
	- [x] ckit_str_index_of(str, sub_string)
	- [x] ckit_str_last_index_of(str, sub_string)
	- [x] ckit_str_split(str, delimitor)
	- [x] ckit_str_starts_with(str, starts_with)
	- [x] ckit_str_ends_with(str, ends_with)
	- [x] ckit_str_reverse(str)

### Memory
- [x] ckit_memory.h
    - [x] ckit_bind_alloc_callback(func_allocator)
    - [x] ckit_bind_free_callback(func_allocator)
    - [x] ckit_alloc(allocation_size)
    - [x] ckit_realloc(data, old_allocation_size, new_allocation_size)
    - [x] ckit_free(data)
    - [x] ckit_memory_compare(buffer_one, buffer_two, b1_allocation_size, b2_allocation_size)
    - [x] ckit_memory_copy(source, destination, source_size, destination_capacity)
    - [x] ckit_memory_move(source, destination, source_payload_size)
    - [x] ckit_memory_zero(data, data_size_in_bytes)
    - [x] ckit_memory_delete_index(data, data_capacity, element_size_in_bytes, index)

- [/] ckit_arena.h (handling dangling pointers is gonna be pretty tough here)
	- [/] ckit_arena_create(allocation, name, flag)
	- [/] ckit_arena_push(arena, type, byte_alignment = 4)
	- [/] ckit_arena_free(arena)
	- [/] ckit_arena_clear(arena)

### FileIO (Finished, but I dont like it gonna rewrite it)
- [ ] ckit_file_io.h
	- [x] ckit_file_system_create(file_name)
	- [x] ckit_file_open(file_system)
	- [x] ckit_file_close(file_system)
	- [x] ckit_file_size(file_system)
	- [x] ckit_file_get_next_line(file_system)
	- [x] ckit_file_get_next_char(file_system)

### Threading
- [ ] ckit_locks.h
	- [ ] ckit_threading_interlock_exchange()
- [ ] ckit_job_system.h
- [ ] ckit_semaphores.h
- [ ] ckit_sleep.h


## MODULES

### Hot_Reloader
- [ ] ckit_file_watcher.h
- [ ] ckit_dynamic_libary_loader.h

### Introspection
- [ ] ckit_struct_introspection.h

### Networking
- [ ] ckit_client.h
- [ ] ckit_server.h

### Intrinsics (PROB CORE)
- [ ] compiler_memory_fences
- [ ] interlock_exchange
- [ ] math_functions
	- sin
	- cos
	- tan

### SIMD
make vector operations easier

### Parser
- Asset
	- [x] BMP
	- [ ] PNG
	- [ ] OBJ
	- [ ] PDF
- JSON

## Tests
- [ ] test_memory_functions()
- [ ] test_arena_functions()
- [ ] test_vector_functions()
- [ ] test_string_functions()

