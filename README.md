# CKIT
The point of ckit is the provide a nice library for me to build projects in c.

# How to build
1. ./bootstrap.ps1
2. ./build.ps1 or ./build.ps1 -debug
2. ./debug.ps1
2. ./run.ps1

- Collection of tools, data structures, and algorithms that I find useful

Thoughts:
 - I need to rethink memory allocations
 - so heres the think with arenas I won't have tons of syscalls for memory allocations so that way I can use a function pointer for the allocation function so default stub is malloc and free, you can define you own callback function
 - I need to rethink how arenas work

Next course of action is to remove all memory allocation tracking, the way I did it was really bad so its hard to actually keep working on anything.
if I want to keep using header I need to make available the header size with a function so I can then say you must specify the size not just the type
it kind of is the case that you want to init some type of memory arena and just push stuff because of the type system and maybe that is better overall?

# Goals
#### key
- '[x]': Finished
- '[/]': Started working on it
- '[ ]': Haven't started working on it

## CORE
- [x] ckit_types.c

### Assert (Not Cross-Platform) (Fully inherits CKG impl)
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

### String
- [ ] ckit_cstring.h
	- [x] ckit_str_create(str)
	- [x] ckit_str_append(str, string_buffer_capacity, to_append)
	- [x] ckit_str_append_char(str, string_buffer_capacity, char to_append)
	- [x] ckit_str_insert(str, string_buffer_capacity, to_insert, index)
	- [x] ckit_str_insert_char(str, string_buffer_capacity, char to_insert, index)
	- [x] ckit_str_copy(str, string_buffer_capacity, to_copy)
	- [x] ckit_str_random(dest, length)
	- [x] ckit_str_equal(s1, s2)
	- [x] ckit_str_length(c_string)
	- [x] ckit_str_clear(str)
	- [x] ckit_substring(str, start_range, end_range)
	- [/] ckit_str_contains(str, contains)
	- [/] ckit_str_index_of(str, sub_string)
	- [/] ckit_str_last_index_of(str, sub_string)
	- [x] ckit_str_split(str, delimitor)
	- [/] ckit_str_starts_with(str, starts_with)
	- [/] ckit_str_ends_with(str, ends_with)
	- [/] ckit_str_reverse(str)

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

- [ ] ckit_arena.h
	- [ ] ckit_arena_create(allocation, name, flag)
	- [ ] ckit_arena_push(arena, type)	
	- [ ] ckit_arena_free(arena)
	- [ ] ckit_arena_clear(arena)

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
	
### Threading
	- [ ] ckit_locks.h
		- [ ] ckit_threading_interlock_exchange()
	- [ ] ckit_semaphores.h
	- [ ] ckit_

### SIMD
	make vector operations easier

### Parser
	- Asset
		- [x] BMP
		- [ ] PNG
		- [ ] OBJ
		- [ ] PDF
	- JSON

### SIMD

## Tests
- [ ] test_memory_functions()
- [ ] test_arena_functions()
- [ ] test_vector_functions()
- [ ] test_string_functions()

